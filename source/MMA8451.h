/*
 * MMA8451.h
 *
 *  Created on: Apr 28, 2021
 *      Author: Dhruv
 */
#include <stdint.h>

#ifndef MMA8451_H_
#define MMA8451_H_


typedef struct{
	int16_t roll;
	int16_t pitch;
}AngleData;


void Accel_Init();
void read_full_xyz();
AngleData convert_xyz_to_roll_pitch();

#endif /* MMA8451_H_ */
