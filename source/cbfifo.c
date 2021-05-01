/************************************************************************
 * Copyright (C)​ 2021 by Dhruv Mehta
 *
 * ​Redistribution, ​​modification ​​or ​​use ​​of ​​this ​​software ​​in​​source ​​or ​​binary
 * forms ​​is ​​permitted ​​as ​​long​​ as ​​the ​​files​​ maintain ​​this ​​copyright.
 *
 * Users ​​are​​ ​​permitted ​​to​​ modify ​​this ​​and ​​use​​ it ​​to​​ learn​​ about​​ the​​ field
 * ​​of ​​embedded​​ ​​software.
 *
 * ​Dhruv Mehta and the University of Colorado are not liable for any misuse
 *  of this material​​.
 *
 ************************************************************************/

/* @file    cbfifo.c
 *
 * @brief   Contains an implementation for a circular buffer FIFO.
 *
 * @tools   GCC compiler v8.1.0 / Visual Studio Code IDE
 * @author  Dhruv Mehta
 * @date    04/04/2021
 * @version 1.1
 *
 ************************************************************************/
#include <stdint.h>
#include "cbfifo.h"
#include "MKL25Z4.h"

static size_t cbfifo_freespace(Buffer* Cbfifo)
{
    return (Cbfifo->size - cbfifo_length(Cbfifo) - 1); /* Total size minus number of occ elements - 1 */
}

size_t cbfifo_enqueue(Buffer* Cbfifo, void *buf, size_t nbyte)
{
    if(buf == NULL)
    {
        return -1;                              /* Input buffer is NULL */
    }
    uint8_t enq_size = 0;
    uint8_t freespace = cbfifo_freespace(Cbfifo);
    uint32_t intr_state;

    if(nbyte > freespace)
    {
        nbyte = freespace;                      /* Cannot write more than available space */
    }
    if(nbyte == 0)
    {
        return 0;                               /* Returns 0 on no write operation */
    }

    for(uint8_t i = 0; i < nbyte; i++)
    {
    	/* Critical section */
    	intr_state = __get_PRIMASK();
    	__disable_irq();

    	Cbfifo->CirBuf[Cbfifo->wrloc] = *((uint8_t *)buf);        /* Write to buffer */
        (uint8_t *)buf++;
        enq_size++;
        Cbfifo->wrloc = (Cbfifo->wrloc + 1) & (Cbfifo->size - 1); /* Wrapped addition of wrloc */

        __set_PRIMASK(intr_state);
    }

    return enq_size;
}

size_t cbfifo_dequeue(Buffer* Cbfifo, void *buf, size_t nbyte)
{
    if(buf == NULL)
    {
        return -1;                                /* Input buffer is NULL */
    }
    uint8_t deq_size = 0;
    uint8_t length = cbfifo_length(Cbfifo);
    uint32_t intr_state;

    if(nbyte > length)
    {
        nbyte = length;                           /* Cannot fetch more than available length */
    }

    if(nbyte == 0)
    {
        return 0;                               /* Returns 0 on no read operation */
    }
    for(int i = 0; i < nbyte; i++)
    {
    	/* Critical section */
    	intr_state = __get_PRIMASK();
    	__disable_irq();

    	*((uint8_t *)buf) = Cbfifo->CirBuf[Cbfifo->rdloc];        /* Read from buffer */
        buf++;
        deq_size++;
        Cbfifo->rdloc = (Cbfifo->rdloc + 1) & (Cbfifo->size - 1);  /* Wrapped addition of rdloc */

        __set_PRIMASK(intr_state);
    }

    return deq_size;
}

size_t cbfifo_length(Buffer* Cbfifo)
{
    return ((Cbfifo->wrloc - Cbfifo->rdloc) & (Cbfifo->size - 1)); /* Wrapped subtraction of wrloc and rdloc */
}

size_t cbfifo_capacity()
{
    return SIZE-1;                                              /* Constant value, restricted to SIZE - 1 */
}
