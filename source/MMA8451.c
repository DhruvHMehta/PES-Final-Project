/*
 * MMA8451.c
 *
 *  Created on: Apr 28, 2021
 *      Author: Dhruv
 */
#include <math.h>
#include "MMA8451.h"
#include "I2C.h"

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

#define COUNTS_PER_G (4096.0)
#define M_PI (3.14159265)

int16_t acc_X, acc_Y, acc_Z;

void Accel_Init()
{
	I2C_Write_Byte(MMA_ADDR, REG_CTRL1, 0x01);
}

void read_full_xyz()
{
	int i;
	uint8_t data[6];
	int16_t temp[3];

	I2C_Start();

	//I2C_Sequential_Read(MMA_ADDR, REG_X1, data, 6);
	data[0] = I2C_Read_Byte(MMA_ADDR, REG_OUT_X_MSB);
	data[1] = I2C_Read_Byte(MMA_ADDR, REG_OUT_X_LSB);
	data[2] = I2C_Read_Byte(MMA_ADDR, REG_OUT_Y_MSB);
	data[3] = I2C_Read_Byte(MMA_ADDR, REG_OUT_Y_LSB);
	data[4] = I2C_Read_Byte(MMA_ADDR, REG_OUT_Z_MSB);
	data[5] = I2C_Read_Byte(MMA_ADDR, REG_OUT_Z_LSB);


	for (i = 0; i < 3; i++)
	{
		temp[i] = 0;
		temp[i] = (int16_t) ((data[2*i]<<8) | (data[2*i + 1]));
	}

	// Align for 14 bits
	acc_X = temp[0]/4;
	acc_Y = temp[1]/4;
	acc_Z = temp[2]/4;

}

AngleData convert_xyz_to_roll_pitch()
{
	AngleData rpstruct;

	float 		ax = acc_X/COUNTS_PER_G,
				ay = acc_Y/COUNTS_PER_G,
				az = acc_Z/COUNTS_PER_G;

	rpstruct.roll = atan2(ay, az)*180/M_PI;
	rpstruct.pitch = atan2(ax, sqrt(ay*ay + az*az))*180/M_PI;

	if(rpstruct.roll > 45)
		rpstruct.roll = 45;

	if(rpstruct.pitch > 45)
		rpstruct.pitch = 45;

	if(rpstruct.roll < -45)
		rpstruct.roll = -45;

	if(rpstruct.pitch < -45)
		rpstruct.pitch = -45;

	return rpstruct;

}


