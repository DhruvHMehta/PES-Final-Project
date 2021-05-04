/*
 * I2C.h
 *
 *  Created on: Apr 27, 2021
 *      Author: Dhruv
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>


/* Function Prototypes */

/**
 * @name    I2C_Init.
 *
 * @brief   Initializes the I2C0 peripheral with settings
 * 			for the MMA8451 accelerometer.
 * 			Fast mode is selected.
 *
 * @param	void
 *
 * @return  void
 */
void I2C_Init();

/**
 * @name    I2C_Start.
 *
 * @brief   Sets the I2C0 peripheral in transmitter mode
 * 			and sends the start bit.
 *
 * @param	void
 *
 * @return  void
 */
void I2C_Start();

/**
 * @name    I2C_Write_Byte.
 *
 * @brief   Writes a single byte of data to the I2C peripheral
 * 			at the requested Register Address.
 *
 * @param	uint8_t Addr - I2C Slave Address
 * 			uint8_t RegAddr - I2C Register Address
 * 			uint8_t Data - Byte of data to write.
 *
 * @return  void
 */
void I2C_Write_Byte(uint8_t Addr, uint8_t RegAddr, uint8_t Data);

/**
 * @name    I2C_Read_Byte.
 *
 * @brief   Reads a single byte of data from the I2C peripheral
 * 			at the requested address and returns it.
 *
 * @param	uint8_t Addr - I2C Slave Address
 * 			uint8_t RegAddr - I2C Register Address
 *
 * @return  uint8_t Byte read
 */
uint8_t I2C_Read_Byte(uint8_t Addr, uint8_t RegAddr);

/**
 * @name    I2C_Sequential_Read.
 *
 * @brief   Reads a sequence of bytes from the I2C peripheral
 * 			starting at the RegAddr. Device internally increments
 * 			the read addresses.
 *
 * @param	uint8_t Addr - I2C Slave Address
 * 			uint8_t RegAddr - I2C Register Address
 * 			uint8_t* buffer - Buffer to write the data to
 * 			uint8_t bytes - Number of bytes to read.
 *
 * @return  void
 */
void I2C_Sequential_Read(uint8_t Addr, uint8_t RegAddr, uint8_t* buffer, uint8_t bytes);

/**
 * @name    i2c_wait.
 *
 * @brief   Writes a single byte of data to the I2C peripheral
 * 			at the requested Register Address.
 *
 * @param	uint8_t Addr - I2C Slave Address
 * 			uint8_t RegAddr - I2C Register Address
 * 			uint8_t Data - Byte of data to write.
 *
 * @return  void
 */

#endif /* I2C_H_ */
