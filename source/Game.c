/*
 * Game.c
 *
 *  Created on: Apr 29, 2021
 *      Author: Dhruv
 *      Brief : Contains the game state machine.
 */
#include <stdio.h>
#include "MMA8451.h"
#include "Cmd_Processor.h"
#include "TSI_Functions.h"
#include "SysTick_Functions.h"

/* Macros and Tpye definitions */

/* Delay in 50ms chunks
 * 0 < x < 45
 */
#define DELAY_TIME(x) ((50 - x) * 20)

/* Flag to print results */
uint8_t resultflag = 0;

/* Game states */
typedef enum state
{
	STARTUP = 0,
	PLAYING = 1,
	RESULT = 2,
	CH_LEVEL = 3

}state;
state curstate = STARTUP;

/* Private function definitions */

/**
 * @name    Startup_State.
 *
 * @brief   Starts the first level of the game
 * 			and waits for user touch.
 *
 * @param	void
 *
 * @return  void
 *
 */
static void Startup_State()
{
	/* Start first level */
	ParseCommand("Level_1");

	/* Wait for user touch */
	while(!(Touch_Scan_LH()))
	{
		delay(30);
	}

	/* Game cursor */
	printf("*");
}

/**
 * @name    Check_and_Move.
 *
 * @brief   Checks the roll, pitch angles
 * 			and determines the speed of
 * 			movement and moves the cursor.
 *
 * @param	AngleData rpstruct - Roll/Pitch structure
 *
 * @return  void
 *
 */
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

	/* Checks if the cursor has reached the end */
	ParseCommand("Cursor");

	/* Change state to RESULT */
	if(resultflag == 1)
		curstate = RESULT;

	resultflag = 0;
}

/**
 * @name    Result_State.
 *
 * @brief   Prints the result and waits for
 * 			the user input to touch the sensor.
 *
 * @param	void
 *
 * @return  void
 *
 */

static void Result_State()
{
	/* Print the result */
	ParseCommand("Result");

	/* Wait for user touch to advance */
	while(!(Touch_Scan_LH()))
	{
		delay(30);
	}
}

/* Function Definitions */

void Set_Result()
{
	resultflag = 1;
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
