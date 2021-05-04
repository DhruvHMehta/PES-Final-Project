/*
 * UART.h
 *
 *  Created on: Mar 28, 2021
 *      Author: Dhruv
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

/* Function Prototypes */

/**
 * @name    Uart_Init.
 *
 * @brief   Initializes the UART0 peripheral with settings as
 * 			chosen in the UART Parameters macros.
 *
 * @param	void
 *
 * @return  void
 *
 */
void Uart_Init();

/**
 * @name    __sys_write.
 *
 * @brief   Redirects printfs, puts, putchar through the
 * 			UART peripheral.
 *
 * @param	int handle - Unused
 * 			char *buf  - Buffer to write
 * 			int size   - Size to be written
 *
 * @return  int - 0/-1 (Success/Failure)
 *
 *  @Note 	: Do not explicitly call this function.
 * 			  Use printf/puts/putchar instead.
 *
 */
int __sys_write(int handle, char *buf, int size);


#endif /* UART_H_ */
