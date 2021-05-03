/*
 * main.c
 *
 *  Created on: Apr 27, 2021
 *      Author: Dhruv
 *      Brief : Application entry point to a terminal-based Maze game
 *      		which uses the on board accelerometer (MMA8451) to
 *      		control the movements in the maze. Consists of two
 *      		levels to play. Just plug in your board, follow the
 *      		instructions and play!
 *
 *      		(Requires a terminal emulator on PC with the settings:
 *      		Baud rate - 38400, 8 data bits, 1 stop bit, no parity)
 *
 */
#include "assert.h"
#include "sysclock.h"
#include "I2C.h"
#include "UART.h"
#include "MMA8451.h"
#include "TSI_functions.h"
#include "Game.h"
#include "SysTick_Functions.h"
#include "Testsuite.h"

/*
 * @brief   Application entry point.
 * 			Initialize peripherals and hand over control to
 * 			the game.
 */
int main()
{

	/* Initialize all peripherals */
	SysClock_Init();
	Uart_Init();
	I2C_Init();
	Accel_Init();
	TouchConfig_Init();
	Ticktime_Init();

	/* Run uCUnit Test Suite */
	Testsuite_Run();

	/* Run the game */
	RunGame();

	/* Should never reach here */
	assert(0);
    return 0 ;
}
