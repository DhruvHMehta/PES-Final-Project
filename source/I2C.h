/*
 * I2C.h
 *
 *  Created on: Apr 27, 2021
 *      Author: Dhruv
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

void I2C_Init();
void I2C_Start();
void I2C_Write_Byte(uint8_t Addr, uint8_t RegAddr, uint8_t Data);
uint8_t I2C_Read_Byte(uint8_t Addr, uint8_t RegAddr);
void I2C_Sequential_Read(uint8_t Addr, uint8_t RegAddr, uint8_t* buffer, uint8_t bytes);
void i2c_wait();
void i2c_busy();
void i2c_read_setup(uint8_t dev, uint8_t address);
uint8_t i2c_repeated_read(uint8_t isLastRead);

#endif /* I2C_H_ */
