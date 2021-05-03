/*
 * TSI_Functions.h
 *
 *  Created on: Feb 20, 2021
 *      Author: Dhruv
 */

#ifndef TSI_FUNCTIONS_H_
#define TSI_FUNCTIONS_H_


#include "MKL25Z4.h"


/* Function Prototypes for Touch  - Referenced from Embedded Systems Fundamentals by Alexander G Dean*/

/**
 * @name    Init_TouchConfig.
 *
 * @brief   Initializes the TSI Module SIM and General Control
 * 			and Status Registers.
 *
 * @param	void
 *
 * @return  void
 *
 */
void TouchConfig_Init();

/**
 * @name    Touch_Scan_LH.
 *
 * @brief   Selects the Touch Channel and Starts a Scan
 * 			for data, acquires it and then returns the
 * 			offset adjusted touch input (raw value).
 *
 * @param	void
 *
 * @return  int - Returns the offset adjust value of touch input.
 *
 */
int Touch_Scan_LH();

#endif /* TSI_FUNCTIONS_H_ */
