/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/led.h>
#include <zephyr/kernel.h>

#define LED1_NODE DT_NODELABEL(userled)
static const struct gpio_dt_spec led_spec = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
#if !DT_NODE_EXISTS(LED1_NODE)
#error "whoops"
#endif

int main(void) {
  if (!device_is_ready(led_spec.port)) {
    printk("LED 设备未就绪\n");
    return 0;
  }
  gpio_pin_configure_dt(&led_spec, GPIO_OUTPUT_ACTIVE);
  while (1) {
    gpio_pin_toggle_dt(&led_spec);
    k_sleep(K_MSEC(200));
  }
}

// #include <zephyr/kernel.h>
// #include <zephyr/drivers/gpio.h>

// /* 500 msec = 0.5 sec */
// #define SLEEP_TIME_MS   500

// /* The devicetree node identifier for the "led0" alias. */
// #define LED0_NODE DT_ALIAS(led0)

// /*
//  * A build error on this line means your board is unsupported.
//  * See the sample documentation for information on how to fix this.
//  */
// static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

// int main(void)
// {
// 	int ret;

// 	// if (!gpio_is_ready_dt(&led)) {
// 	// 	return 0;
// 	// }

// 	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
// 	// if (ret < 0) {
// 	// 	return 0;
// 	// }

// 	while (1) {
// 		ret = gpio_pin_toggle_dt(&led);
// 		// if (ret < 0) {
// 		// 	return 0;
// 		// }

// 		k_msleep(SLEEP_TIME_MS);
// 	}
// 	return 0;
// }
