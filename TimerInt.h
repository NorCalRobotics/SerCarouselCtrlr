// TimerInt.h
#ifndef TIMER_INT_DRIVEN_H_INCLUDED
 #define TIMER_INT_DRIVEN_H_INCLUDED

 #include <LPC.h> // LPC21XX Peripheral Registers

 #define MICRO_SEC_PER_TICK 200

 extern void init_timer(void);
 unsigned char SyncWithTimer(unsigned char ucCheckMax);
 void TimerMicroSecWait(unsigned int uiWaitMicroSec);
 unsigned long ulTimerGetMicroSecs(unsigned long *Timer);
 void TimerMarkStart(unsigned long *Timer);

 void DisableTimer0Interrupts();
 void ReEnableTimer0Interrupts();

 void SleepMilliSeconds(unsigned long int numMilliSeconds);

#endif
