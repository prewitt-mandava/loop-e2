#define DT_DRV_COMPAT umdloop_servo

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <loop/servo.h>

struct servo_config {
	struct pwm_dt_spec pwm;
	uint32_t min_pulse_us;
	uint32_t max_pulse_us;
};

static int servo_set_angle_impl(const struct device *dev, uint8_t degrees)
{
	const struct servo_config *cfg = dev->config;
	uint32_t pulse_us;

	if (degrees > 180) {
		return -EINVAL;
	}

	/* Linear map: 0 deg -> min pulse, 180 deg -> max pulse */
	pulse_us = cfg->min_pulse_us +
		   ((cfg->max_pulse_us - cfg->min_pulse_us) * degrees) / 180;

	/* Period stays fixed at 20 ms (from devicetree); only pulse width changes */
	return pwm_set_pulse_dt(&cfg->pwm, pulse_us * NSEC_PER_USEC);
}

static int servo_init(const struct device *dev)
{
	const struct servo_config *cfg = dev->config;

	if (!pwm_is_ready_dt(&cfg->pwm)) {
		return -ENODEV;
	}

	/* No pulse at boot: servo stays wherever it is until commanded */
	return pwm_set_pulse_dt(&cfg->pwm, 0);
}

static const struct servo_driver_api servo_api = {
	.set_angle = servo_set_angle_impl,
};

#define SERVO_DEFINE(inst)                                               \
	static const struct servo_config servo_config_##inst = {         \
		.pwm = PWM_DT_SPEC_INST_GET(inst),                       \
		.min_pulse_us = DT_INST_PROP(inst, min_pulse_us),        \
		.max_pulse_us = DT_INST_PROP(inst, max_pulse_us),        \
	};                                                               \
	DEVICE_DT_INST_DEFINE(inst, servo_init, NULL, NULL,              \
			      &servo_config_##inst, POST_KERNEL,         \
			      CONFIG_SERVO_INIT_PRIORITY, &servo_api);

DT_INST_FOREACH_STATUS_OKAY(SERVO_DEFINE)