
from maix import camera, display, image, nn, app, comm, uart
import struct, os

# Disable kernel messages on UART to avoid corrupting MCU frames.
os.system("echo 1 > /proc/sys/kernel/printk")
os.system("dmesg -n 0")

report_on = True
APP_CMD_DETECT_RES = 0x02

# UART frame: 0xAA + ASCII number + 0xBB.
UART_DEVICE = "/dev/ttyS0"
UART_BAUDRATE = 115200
FRAME_HEAD = bytes([0xAA])
FRAME_TAIL = bytes([0xBB])

# The model's class order is 3, 4, 5, 1, 2 (see report.json), rather than
# numerical order. Keep the mapping explicit so that the transmitted and
# displayed number is always correct.
CLASS_TO_NUMBER = ("3", "4", "5", "1", "2")
CONF_THRESHOLD = 0.65
STABLE_FRAMES = 4
RESEND_INTERVAL_FRAMES = 10
NO_TARGET_RESET_FRAMES = 60


def send_cam_serial(number):
    frame = FRAME_HEAD + number.encode("ascii") + FRAME_TAIL
    serial0.write(frame)

def encode_objs(objs):
    '''
        encode objs info to bytes body for protocol
        2B x(LE) + 2B y(LE) + 2B w(LE) + 2B h(LE) + 2B idx + 4B score(float) ...
    '''
    body = b''
    for obj in objs:
        body += struct.pack("<hhHHHf", obj.x, obj.y, obj.w, obj.h, obj.class_id, obj.score)
    return body

model_path = "model_316024.mud"
if not os.path.exists(model_path):
    model_path = "/root/models/maixhub/316024/model_316024.mud"
detector = nn.YOLOv5(model=model_path)

cam = camera.Camera(detector.input_width(), detector.input_height(), detector.input_format())
dis = display.Display()
serial0 = uart.UART(UART_DEVICE, UART_BAUDRATE)

p = comm.CommProtocol(buff_size = 1024)
last_sent_number = None
candidate_number = None
candidate_count = 0
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

        if detected_number == candidate_number:
            if candidate_count < STABLE_FRAMES:
                candidate_count += 1
        else:
            candidate_number = detected_number
            candidate_count = 1
            frames_since_send = 0

        if candidate_count >= STABLE_FRAMES:
            frames_since_send += 1
            if (candidate_number != last_sent_number or
                    frames_since_send >= RESEND_INTERVAL_FRAMES):
                send_cam_serial(candidate_number)
                last_sent_number = candidate_number
                frames_since_send = 0
    else:
        candidate_number = None
        candidate_count = 0
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
                send_cam_serial("0")
                last_sent_number = None
                frames_since_send = 0

    for obj in objs:
        img.draw_rect(obj.x, obj.y, obj.w, obj.h, color = image.COLOR_RED)
        if 0 <= obj.class_id < len(CLASS_TO_NUMBER):
            # Only draw the number; do not draw the unsupported Chinese label.
            img.draw_string(obj.x, obj.y, CLASS_TO_NUMBER[obj.class_id], color = image.COLOR_RED)
    dis.show(img)
