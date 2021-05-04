/*
 * MMA8451.h
 *
 *  Created on: Apr 28, 2021
 *      Author: Dhruv
 */
#include <stdint.h>

#ifndef MMA8451_H_
#define MMA8451_H_

/* Typedefs */

typedef struct{
	int16_t roll;
	int16_t pitch;
}AngleData;

/* Function Prototypes */

/**
 * @name    Accel_Init.
 *
 * @brief   Initializes the MMA8451 I2C accelerometer
 * 			by setting it into Active Mode.
 * 			2g (default) full scale resolution used
 * 			14-bit data
 *
 * @param	void
 *
 * @return  void
 */
void Accel_Init();

/**
 * @name    read_full_xyz.
 *
 * @brief   Starts the I2C Receive transaction by
 * 			polling the accelerometer for X,Y,Z
 * 			data.
 *
 * @param	void
 *
 * @return  void
 */
void read_full_xyz();

/**
 * @name    convert_xyz_to_roll_pitch.
 *
 * @brief   Converts the obtained accelerometer
 * 			data from raw values to roll and
 * 			pitch angles and returns to the caller.
 * 			Must be used directly after calling
 * 			read_full_xyz.
 *
 * @param	void
 *
 * @return  AngleData -
 * 			AngleData.roll - roll angle
 * 			AngleData.pitch - pitch angle
 */
AngleData convert_xyz_to_roll_pitch();

#endif /* MMA8451_H_ */
