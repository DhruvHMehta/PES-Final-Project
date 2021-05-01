/*
 * Cmd_Processor.h
 *
 *  Created on: Mar 29, 2021
 *      Author: Dhruv
 */

#ifndef CMD_PROCESSOR_H_
#define CMD_PROCESSOR_H_

#include "UART.h"
#include <stdint.h>

/* Function Prototypes */

/**
 * @name    LineAccumalator.
 *
 * @brief   Accumulates the user input into a buffer until
 * 			a carriage return is received. Handles backspace.
 *
 * @param	void
 *
 * @return  void
 *
 */
void LineAccumalator();
void ParseCommand(char* input);

#endif /* CMD_PROCESSOR_H_ */
