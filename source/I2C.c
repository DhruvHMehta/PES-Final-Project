/*
 * I2C.c
 *
 *  Created on: Apr 27, 2021
 *      Author: Dhruv
 *      Brief : Contains I2C Drivers used for interface
 *      		with the MMA8451 accelerometer.
 *      Reference : https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_8/I2C-Demo/src/i2c.c
 */
#include <stdint.h>
#include "MKL25Z4.h"
#include "I2C.h"

int lock_detect=0;
int i2c_lock=0;


#define I2C_START	I2C0->C1 |= I2C_C1_MST_MASK;
#define I2C_STOP	I2C0->C1 &= ~I2C_C1_MST_MASK;
#define I2C_RSTA	I2C0->C1 |= I2C_C1_RSTA_MASK;

#define I2C_TX		I2C0->C1 |= I2C_C1_TX_MASK;
#define I2C_RX		I2C0->C1 &= ~I2C_C1_TX_MASK;

#define I2C_ACK		I2C0->C1 |= I2C_C1_TXAK_MASK;
#define I2C_NACK	I2C0->C1 &= ~I2C_C1_TXAK_MASK;

#define I2C_BUSY	while(I2C0->S & I2C_S_BUSY_MASK)
#define I2C_TCF		while(!(I2C0->S & I2C_S_BUSY_MASK))
#define I2C_WAIT	i2c_wait()

void I2C_Init()
{
	//clock i2c peripheral and port E
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	SIM->SCGC5 |= (SIM_SCGC5_PORTE_MASK);

	//set pins to I2C function
	PORTE->PCR[24] |= PORT_PCR_MUX(5);
	PORTE->PCR[25] |= PORT_PCR_MUX(5);

	//set to 400k baud
	//baud = bus freq/(scl_div+mul)
 	//~400k = 24M/(64); icr=0x12 sets scl_div to 64

 	I2C0->F = (I2C_F_ICR(0x11) | I2C_F_MULT(0));

	//enable i2c and set to master mode
	I2C0->C1 |= (I2C_C1_IICEN_MASK);

	// Select high drive mode
	I2C0->C2 |= (I2C_C2_HDRS_MASK);
}

void I2C_Start()
{
	I2C_TX;
	I2C_START;
}

void I2C_Write_Byte(uint8_t Addr, uint8_t RegAddr, uint8_t Data)
{
	/* Set Transmitter Mode, Send Start Bit */
	I2C_TX;
	I2C_START;

	/* Send Slave Address */
	I2C0->D = Addr;
	I2C_WAIT;

	/* Send Register Address */
	I2C0->D = RegAddr;
	I2C_WAIT;

	/* Send Data */
	I2C0->D = Data;
	I2C_WAIT;
	I2C_STOP;
}

uint8_t I2C_Read_Byte(uint8_t Addr, uint8_t RegAddr)
{
	uint8_t data;

	/* Set Transmitter Mode, Send Start Bit */
	I2C_TX;
	I2C_START;

	/* Send Slave Address */
	I2C0->D = Addr;
	I2C_WAIT;

	/* Send Register Address */
	I2C0->D = RegAddr;
	I2C_WAIT;

	/* Send Repeated Start and Read */
	I2C_RSTA;
	I2C0->D = (Addr | 0x01);
	I2C_WAIT;

	/* Set to receiver mode, only 1 byte */
	I2C_RX;
	I2C_NACK;

	/* Read dummy byte */
	data = I2C0->D;
	I2C_WAIT;

	/* Read byte and send stop bit*/
	I2C_STOP;
	data = I2C0->D;

	return data;
}

void I2C_Sequential_Read(uint8_t Addr, uint8_t RegAddr, uint8_t* buffer, uint8_t bytes)
{
	/* Set Transmitter Mode, Send Start Bit */
	I2C_TX;
	I2C_START;

	/* Send Slave Address */
	I2C0->D = Addr;
	I2C_WAIT;

	/* Send Register Address */
	I2C0->D = RegAddr;
	I2C_WAIT;

	/* Send Repeated Start and Read */
	I2C_RSTA;
	I2C0->D = (Addr | 0x01);
	I2C_WAIT;

	/* Set to receiver mode */
	I2C_RX;

	while(bytes > 2)
	{
		/* Acknowledge until the last byte */
		I2C_ACK;

		/* Read dummy byte */
		*buffer = I2C0->D;
		I2C_WAIT;

		/* Read byte*/
		*buffer++ = I2C0->D;

		bytes--;

		for(uint32_t i = 0; i < 100000; i++);
	}

	/* NACK the last byte */
	I2C_NACK;

	/* Read dummy byte */
	*buffer = I2C0->D;
	I2C_WAIT;

	/* Read byte and send stop bit*/
	I2C_STOP;
	*buffer++ = I2C0->D;

}

void i2c_busy(void){
	// Start Signal
	lock_detect=0;
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	I2C_START;
	I2C_START;
	I2C0->C1 |=  I2C_C1_IICEN_MASK;
	// Write to clear line
	I2C0->C1 |= I2C_C1_MST_MASK; /* set MASTER mode */
	I2C0->C1 |= I2C_C1_TX_MASK; /* Set transmit (TX) mode */
	I2C0->D = 0xFF;
	while ((I2C0->S & I2C_S_IICIF_MASK) == 0U) {
	} /* wait interrupt */
	I2C0->S |= I2C_S_IICIF_MASK; /* clear interrupt bit */


							/* Clear arbitration error flag*/
	I2C0->S |= I2C_S_ARBL_MASK;


							/* Send start */
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	I2C0->C1 |= I2C_C1_TX_MASK; /* Set transmit (TX) mode */
	I2C0->C1 |= I2C_C1_MST_MASK; /* START signal generated */

	I2C0->C1 |= I2C_C1_IICEN_MASK;
							/*Wait until start is send*/

							/* Send stop */
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	I2C0->C1 |= I2C_C1_MST_MASK;
	I2C0->C1 &= ~I2C_C1_MST_MASK; /* set SLAVE mode */
	I2C0->C1 &= ~I2C_C1_TX_MASK; /* Set Rx */
	I2C0->C1 |= I2C_C1_IICEN_MASK;


								/* wait */
							/* Clear arbitration error & interrupt flag*/
	I2C0->S |= I2C_S_IICIF_MASK;
	I2C0->S |= I2C_S_ARBL_MASK;
	lock_detect=0;
	i2c_lock=1;
}

void i2c_wait(void) {
	lock_detect = 0;
	while(((I2C0->S & I2C_S_IICIF_MASK)==0) & (lock_detect < 200)) {
		lock_detect++;
	}
	if (lock_detect >= 200)
		i2c_busy();
	I2C0->S |= I2C_S_IICIF_MASK;
}
