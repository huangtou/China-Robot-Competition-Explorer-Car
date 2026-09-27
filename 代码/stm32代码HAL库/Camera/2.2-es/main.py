from maix import camera, display, image, nn, app, comm, uart
import struct, os

# 一开机关闭串口内核打印，消除系统日志干扰单片机
os.system("echo 1 > /proc/sys/kernel/printk")
os.system("dmesg -n 0")

report_on = True
APP_CMD_DETECT_RES = 0x02
# Serial settings. The MCU receives frame: 0xAA + ASCII_NUM + 0xBB
UART_DEVICE = "/dev/ttyS0"
UART_BAUDRATE = 115200

# 模型类别映射
CLASS_TO_NUMBER = ("3", "4", "5", "1", "2")
CONF_THRESHOLD = 0.60
VOTE_WINDOW_FRAMES = 5
MIN_VOTES = 2
SEND_REPEAT = 3
RESEND_INTERVAL_FRAMES = 5
NO_TARGET_RESET_FRAMES = 60

# 协议帧定义
FRAME_HEAD = bytes([0xAA])
FRAME_TAIL = bytes([0xBB])

def send_cam_serial(num_str: str):
    """发送标准帧 0xAA + 字符 + 0xBB"""
    frame = FRAME_HEAD + num_str.encode("ascii") + FRAME_TAIL
    for _ in range(SEND_REPEAT):
        serial0.write(frame)

def encode_objs(objs):
    body = b''
    for obj in objs:
        body += struct.pack("<hhHHHf", obj.x, obj.y, obj.w, obj.h, obj.class_id, obj.score)
    return body

model_path = "model_316110.mud"
if not os.path.exists(model_path):
    model_path = "/root/models/maixhub/316110/model_316110.mud"

detector = nn.YOLOv5(model=model_path)
cam = camera.Camera(detector.input_width(), detector.input_height(), detector.input_format())
dis = display.Display()
serial0 = uart.UART(UART_DEVICE, UART_BAUDRATE)
p = comm.CommProtocol(buff_size = 1024)

last_sent_number = None
vote_history = []
frames_since_send = 0
no_target_frames = 0

while not app.need_exit():
    img = cam.read()
    objs = detector.detect(img, conf_th = CONF_THRESHOLD, iou_th = 0.45)

    if len(objs) > 0 and report_on:
        body = encode_objs(objs)
        p.report(APP_CMD_DETECT_RES, body)

    valid_objs = [obj for obj in objs if 0 <= obj.class_id < len(CLASS_TO_NUMBER)]
    if valid_objs:
        no_target_frames = 0
        best_obj = max(valid_objs, key=lambda obj: obj.score)
        detected_number = CLASS_TO_NUMBER[best_obj.class_id]

        # Moving-mode vote: tolerate occasional missed or incorrect frames.
        vote_history.append(detected_number)
        if len(vote_history) > VOTE_WINDOW_FRAMES:
            vote_history.pop(0)

        confirmed_number = None
        confirmed_votes = 0
        for number in CLASS_TO_NUMBER:
            votes = vote_history.count(number)
            if votes > confirmed_votes:
                confirmed_number = number
                confirmed_votes = votes

        # Prefer the current detection when two numbers have equal votes.
        if vote_history.count(detected_number) == confirmed_votes:
            confirmed_number = detected_number

        if confirmed_votes >= MIN_VOTES:
            frames_since_send += 1
            if (confirmed_number != last_sent_number or
                    frames_since_send >= RESEND_INTERVAL_FRAMES):
                send_cam_serial(confirmed_number)
                last_sent_number = confirmed_number
                frames_since_send = 0
            # 注释print，防止控制台日志干扰串口
            # print("UART sent:", detected_number)
    else:
        vote_history.append(None)
        if len(vote_history) > VOTE_WINDOW_FRAMES:
            vote_history.pop(0)
        no_target_frames += 1
        if (last_sent_number is not None and
                no_target_frames < NO_TARGET_RESET_FRAMES):
            # Keep resending the last stable result while the target is
            # temporarily out of view, giving the MCU time to read it.
            frames_since_send += 1
            if frames_since_send >= RESEND_INTERVAL_FRAMES:
                send_cam_serial(last_sent_number)
                frames_since_send = 0
        if no_target_frames >= NO_TARGET_RESET_FRAMES:
            if last_sent_number is not None:
                # 无目标发送 0
                send_cam_serial("0")
                last_sent_number = None
                frames_since_send = 0

    # 绘制框与数字
    for obj in objs:
        img.draw_rect(obj.x, obj.y, obj.w, obj.h, color = image.COLOR_RED)
        if 0 <= obj.class_id < len(CLASS_TO_NUMBER):
            img.draw_string(obj.x, obj.y, CLASS_TO_NUMBER[obj.class_id], color = image.COLOR_RED)
    dis.show(img)
