#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/console/console.h>
#include <stdlib.h>
#include <loop/servo.h>

/* Based on teammate's System Two app; hardware access moved into the servo driver */
static const struct device *const servo = DEVICE_DT_GET(DT_ALIAS(servo0));

int main(void)
{
	if (!device_is_ready(servo)) {
		printk("servo not ready\n");
		return 0;
	}

	console_init();
	printk("Enter an angle from 0 to 180:\n");

	while (1) {
		char input[4];
		int index = 0;
		int angle;
		int ret;

		/* Read one line, keeping only digits */
		while (1) {
			int c = console_getchar();

			if (c == '\r' || c == '\n') {
				break;
			}
			if (c >= '0' && c <= '9' && index < 3) {
				input[index++] = (char)c;
			}
		}

		/* Blank line (e.g. the \n after \r): ignore instead of moving to 0 */
		if (index == 0) {
			continue;
		}

		input[index] = '\0';
		angle = atoi(input);

		if (angle > 180) {
			printk("Please enter a value from 0 to 180.\n");
			continue;
		}

		ret = servo_set_angle(servo, (uint8_t)angle);
		if (ret < 0) {
			printk("servo error: %d\n", ret);
		} else {
			printk("Servo moved to: %d degrees\n", angle);
		}
	}

	return 0;
}