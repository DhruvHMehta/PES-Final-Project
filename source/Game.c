/*
 * Game.c
 *
 *  Created on: Apr 29, 2021
 *      Author: Dhruv
 */
#include "MMA8451.h"
#include "Cmd_Processor.h"

void Delay(long int millis)
{
	/* TIME_FACTOR = multiplication factor for 1ms delay */

	millis = 5300 * millis;

	while(millis--)
	{
    	__asm volatile ("nop");
	}
}

void RunGame()
{
	AngleData rpstruct;
	ParseCommand("Test\0");
	printf("*");

	while(1)
	{
		read_full_xyz();
		rpstruct = convert_xyz_to_roll_pitch();
		LineAccumalator();

		if(rpstruct.roll > 40)
		{
			ParseCommand("Left\0");
			Delay(50);
		}
		else if(rpstruct.roll > 30)
		{
			ParseCommand("Left\0");
			Delay(100);
		}
		else if(rpstruct.roll > 20)
		{
			ParseCommand("Left\0");
			Delay(200);
		}
		else if(rpstruct.roll > 10)
		{
			ParseCommand("Left\0");
			Delay(350);
		}


		if(rpstruct.roll < -40)
		{
			ParseCommand("Right\0");
			Delay(50);
		}
		else if(rpstruct.roll < -30)
		{
			ParseCommand("Right\0");
			Delay(100);
		}
		else if(rpstruct.roll < -20)
		{
			ParseCommand("Right\0");
			Delay(200);
		}
		else if(rpstruct.roll < -10)
		{
			ParseCommand("Right\0");
			Delay(350);
		}


		if(rpstruct.pitch < -40)
		{
			ParseCommand("Up\0");
			Delay(50);
		}

		else if(rpstruct.pitch < -30)
		{
			ParseCommand("Up\0\0");
			Delay(100);
		}
		else if(rpstruct.pitch < -20)
		{
			ParseCommand("Up\0\0");
			Delay(200);
		}
		else if(rpstruct.pitch < -10)
		{
			ParseCommand("Up\0\0");
			Delay(350);
		}

		if(rpstruct.pitch > 40)
		{
			ParseCommand("Down\0");
			Delay(50);
		}

		else if(rpstruct.pitch > 30)
		{
			ParseCommand("Down\0");
			Delay(100);
		}
		else if(rpstruct.pitch > 20)
		{
			ParseCommand("Down\0");
			Delay(200);
		}
		else if(rpstruct.pitch > 10)
		{
			ParseCommand("Down\0");
			Delay(350);
		}
	}

}
