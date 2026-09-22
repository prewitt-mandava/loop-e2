# loop-e2

Zephyr module + test app for E2, System 2 (servo). Type an angle (0-180) in the
serial monitor and the servo turns to it. Servo signal on GPIO 18.

## Build & run

Every new terminal:
```bash
source ~/zephyrproject/.venv/bin/activate
cd ~/zephyrproject/loop-e2/app
```
Build and flash:
```bash
west build -p always -b esp32_devkitc/esp32/procpu
west flash
```
Open the serial monitor (find the port with `ls /dev/cu.*`; leave with Ctrl-A, K, Y):
```bash
screen /dev/cu.usbserial-XXXX 115200
```
Type a number 0-180 and press Enter. The servo moves and prints the angle back.

## Layout
- `drivers/servo/` - the driver (`servo_set_angle()`)
- `include/loop/servo.h` - its public API
- `dts/bindings/` - devicetree bindings (servo done; speaker binding only, no driver yet)
- `app/` - the test application (`src/main.c`) and board wiring (`app/boards/*.overlay`)
