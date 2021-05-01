/*
 * MMA8451.h
 *
 *  Created on: Apr 28, 2021
 *      Author: Dhruv
 */
#include <stdint.h>

#ifndef MMA8451_H_
#define MMA8451_H_

#define SLAVE_ADDR		0x1D
#define MMA_ADDR		(SLAVE_ADDR << 1)
#define REG_CTRL1 		0x2A
#define WHOAMI			0x0D
#define XYZ_DATA_CFG	0x0E

#define REG_OUT_X_MSB	0x01
#define REG_OUT_X_LSB	0x02
#define REG_OUT_Y_MSB	0x03
#define REG_OUT_Y_LSB	0x04
#define REG_OUT_Z_MSB	0x05
#define REG_OUT_Z_LSB	0x06

typedef struct{
	int16_t roll;
	int16_t pitch;
}AngleData;


void Accel_Init();
void read_full_xyz();
AngleData convert_xyz_to_roll_pitch();

#endif /* MMA8451_H_ */
