#ifndef LOOP_SERVO_H_
#define LOOP_SERVO_H_

#include <zephyr/device.h>
#include <stdint.h>

typedef int (*servo_set_angle_t)(const struct device *dev, uint8_t degrees);

struct servo_driver_api {
	servo_set_angle_t set_angle;
};

/* Move to 0-180 degrees. Returns -EINVAL if out of range. */
static inline int servo_set_angle(const struct device *dev, uint8_t degrees)
{
	const struct servo_driver_api *api = dev->api;

	return api->set_angle(dev, degrees);
}

#endif /* LOOP_SERVO_H_ */