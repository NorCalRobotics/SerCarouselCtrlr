#include "TimerInt.h"

extern volatile unsigned long timeval;

void tc0 (void) __attribute__ ((interrupt));    // Generate Interrupt

/* Setup the Timer Counter 0 Interrupt */
/*  /1  =  10ms per int / 100  ints per sec
* 	/10 =   1ms per int / 1000 ints per sec
*  	/50 = 200us per int / 5000 ints per sec
*/

void init_timer (void) {
	unsigned long int curIntsEnabledMask;

    T0MR0 = 149999/50;  // 10mSec = 150.000-1 counts  1mSec when / 10  200us when /50
    T0MCR = 3;                                  // Interrupt and Reset on MR0
    T0TCR = 1;                                  // Timer0 Enable
    VICVectAddr2 = (unsigned long)tc0;          // set interrupt vector in 1
    VICVectCntl2 = 0x20 | 0x4;                  // use it for Timer 0 Interrupt

	curIntsEnabledMask = VICIntEnable;
	curIntsEnabledMask |= 0x10; // Bit 5 Enables Timer0 Interrupt
    VICIntEnable = curIntsEnabledMask;
}

void DisableTimer0Interrupts(){
	unsigned long int curIntsEnabledMask;
	curIntsEnabledMask = VICIntEnable;
	curIntsEnabledMask |= 0x10; // Bit 5 Enables Timer0 Interrupt
    VICIntEnable = curIntsEnabledMask;
}

void ReEnableTimer0Interrupts(){
	unsigned long int curIntsEnabledMask;
	curIntsEnabledMask = VICIntEnable;
	curIntsEnabledMask &= (~0x10); // Bit 5 Enables Timer0 Interrupt
    VICIntEnable = curIntsEnabledMask;
}

/* Timer Counter 0 Interrupt executes each 10ms @ 60 MHz CPU Clock */
void tc0 (void) {

//	IODIR0 |= 0x00000400;	// debug - bit 10 to output
//  IOSET0 =  0x00000400;

    timeval++;
    T0IR = 1;                                   // Clear interrupt flag

//	IOCLR0 =  0x00000400;	// debug - clr bit 10

    VICVectAddr = 0;                            // Acknowledge Interrupt
}

// sync with timer by returning on a time edge
// return current timer value
// if passed in max will overflow the timer then wait for timer overflow
//
unsigned char SyncWithTimer(unsigned char ucCheckMax)
{
	unsigned int uiTemp;

	if ((ucCheckMax + timeval) < timeval)
	 	while (timeval);  // would roll over - wait for timer to roll over

	uiTemp = timeval;
	while (uiTemp == timeval);	// wait for next time tick

	return timeval;			// return current timer count
}

// wait for a min of passed in micro secs
//
void TimerMicroSecWait(unsigned int uiWaitMicroSecs)
{
	unsigned long uiStartTicks, uiEndTicks;

	uiStartTicks = SyncWithTimer(uiWaitMicroSecs/MICRO_SEC_PER_TICK +10);

	// if less than min time per tick then we just wait 1 tick
	if (uiWaitMicroSecs <= MICRO_SEC_PER_TICK + 10)
		{
		while(timeval <= uiStartTicks+1);	// wait one tick
		return;
		}

	// figure out how many time ticks we need to wait
	uiEndTicks = uiWaitMicroSecs/MICRO_SEC_PER_TICK;
	uiEndTicks++;		// just in case was == 0
	uiEndTicks += uiStartTicks;
	while(timeval <= uiEndTicks);			// wait for time to expire
}

/*****
*
*  Init timer for later use
*/
void TimerMarkStart(unsigned long *Timer)
{
  	*Timer = timeval;				// remember current timer
}

/*****
*
* 	Return number of microsecs since TimerMarkStart was called
*   NOTE: max time of about 70 hours.....
*/
unsigned long ulTimerGetMicroSecs(unsigned long *Timer)
{
	unsigned int ulMicroSecs;

	if (timeval < *Timer)
		{	// timeval rolled over
			ulMicroSecs = 0xffffffff - *Timer + timeval;
		}
	else
		ulMicroSecs = timeval - *Timer;

	return ulMicroSecs*MICRO_SEC_PER_TICK;
}

void SleepMilliSeconds(unsigned long int numMilliSeconds){
	unsigned long int numMsWaited = 0;
	unsigned long int startMicroSecs;

	while(numMsWaited < numMilliSeconds){
		TimerMarkStart(&startMicroSecs);
		while(ulTimerGetMicroSecs(&startMicroSecs) < 1000){ }

		numMsWaited++;
	}
}
