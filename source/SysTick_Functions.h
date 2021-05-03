/*
 * SysTick_Functions.h
 *
 *  Created on: Feb 27, 2021
 *      Author: Dhruv Mehta
 */

#ifndef SYSTICK_FUNCTIONS_H_
#define SYSTICK_FUNCTIONS_H_

#include <stdint.h>
#include "MKL25Z4.h"

/* Type Definitions */
typedef uint32_t ticktime_t;

/* Function Prototypes */

/**
 * @name    init_ticktime.
 *
 * @brief   Initializes the SysTick VAL, LOAD and CTRL Registers
 * 			and starts the Timer.
 *
 * @param	void
 *
 * @return  void
 *
 */
void Ticktime_Init();

/**
 * @name    now.
 *
 * @brief   Returns the time since system boot.
 *
 * @param	void
 *
 * @return  ticktime_t - boot_time - Time since system start.
 *
 */
ticktime_t now();

/**
 * @name    reset_timer.
 *
 * @brief   Resets the timer by setting reset_timer = timer_time.
 *
 * @param	void
 *
 * @return  void
 *
 */
void reset_timer();

/**
 * @name    get_timer.
 *
 * @brief   Gets the time since last reset.
 *
 * @param	void
 *
 * @return  ticktime_t - timer_time - Time since last reset.
 *
 */
ticktime_t get_timer();

/**
 * @name    delay.
 *
 * @brief   Blocking delay call for delay_millis milliseconds.
 *
 * @param	ticktime_t - delay_millis - Time to delay in milliseconds.
 * 			NOTE : Only delays in multiples of 50ms.
 *
 * @return  void
 *
 */
void delay(ticktime_t delay_millis);

/**
 * @name    SysTick_Handler.
 *
 * @brief	ISR for SysTick Interrupts, increments boot timer
 * 			and timer time global variables.
 *
 * @param	void
 *
 * @return  void
 *
 */

void SysTick_Handler();

#endif /* SYSTICK_FUNCTIONS_H_ */
