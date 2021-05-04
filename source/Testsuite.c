/*****************************************************************************
 *                                                                           *
 *  uCUnit - A unit testing framework for microcontrollers                   *
 *                                                                           *
 *  (C) 2007 - 2008 Sven Stefan Krauss                                       *
 *                  https://www.ucunit.org                                   *
 *                                                                           *
 *  File        : Testsuite.h                                                *
 *  Description : Sample testsuite                                           *
 *  Author      : Sven Stefan Krauss                                         *
 *  Contact     : www.ucunit.org                                             *
 *                                                                           *
 *****************************************************************************/

/*
 * This file is part of ucUnit.
 *
 * You can redistribute and/or modify it under the terms of the
 * Common Public License as published by IBM Corporation; either
 * version 1.0 of the License, or (at your option) any later version.
 *
 * uCUnit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Common Public License for more details.
 *
 * You should have received a copy of the Common Public License
 * along with uCUnit.
 *
 * It may also be available at the following URL:
 *       http://www.opensource.org/licenses/cpl1.0.txt
 *
 * If you cannot obtain a copy of the License, please contact the
 * author.
 */
#include "System.h"
#include "uCUnit-v1.0.h"
#include "Testsuite.h"
#include "cbfifo.h"
#include "test_cbfifo.h"
#include "Cmd_Processor.h"

#define ROW 0
#define COL 1

#ifdef DEBUG

/* Do not want these variables if not debugging */
extern uint8_t cursor_pos[];
Buffer Cbfifo_Test = {.wrloc = 0, .rdloc = 0, .size = SIZE};
#endif

static void Test_CmdProcessor()
{

    UCUNIT_TestcaseBegin("Testing Command Processor : All commands");

    UCUNIT_WriteString("Some prints may appear out of indentation. This is normal\n\r");

    UCUNIT_CheckIsEqual(8, cursor_pos[ROW]);
    UCUNIT_CheckIsEqual(0, cursor_pos[COL]);

    /* Normal movements */
    ParseCommand("Right");
    UCUNIT_CheckIsEqual(8, cursor_pos[ROW]);
    UCUNIT_CheckIsEqual(1, cursor_pos[COL]);

    ParseCommand("Down");
    UCUNIT_CheckIsEqual(9, cursor_pos[ROW]);
    UCUNIT_CheckIsEqual(1, cursor_pos[COL]);

    ParseCommand("Left");
    UCUNIT_CheckIsEqual(9, cursor_pos[ROW]);
    UCUNIT_CheckIsEqual(0, cursor_pos[COL]);

    ParseCommand("Up");
    UCUNIT_CheckIsEqual(8, cursor_pos[ROW]);
    UCUNIT_CheckIsEqual(0, cursor_pos[COL]);

    /* Edge movements */
    ParseCommand("Up");
    UCUNIT_CheckIsEqual(8, cursor_pos[ROW]);
    UCUNIT_CheckIsEqual(0, cursor_pos[COL]);

    ParseCommand("Left");
    UCUNIT_CheckIsEqual(8, cursor_pos[ROW]);
    UCUNIT_CheckIsEqual(0, cursor_pos[COL]);

    /* Out of bounds */
    for(int i = 0; i < 100; i++)
    	ParseCommand("Right");

    UCUNIT_CheckIsEqual(8, cursor_pos[ROW]);
    UCUNIT_CheckIsEqual(38, cursor_pos[COL]);

    for(int i = 0; i < 50; i++)
    	ParseCommand("Down");

    UCUNIT_CheckIsEqual(8, cursor_pos[ROW]);
    UCUNIT_CheckIsEqual(38, cursor_pos[COL]);

    /* Should reset cursor to initial position */
    ParseCommand("Result");
    UCUNIT_CheckIsEqual(8, cursor_pos[ROW]);
    UCUNIT_CheckIsEqual(0, cursor_pos[COL]);


    UCUNIT_TestcaseEnd();
}

static void Test_Pointers(void)
{
    char * s = NULL;

    UCUNIT_TestcaseBegin("Checking pointers");

    /* Checking for NULL parsing
     * Note: Since ParseCommand does not return,
     * this check is manual
     */
    ParseCommand(s);
    UCUNIT_WriteString("No output indicates success\n\r");
    UCUNIT_CheckIsNull(s);

    UCUNIT_TestcaseEnd();
}

static void Test_FIFO(void)
{
	uint8_t retval;

    UCUNIT_TestcaseBegin("Testing Circular Buffer FIFO - All test cases");

    /* Check if return value is 1, all cbfifo test cases have passed */
    retval = test_cbfifo(&Cbfifo_Test);
    UCUNIT_CheckIsEqual(1, retval);

    UCUNIT_TestcaseEnd();
}

static void Testsuite_RunTests(void)
{
	Test_FIFO();
	Test_CmdProcessor();
    Test_Pointers();
    UCUNIT_WriteSummary();
}

void Testsuite_Run(void)
{
#ifdef DEBUG

    UCUNIT_Init();
    UCUNIT_WriteString("\n\r**************************************");
    UCUNIT_WriteString("\n\rName:     ");
    UCUNIT_WriteString("uCUnit MazeTerminal application");
    UCUNIT_WriteString("\n\rCompiled: ");
    UCUNIT_WriteString(__DATE__);
    UCUNIT_WriteString("\n\rTime:     ");
    UCUNIT_WriteString(__TIME__);
    UCUNIT_WriteString("\n\rVersion:  ");
    UCUNIT_WriteString(UCUNIT_VERSION);
    UCUNIT_WriteString("\n\r**************************************");
    Testsuite_RunTests();
#endif
}
