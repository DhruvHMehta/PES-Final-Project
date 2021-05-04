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
 * @name    Parse_Command.
 *
 * @brief   Tokenizes the input string and calls the
 * 			correct handler for the command.
 *
 * @param	char* input - Command string
 * 			For a list of commands, refer to the
 * 			command table (command_table_t commands[]).
 *
 * @return  void
 *
 */
void ParseCommand(char* input);

#endif /* CMD_PROCESSOR_H_ */
