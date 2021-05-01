/*
 * UART.c
 *
 *  Created on: Mar 28, 2021
 *      Author: Dhruv
 */

#include "MKL25Z4.h"
#include "UART.h"
#include "cbfifo.h"

/* Macros */

#define UART0_TX_PIN 1
#define UART0_RX_PIN 2

/* UART Parameters */
#define SYS_CLK		24000000
#define OVERSAMPL	16

/* Select the Baud Rate - 9600, 19200, 38400, 57600, 115200 */
#define BAUD_RATE	38400

/* Choose between 8 or 9 data bits */
#define DATA_SIZE	8

/* Choose to enable or disable parity. If enabled,
 * select from EVEN_PTY or ODD_PTY
 */
#define PARITY_EN	0
#define PARITY		EVEN_PTY
#define EVEN_PTY	0
#define ODD_PTY		1

/* Choose between 1 and 2 stop bits */
#define STOP_BITS	2

Buffer Cbfifo_RX = {.size = SIZE};
Buffer Cbfifo_TX = {.size = SIZE};

/* Function Definitions */

void uart_init()
{

	/* Variables to calculate and write Baud rate value */
	uint32_t SBR;
	uint8_t SBR_H, SBR_L;

	/* Clock Gating for UART0 and Setting Clock Source to MCGCFLL */
	SIM->SCGC4 |= SIM_SCGC4_UART0(1);
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);

	/* Setting two stop bits and Baud Rate
	 * SBR = MCGFLL/BR*16 = (24*10^6/(38400*16))
	 * SBR = 39.0625
	 */
	SBR = (SYS_CLK/(BAUD_RATE * OVERSAMPL));
	SBR_H = (SBR & 0x1F00) >> 8;
	SBR_L = (SBR & 0xFF);

	/* Setting Stop Bits and Baud Rate */
	if(STOP_BITS == 2)
	{
		UART0->BDH |= (UART0_BDH_SBNS(1) |  UART0_BDH_SBR(SBR_H));
	}
	else UART0->BDH |= UART0_BDH_SBR(SBR_H);

	UART0->BDL |= UART0_BDL_SBR(SBR_L);

	/* Setting Data Size */
	if(DATA_SIZE == 9)
	{
		UART0->C1 |= UART0_C1_M(1);
	}

	/* Setting Parity */
	if(PARITY_EN == 1)
	{
		UART0->C1 |=  (UART0_C1_PE(1) | UART0_C1_PT(PARITY));
	}

	/* Start Transmitter and Receiver */
	UART0->C2 |= (UART0_C2_TE(1) | UART0_C2_RE(1));

	/* Enable Clock Gating for UART0 Pins */
	SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK);

	/* Set Pin Mux Register for ALT 2 Port A 1 & 2 */
	PORTA->PCR[UART0_TX_PIN] &= ~(PORT_PCR_MUX_MASK);
	PORTA->PCR[UART0_RX_PIN] &= ~(PORT_PCR_MUX_MASK);
	PORTA->PCR[UART0_TX_PIN] |= PORT_PCR_MUX(2);
	PORTA->PCR[UART0_RX_PIN] |= PORT_PCR_MUX(2);

	NVIC_EnableIRQ(UART0_IRQn);
	/* Enable Interrupts for Transmit and Receive */
	UART0->C2 |= (UART0_C2_TIE(1) | UART0_C2_RIE(1));

	#ifndef DEBUG
	  Buffer Cbfifo_Test = {.wrloc = 0, .rdloc = 0, .size = SIZE};
	  test_cbfifo(&Cbfifo_Test);
	#endif

}


int __sys_write(int handle, char *buf, int size)
{
	/* Ignore the handle argument */
	if(handle == 1 || handle == 2)
	{
		/* Only enqueue if there is enough space, else wait */
		while((Cbfifo_TX.size - cbfifo_length(&Cbfifo_TX) - 1) < size);

		cbfifo_enqueue(&Cbfifo_TX, buf, size);
		UART0->C2 |= (UART0_C2_TIE_MASK);
		return 0;
	}
	return -1;
}

int __sys_readc(void)
{
	uint8_t UART_buf;

	/* Dequeue a byte if available, else return -1 */
	if(cbfifo_dequeue(&Cbfifo_RX, &UART_buf, 1))
	{
		return UART_buf;
	}
	return -1;
}

uint8_t ByteReceived()
{
	if(cbfifo_length(&Cbfifo_RX))
		return 1;

	return 0;
}


void UART0_IRQHandler()
{
	/* Byte to store data and error counter for debugging */
	uint8_t byte;
	static uint16_t errorctr = 0;

	if(UART0->S1 & UART0_S1_RDRF_MASK)
	{
		byte = UART0->D;

		/* Enqueue the received byte onto RX buffer
		 * Increment error count if buffer is full
		 */
		if(cbfifo_enqueue(&Cbfifo_RX, &byte, 1) < 1)
		errorctr++;

		/* Enqueue the received byte onto TX buffer (loopback)
		* Increment error count if buffer is full
		*/
		if(cbfifo_enqueue(&Cbfifo_TX, &byte, 1) < 1)
		errorctr++;
	}


	if(UART0->S1 & UART0_S1_TDRE_MASK)
	{
		/* Send a byte if available, else disable TX interrupts */
		if(cbfifo_dequeue(&Cbfifo_TX, &byte, 1))
		{
			UART0->D = byte;
		}
		else (UART0->C2 &= ~(UART0_C2_TIE_MASK));

	}
}

