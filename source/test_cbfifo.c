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

/* @file    test_cbfifo.c
 *
 * @brief   Contains test functions to test the circular buffer fifo
 *          and all of its functions.
 * 
 * @tools   GCC compiler v8.1.0 / Visual Studio Code IDE
 * @author  Dhruv Mehta
 * @date    02/07/2021
 * @version 1.0
 * 
 ************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include "cbfifo.h"
#include "test_cbfifo.h"

int test_cbfifo(Buffer* Cbfifo);

/* Start of Test Cases */

 int Test_Case_1(Buffer* Cbfifo)
{
    char deq_buf[150]; /* Stores dequeued data */
    int retvalenq; /* Stores return value of enqueue to compare for correctness */
    int retvallen; /* Stores return value of length to compare for correctness */
    int retvaldeq; /* Stores return value of dequeue to compare for correctness */
    
    int expvalenq = 48;
    int expvallen = 48;
    int expvaldeq = 48;

    char *data1     = "Testing for a normal case with nbytes < capacity";
    size_t nbyte1   = 48; 

    retvalenq = cbfifo_enqueue(Cbfifo, data1, nbyte1);   /* Enqueues 48 bytes */
    retvallen = cbfifo_length(Cbfifo);                	 /* 48 */
    retvaldeq = cbfifo_dequeue(Cbfifo, deq_buf, nbyte1); /* Dequeues 48 bytes */

    cbfifo_enqueue(Cbfifo, "Enqueuing more data to fill out the buffer" \
                   "and return the pointers to the start.",80);
    cbfifo_dequeue(Cbfifo, deq_buf,80);

    /* Checking Results */

    if((retvalenq == expvalenq) && (retvaldeq == expvaldeq) && (retvallen == expvallen))
    {
        return 1;
    }
    else
    {
        if(retvalenq != expvalenq)
        {
            printf("TestCase 1 failed. Return value enqueue = %d\t, expected = %d\n\r",retvalenq, expvalenq);
        }
        if(retvaldeq != expvaldeq)
        {
            printf("TestCase 1 failed. Return value dequeue = %d\t, expected = %d\n\r",retvaldeq, expvaldeq);
        }
        if(retvallen != expvallen)
        {
            printf("TestCase 1 failed. Return value length = %d\t, expected = %d\n\r",retvallen, expvallen);
        }
    }
    return 0;
}

 int Test_Case_2(Buffer* Cbfifo)
{
    char deq_buf[260]; /* Stores dequeued data */
    int retvalenq; /* Stores return value of enqueue to compare for correctness */
    int retvallen; /* Stores return value of length to compare for correctness */
    int retvaldeq; /* Stores return value of dequeue to compare for correctness */
    
    int expvalenq = 255;
    int expvallen = 255;
    int expvaldeq = 255;

    char *data2     = "Who let the dogs out? Who Who Who Who?" \
    "Who let the dogs out? Who Who Who Who?" \
    "Who let the dogs out? Who Who Who Who?" \
    "Who let the dogs out? Who Who Who Who?" \
	"Who let the dogs out? Who Who Who Who?" \
	"Who let the dogs out? Who Who Who Who?" \
	"Who let the dogs out? Who Who Who Who?"; /* Data > capacity, only 128 bytes written */
    size_t nbyte2   = 266;                    /* Covers case if nbyte > capacity */

    retvalenq = cbfifo_enqueue(Cbfifo, data2, nbyte2);        /* Enqueues 127/152 bytes */
    retvallen = cbfifo_length(Cbfifo);                     	  /* 127 */
    retvaldeq = cbfifo_dequeue(Cbfifo, deq_buf, nbyte2);     /* Dequeues 127 bytes */

    /* Checking Results */

    if((retvalenq == expvalenq) && (retvaldeq == expvaldeq) && (retvallen == expvallen))
    {
        return 1;
    }
    else
    {
        if(retvalenq != expvalenq)
        {
            printf("TestCase 2 failed. Return value enqueue = %d\t, expected = %d\n\r",retvalenq, expvalenq);
        }
        if(retvaldeq != expvaldeq)
        {
            printf("TestCase 2 failed. Return value dequeue = %d\t, expected = %d\n\r",retvaldeq, expvaldeq);
        }
        if(retvallen != expvallen)
        {
            printf("TestCase 2 failed. Return value length = %d\t, expected = %d\n\r",retvallen, expvallen);
        }
    }
    return 0;
}

 int Test_Case_3(Buffer* Cbfifo)
{
    char deq_buf[150]; /* Stores dequeued data */
    int retvalenq[4]; /* Stores return value of enqueue to compare for correctness */
    int retvallen[5]; /* Stores return value of length to compare for correctness */
    int retvaldeq[2]; /* Stores return value of dequeue to compare for correctness */
    short TC3_e = 0;
    short TC3_l = 0;
    short TC3_d = 0;

    int expvalenq[4] = {126,1,1,1};
    int expvallen[5] = {126,127,128,127,128};
    int expvaldeq[2] = {1,127};

    char *data3_1   = "abcdefghijklmnopqrstuvwxyz_" \
    "abcdefghijklmnopqrstuvwxyz_" \
    "abcdefghijklmnopqrstuvwxyz_" \
    "abcdefghijklmnopqrstuvwxyz_" \
    "abcdefghijklmnopqr";                    /* Data on 1 less than boundary */
    size_t nbyte3_1 = 126;

    char *data3_2   = "t";                  /* Data on the boundary */
    size_t nbyte3_2 = 1;

    char *data3_3   = "u";                  /* Data should not be enqueued */
    size_t nbyte3_3 = 1;

    char *data3_4   = "v";                  /* Data enqueued at 0 after dequeue */
    size_t nbyte3_4 = 1;


    retvalenq[0] = cbfifo_enqueue(Cbfifo, data3_1,nbyte3_1);	/* Enqueues 126 bytes */
    retvallen[0] = cbfifo_length(Cbfifo);						/* 126 */

    retvalenq[1] = cbfifo_enqueue(Cbfifo, data3_2, nbyte3_2);   /* Enqueues 1 byte */
    retvallen[1] = cbfifo_length(Cbfifo);						/* 127 */

    retvalenq[2] = cbfifo_enqueue(Cbfifo, data3_3, nbyte3_3);   /* Return 0, no enqueue */
    retvallen[2] = cbfifo_length(Cbfifo);						/* 127 */

    retvaldeq[0] = cbfifo_dequeue(Cbfifo, deq_buf, 1);          /* Dequeues first byte */
    retvallen[3] = cbfifo_length(Cbfifo);						/* 126 */
    retvalenq[3] = cbfifo_enqueue(Cbfifo, data3_4, nbyte3_4);   /* Enqueues 1 byte at 0th location */
    retvallen[4] = cbfifo_length(Cbfifo);						/* 127 */
    retvaldeq[1] = cbfifo_dequeue(Cbfifo, deq_buf,127);         /* Dequeues all 127 bytes */

    cbfifo_enqueue(Cbfifo,"1", 1);                              /* Random string to reset pointers */
    cbfifo_dequeue(Cbfifo,deq_buf,1);


    /* Checking Results */

    for(int i = 0; i < (sizeof(retvalenq)/sizeof(int)); i++)
    {
        if(retvalenq[i] == expvalenq[i])
        {
            TC3_e++;
        }
        else printf("Test Case 3 Failed. Return value enqueue = %d\t, expected = %d\n\r",retvalenq[i], expvalenq[i]);
    }

    for(int i = 0; i < (sizeof(retvallen)/sizeof(int)); i++)
    {
        if(retvallen[i] == expvallen[i])
        {
            TC3_l++;
        }
        else printf("Test Case 3 Failed. Return value length = %d\t, expected = %d\n\r",retvallen[i], expvallen[i]);
    }

    for(int i = 0; i < (sizeof(retvaldeq)/sizeof(int)); i++)
    {
        if(retvaldeq[i] == expvaldeq[i])
        {
            TC3_d++;
        }
        else printf("Test Case 3 Failed. Return value dequeue = %d\t, expected = %d\n\r",retvaldeq[i], expvaldeq[i]);
    }
    if((TC3_e + TC3_l + TC3_d) == 11)
    {
        return 1;
    }
    return 0;

}

 int Test_Case_4(Buffer* Cbfifo)
{    
    char deq_buf[150]; /* Stores dequeued data */
    int retvalenq; /* Stores return value of enqueue to compare for correctness */
    int retvallen; /* Stores return value of length to compare for correctness */
    int retvaldeq; /* Stores return value of dequeue to compare for correctness */
    
    int expvalenq = 1;
    int expvallen = 1;
    int expvaldeq = 1;
    

    char *data4     = "Testing enqueuing and dequeing a single byte at a time." \
    "At every enqueue and corresponding dequeue, the read and write pointers move" \
    "alongside each other until the capacity of the buffer is reached and wrapped again.";
    size_t nbyte4   = 214;

    for(int i = 0; i < nbyte4-1; i++)
    {
        cbfifo_enqueue(Cbfifo, data4, 1);               /* Enqueuing 1 byte each loop till nbytes - 1 */
        cbfifo_dequeue(Cbfifo, deq_buf,1);              /* Dequeuing 1 byte each loop till nbytes -1 */
    }
    retvalenq = cbfifo_enqueue(Cbfifo, ".",1);       /* Enqueued last byte */
    retvaldeq = cbfifo_dequeue(Cbfifo, deq_buf,1);   /* Dequeued last byte */
    retvallen = cbfifo_length(Cbfifo);             /* Check length after these operations */

    /* Checking Results */

    if((retvalenq == expvalenq) && (retvaldeq == expvaldeq) && (retvallen == expvallen))
    {
        return 1;
    }
    else
    {
        if(retvalenq != expvalenq)
        {
            printf("TestCase 4 failed. Return value enqueue = %d\t, expected = %d\n\r",retvalenq, expvalenq);
        }
        if(retvaldeq != expvaldeq)
        {
            printf("TestCase 4 failed. Return value dequeue = %d\t, expected = %d\n\r",retvaldeq, expvaldeq);
        }
        if(retvallen != expvallen)
        {
            printf("TestCase 4 failed. Return value length = %d\t, expected = %d\n\r",retvallen, expvallen);
        }
    }
    return 0;

}

 int Test_Case_5(Buffer* Cbfifo)
{    
    int retvalenq; /* Stores return value of enqueue to compare for correctness */
    int retvallen[2]; /* Stores return value of length to compare for correctness */
    int retvaldeq; /* Stores return value of dequeue to compare for correctness */
    char *errorbuf = NULL;
    short TC3_l = 0;

    int expvalenq = -1;
    int expvallen[2] = {1,1};
    int expvaldeq = -1;
  
    retvalenq = cbfifo_enqueue(Cbfifo,errorbuf, 5);        		/* Test for NULL buffer, return -1 */
    retvallen[0] = cbfifo_length(Cbfifo);                    	/* Check length here = 0 */
    retvaldeq = cbfifo_dequeue(Cbfifo,errorbuf, 10);       		/* Test for NULL buffer, return -1 */
    retvallen[1] = cbfifo_length(Cbfifo);                    	/* Check length here = 0 */


    /* Checking Results */


    for(int i = 0; i < (sizeof(retvallen)/sizeof(int)); i++)
    {
        if(retvallen[i] == expvallen[i])
        {
            TC3_l++;
        }
        else printf("Test Case 5 Failed. Return value length = %d\t, expected = %d\n\r",retvallen[i], expvallen[i]);
    }

    if((retvalenq == expvalenq) && (retvaldeq == expvaldeq) && (TC3_l == 2))
    {
        return 1;
    }
    else
    {
        if(retvalenq != expvalenq)
        {
            printf("TestCase 5 failed. Return value enqueue = %d\t, expected = %d\n\r",retvalenq, expvalenq);
        }
        if(retvaldeq != expvaldeq)
        {
            printf("TestCase 5 failed. Return value dequeue = %d\t, expected = %d\n\r",retvaldeq, expvaldeq);
        }
    }
    return 0;

}

 int Test_Case_6(Buffer* Cbfifo)
{
    char deq_buf[150]; /* Stores dequeued data */
    int retvalenq[2]; /* Stores return value of enqueue to compare for correctness */
    int retvallen; /* Stores return value of length to compare for correctness */
    int retvaldeq; /* Stores return value of dequeue to compare for correctness */
    short TC3_e = 0;

    int expvalenq[2] = {130,14};
    int expvallen = 135;
    int expvaldeq = 10;

    char *data6_1     = "Who let the dogs out? Who Who Who Who?" \
    "Who let the dogs out? Who Who Who Who?" \
    "Who let the dogs out? Who Who Who Who?" \
    "Who let the dogs out? Who Who Who Who?"; /* Data > capacity, only 128 bytes written */
    size_t nbyte6_1   = 130;

    char *data6_2     = "ABCDEFGHIJK";       /* To make wr ptr wrap around behind rd ptr */
    size_t nbyte6_2   = 14;

    retvalenq[0] = cbfifo_enqueue(Cbfifo,data6_1, nbyte6_1);  /* Filling the cbfifo completely */
    retvaldeq = cbfifo_dequeue(Cbfifo,deq_buf, 10);           /* Removing 10 bytes */
    retvalenq[1] = cbfifo_enqueue(Cbfifo,data6_2, nbyte6_2);  /* Try to enqueue 14 bytes, only 10 should */
    retvallen = cbfifo_length(Cbfifo);                        /* Check length here  = 127 */


    /* Checking Results */

    for(int i = 0; i < (sizeof(retvalenq)/sizeof(int)); i++)
    {
        if(retvalenq[i] == expvalenq[i])
        {
            TC3_e++;
        }
        else printf("Test Case 6 Failed. Return value enqueue = %d\t, expected = %d\n\r",retvalenq[i], expvalenq[i]);
    }

    if((retvaldeq == expvaldeq) && (retvallen == expvallen) && (TC3_e == 2))
    {
        return 1;
    }
    else
    {
        if(retvaldeq != expvaldeq)
        {
            printf("TestCase 6 failed. Return value dequeue = %d\t, expected = %d\n\r",retvaldeq, expvaldeq);
        }
        if(retvallen != expvallen)
        {
            printf("TestCase 6 failed. Return value length = %d\t, expected = %d\n\r",retvallen, expvallen);
        }
    }
    return 0;
}

int test_cbfifo(Buffer* Cbfifo)
{
int Results[6];
int returnval = 0;

Results[0] = Test_Case_1(Cbfifo);
Results[1] = Test_Case_2(Cbfifo);
Results[2] = Test_Case_3(Cbfifo);
Results[3] = Test_Case_4(Cbfifo);
Results[4] = Test_Case_5(Cbfifo);
Results[5] = Test_Case_6(Cbfifo);

for(int i = 0; i < 6; i++)
{
   returnval += Results[i];
}
if(returnval == 6)
{
    printf("\n\rAll Test Cases for cbfifo passed\n\r");
    return 1;
}
else printf("\n\rTest Cases for cbfifo failed. Please refer to the above messages\n\r");
    return 0;
} 
