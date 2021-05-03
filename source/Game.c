/*
 * Game.c
 *
 *  Created on: Apr 29, 2021
 *      Author: Dhruv
 */
#include <stdio.h>
#include "MMA8451.h"
#include "Cmd_Processor.h"
#include "TSI_Functions.h"
#include "SysTick_Functions.h"

#define DELAY_TIME(x) ((50 - x) * 20)

uint8_t resultflag = 0;

typedef enum state
{
	STARTUP = 0,
	PLAYING = 1,
	RESULT = 2,
	CH_LEVEL = 3

}state;
state curstate = STARTUP;


void Set_Result()
{
	resultflag = 1;
}

static void Startup_State()
{
	ParseCommand("Level_1");

	while(!(Touch_Scan_LH()))
	{
		delay(30);
	}

	printf("*");
}

static void Check_and_Move(AngleData rpstruct)
{
	if(rpstruct.roll > 10)
	{
		ParseCommand("Left");
		delay(DELAY_TIME(rpstruct.roll));
	}


	if(rpstruct.roll < -10)
	{
		ParseCommand("Right");
		delay(DELAY_TIME(rpstruct.roll*-1));
	}


	if(rpstruct.pitch < -10)
	{
		ParseCommand("Up");
		delay(DELAY_TIME(rpstruct.pitch*-1));
	}


	if(rpstruct.pitch > 10)
	{
		ParseCommand("Down");
		delay(DELAY_TIME(rpstruct.pitch));
	}

	ParseCommand("Cursor");

	if(resultflag == 1)
		curstate = RESULT;

	resultflag = 0;
}

static void Result_State()
{
	ParseCommand("Result");

	while(!(Touch_Scan_LH()))
	{
		delay(30);
	}
}


void RunGame()
{
	/* Structure to obtain roll and pitch angles */
	AngleData rpstruct;
	reset_timer();

	while(1)
	{
		read_full_xyz();
		rpstruct = convert_xyz_to_roll_pitch();

		/* State Machine to cycle between :
		 * Startup - Game starts here, waits for user touch
		 * Playing - Game in progress
		 * Result - End reached, show completion message
		 * Change Level - Move to the next level
		 */
		switch(curstate)
		{
		case STARTUP:
			Startup_State();
			curstate = PLAYING;
			break;

		case PLAYING:
			Check_and_Move(rpstruct);
			break;

		case RESULT:
			Result_State();
			curstate = CH_LEVEL;
			break;

		case CH_LEVEL:
			ParseCommand("Level_2");
			curstate = PLAYING;
			break;

		}

	}

}
