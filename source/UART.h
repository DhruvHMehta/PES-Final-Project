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
 * @name    uart_init.
 *
 * @brief   Initializes the UART0 peripheral with settings as
 * 			chosen in the UART Parameters macros.
 *
 * @param	void
 *
 * @return  void
 *
 */
void uart_init();

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

/**
 * @name    __sys_read.
 *
 * @brief   Redirects scanf, gets, getchar through the
 * 			UART peripheral.
 *
 * @param	none
 *
 * @return  int - 0/-1 (Success/Failure)
 *
 * @Note 	: Do not explicitly call this function.
 * 			  Use scanf/gets/getchar instead.
 *
 */
int __sys_readc(void);

/**
 * @name    ByteReceived.
 *
 * @brief   Interface to the command processor to
 * 			check whether a byte is received from
 * 			the user.
 *
 * @param	none
 *
 * @return  int - 1/0 (Byte received/ No data received)
 *
 *
 */
uint8_t ByteReceived();

#endif /* UART_H_ */
