/*****
*
* (c) Copyright 2006 R-Quest Technologies, LLC.  
* All rights reserved.
*/

#include "TimerNoInt.h"

void TimerInit (void) 
{
 	T1TCR = 0; // Disable Timer1 Until Ready
	T1MCR |= 0x0004; // Setting bit 3 of T1's MCR means to Stop when MR0=0
}

void WaitMicroSeconds(unsigned int MicroSeconds)
{
	// set timer count
	T1MR0 = MicroSeconds * COUNTS_PER_MICRO_SEC;
	T1TC = 0;
	T1TCR = 1;              // Timer1 Enable

	while (T1TCR & 0x01);	// while timer is running
}
