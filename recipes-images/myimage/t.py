import serial
import uinput

ser = serial.Serial('/dev/ttyS1', 115200, timeout=100)

last_buttons_mask = 0

first_start = True

events = (
    uinput.BTN_A,
    uinput.BTN_B,
    uinput.BTN_X,
    uinput.BTN_Y,
    uinput.BTN_TL,
    uinput.BTN_TR,
    uinput.BTN_THUMBL,
    uinput.BTN_THUMBR,
    uinput.BTN_START,
    uinput.BTN_SELECT,
    uinput.BTN_TL2,
    uinput.BTN_TR2,
    uinput.ABS_X + (0, 255, 0, 0),
    uinput.ABS_Y + (0, 255, 0, 0),
    uinput.ABS_Z + (0, 255, 0, 0),
    uinput.ABS_RX + (0, 255, 0, 0),
    uinput.ABS_RY + (0, 255, 0, 0),
    uinput.ABS_RZ + (0, 255, 0, 0),
)

buttons = (
    (0x01, uinput.BTN_A),
    (0x02, uinput.BTN_B),
    (0x04, uinput.BTN_X),
    (0x08, uinput.BTN_Y),
    (0x10, uinput.BTN_TL2),
    (0x20, uinput.BTN_TR2),
    (0x400, uinput.BTN_TL),
    (0x800, uinput.BTN_THUMBL),
    (0x1000, uinput.BTN_TR),
    (0x2000, uinput.BTN_THUMBR),
    (0x0400, uinput.BTN_START),
    (0x0800, uinput.BTN_SELECT)
)

device = uinput.Device(
    events,
    vendor=0x045e,
    product=0x028e,
    version=0x110,
    name="Microsoft X-Box 360 pad",
)

def process_buttons(current_buttons_mask):
    global first_start
    global last_buttons_mask
    global buttons
    global device
    if first_start == True:
        last_buttons_mask = current_buttons_mask
        first_start = False

    for it in buttons:
        if (it[0] & current_buttons_mask) != (it[0] & last_buttons_mask):
            lvl = 1 if (it[0] & current_buttons_mask) > 0 else 0
            device.emit(it[1], lvl)

    last_buttons_mask = current_buttons_mask

while 1:
    s = ser.read(2)
    if s[0] == 171 and s[1] == 171:
        s = ser.read(6)
        buttons_mask = s[1] << 8 | s[0]
        process_buttons(buttons_mask)
        LX = s[2]
        LY = s[3]
        RX = s[4]
        RY = s[5]
        device.emit(uinput.ABS_X, LX)
        device.emit(uinput.ABS_Y, LY)
        device.emit(uinput.ABS_RX, RX)
        device.emit(uinput.ABS_RY, RY)

ser.close()

device.destroy()
