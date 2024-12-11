#include "project.h"

void gpio_config(void)
{
	/* enable the LED clock */
    rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_GPIOD);
    /* configure LED GPIO port */
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_14);		/* led */
	gpio_init(GPIOD, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);		/* beep */
    /* reset LED GPIO pin */
    gpio_bit_reset(GPIOC, GPIO_PIN_14);
	gpio_bit_set(GPIOD, GPIO_PIN_2);						/*设为高，蜂鸣器不响*/
}

