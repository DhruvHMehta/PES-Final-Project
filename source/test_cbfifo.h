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

/* @file    test_cbfifo.h
 *
 * @brief   Contains function prototypes and descriptions for testing 
 *          circular buffer FIFO functions.
 * 
 * @author  Dhruv Mehta
 * @date    02/07/2021
 * @version 1.0
 * 
 ************************************************************************/
#ifndef __TEST_CBFIFO_H__
#define __TEST_CBFIFO_H__

/* Function Prototypes */

/**
 * @name    test_cbfifo.
 * 
 * @brief   Tests the Circular Buffer FIFO through a range of test cases. 
 
 * @return  int If successful, 1
 *              If error, -1
 *  
 */
int test_cbfifo(Buffer* Cbfifo);

/**
 * @name    Test_Case_1.
 * 
 * @brief   Tests the Circular Buffer with nybtes = 48 as a normal case.
 *          Checks return values of enqueue, dequeue and length functions. 
 
 * @return  int If successful, 1
 *              If error, 0
 *  
 */
int Test_Case_1(Buffer* Cbfifo);

/**
 * @name    Test_Case_2.
 * 
 * @brief   Tests the Circular Buffer with nybtes > 127. Only 127 bytes should
 *          be written. It is then dequeued and return values are verified. 
 
 * @return  int If successful, 1
 *              If error, 0
 *  
 */
 int Test_Case_2(Buffer* Cbfifo);

/**
 * @name    Test_Case_3.
 * 
 * @brief   Tests the Circular Buffer for boundary conditions at nbyte = 126,
 *          and enqueuing beyond the boundary. Also check for dequeuing a
 *          byte and then trying to enqueue a byte.          
 * 
 * @return  int If successful, 1
 *              If error, 0
 *  
 */
 int Test_Case_3(Buffer* Cbfifo);

/**
 * @name    Test_Case_4.
 * 
 * @brief   Tests the Circular Buffer with single byte enqueue and dequque 
 *          operations.
 
 * @return  int If successful, 1
 *              If error, 0
 *  
 */
 int Test_Case_4(Buffer* Cbfifo);

/**
 * @name    Test_Case_5.
 * 
 * @brief   Tests the Circular Buffer for NULL values of enqueued data.
 
 * @return  int If successful, 1
 *              If error, 0
 *  
 */
 int Test_Case_5(Buffer* Cbfifo);

/**
 * @name    Test_Case_6.
 * 
 * @brief   Tests the Circular Buffer for wrap around and write cases. 
 
 * @return  int If successful, 1
 *              If error, 0
 *  
 */
 int Test_Case_6(Buffer* Cbfifo);

#endif
