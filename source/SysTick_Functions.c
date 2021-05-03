/*
 * SysTick_Functions.c
 *
 *  Created on: Feb 27, 2021
 *      Author: Dhruv Mehta
 *      Brief : Contains implementations for the SysTick
 *      		Interrupt functionality.
 */

#include <SysTick_Functions.h>

#define SYSTICK_RELOAD (24000000/20)

/* timer_time = time since last reset (timer_time - reset_time)
 * reset_time = used internally and set to timer_time when reset_timer is called
 * boot_time = time since SysTick is initialised
 */
static ticktime_t timer_time, reset_time, boot_time;


void Ticktime_Init()
{
	/* Initialize Counter Value to 0 */
	SysTick->VAL = 0;

	/* Set the LOAD Value to 1200000 for 50ms Ticks */
	SysTick->LOAD = (SYSTICK_RELOAD);

	/* Set the Control Register for Internal ClockSource (24Mhz)
	 * Enable Interrupt on Overflow and Enable the Timer */
	SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);

}

ticktime_t now()
{
	return boot_time*50;
}

void reset_timer()
{
	reset_time = timer_time;
}

ticktime_t get_timer()
{
	return (timer_time - reset_time);
}

void delay(ticktime_t delay_millis)
{
	ticktime_t delay_time;

	delay_millis /= 50;
	delay_time = get_timer();

	while((get_timer() - delay_time) < delay_millis);
}

void SysTick_Handler()
{
	timer_time++;
	boot_time++;
}
