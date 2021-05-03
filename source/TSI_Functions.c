/*
 * TSI_Functions.c
 *
 *  Created on: Feb 20, 2021
 *      Author: Dhruv Mehta
 *      Brief : Contains Implementations for Touch Sensor Interfacing.
 */
#include "TSI_Functions.h"

/* MACROS */

/* Macros for Touch  - Referenced from Embedded Systems Fundamentals by Alexander G Dean*/

#define BUSY_WAIT 	1 					// implements the code in busy-wait loop
										//variation of the capacitance from 90 to 700
#define TOUCH_OFFSET 550  				// offset value to be subtracted
#define TOUCH_DATA (TSI0->DATA & 0xFFFF)//macro for extracting the count from data register

/* Macros for Location of Touch */
#define MIN_LEFT	100

void Init_TouchConfig()
{
	/* Enabling the clock */
	SIM->SCGC5 |= SIM_SCGC5_TSI_MASK;

	/* Set the Touch Sensor Interface General Control and Status Register */

	TSI0->GENCS = TSI_GENCS_MODE(0u) | //operating in non-noise mode
	TSI_GENCS_REFCHRG(0u) | 		   //reference oscillator charge and discharge value 500nA
	TSI_GENCS_DVOLT(0u)   | 		   //oscillator voltage rails set to default
	TSI_GENCS_EXTCHRG(0u) | 		   //electrode oscillator charge and discharge value 500nA
	TSI_GENCS_PS(0u)      |  		   // frequency clock divided by one
	TSI_GENCS_NSCN(31u)   | 		   //scanning the electrode 32 times
	TSI_GENCS_TSIEN_MASK  | 		   //enabling the TSI module
	TSI_GENCS_EOSF_MASK; 			   // writing one to clear the end of scan flag

}


int Touch_Scan_LH()
{
	/* Stores touch sensor raw input */
	unsigned int scan = 0;

	/* Select channel 10 and Start the scan */
	TSI0->DATA  = TSI_DATA_TSICH(10u);
	TSI0->DATA |= TSI_DATA_SWTS_MASK;

	/* Waiting for the scan to complete 32 times */
	while (!(TSI0->GENCS & TSI_GENCS_EOSF_MASK));

	/* Acquiring data in scan and writing one to clear the end of scan flag */
	scan = TOUCH_DATA;
	TSI0->GENCS |= TSI_GENCS_EOSF_MASK;

	scan = scan - TOUCH_OFFSET;

	if(scan >= MIN_LEFT)
	{
		return scan;
	}

	return 0;
}
