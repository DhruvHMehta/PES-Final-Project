/*
 * Game.c
 *
 *  Created on: Apr 29, 2021
 *      Author: Dhruv
 */
#include "MMA8451.h"
#include "Cmd_Processor.h"
#include "TSI_Functions.h"

#define DELAY_TIME(x) ((50 - x) * 10)

uint8_t resultflag = 0;

typedef enum state
{
	STARTUP = 0,
	PLAYING = 1,
	RESULT = 2,
	CH_LEVEL = 3

}state;

state curstate = STARTUP;

void Delay(long int millis)
{
	/* TIME_FACTOR = multiplication factor for 1ms delay */

	millis = 5300 * millis;

	while(millis--)
	{
    	__asm volatile ("nop");
	}
}

void Set_Result()
{
	resultflag = 1;
}

void Check_and_Move(AngleData rpstruct)
{
	if(rpstruct.roll > 10)
	{
		ParseCommand("Left\0");
		Delay(DELAY_TIME(rpstruct.roll));
	}


	if(rpstruct.roll < -10)
	{
		ParseCommand("Right\0");
		Delay(DELAY_TIME(rpstruct.roll*-1));
	}


	if(rpstruct.pitch < -10)
	{
		ParseCommand("Up\0");
		Delay(DELAY_TIME(rpstruct.pitch*-1));
	}


	if(rpstruct.pitch > 10)
	{
		ParseCommand("Down\0");
		Delay(DELAY_TIME(rpstruct.pitch));
	}

	ParseCommand("Cursor\0");

	if(resultflag == 1)
		curstate = RESULT;

	resultflag = 0;
}

void RunGame()
{
	AngleData rpstruct;

	while(1)
	{
		read_full_xyz();
		rpstruct = convert_xyz_to_roll_pitch();

		switch(curstate)
		{
		case STARTUP:
			ParseCommand("Test\0");

			while(!(Touch_Scan_LH()))
			{
				Delay(30);
			}

			printf("*");
			curstate = PLAYING;
			break;


		case PLAYING:
			Check_and_Move(rpstruct);
			break;

		case RESULT:
			ParseCommand("Result\0");

			while(!(Touch_Scan_LH()))
			{
				Delay(30);
			}

			curstate = CH_LEVEL;
			break;

		case CH_LEVEL:
			ParseCommand("Level_2\0");
			curstate = PLAYING;
			break;

		}

	}

}
