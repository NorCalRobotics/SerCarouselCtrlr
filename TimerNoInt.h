/*****
*
* (c) Copyright 2006 R-Quest Technologies, LLC.  
* All rights reserved.
*/

#ifndef BLOCKING_TIMER_H_INCLUDED
 #define BLOCKING_TIMER_H_INCLUDED

 #include <LPC.h>
 #include "stdio.h"  
 #include "GlobalDefs.h"

 #define COUNTS_PER_MICRO_SEC 13 // PCLK = 12Mhz -> 83ns per count -> 12 counts per us

 void TimerInit (void); 
 void WaitMicroSeconds(unsigned int MicroSeconds);

#endif
