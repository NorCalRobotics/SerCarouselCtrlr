#include "LPC.h"                  
#include "Stepper.h"

#define ROT_A0  0x00001000		// P0.12
#define ROT_A1  0x00002000	  	// P0.13
#define ROT_AP  0x00008000		// P0.15
#define ROT_B0  0x00020000		// P0.17
#define ROT_B1  0x00080000		// P0.19
#define ROT_BP  0x00040000		// P0.18

#define ROT_OFF_SET ROT_A0 | ROT_A1 | ROT_AP | ROT_B0 | ROT_B1 | ROT_BP
#define ROT_OFF_CLR 0x00000000

#define ROT_STEP0_SET (ROT_A0 | ROT_A1 | ROT_AP | ROT_BP | ROT_B0)
#define ROT_STEP0_CLR (ROT_B1)

#define ROT_STEP2_SET (ROT_A0 | ROT_AP | ROT_B0 | ROT_B1)
#define ROT_STEP2_CLR (ROT_A1 | ROT_BP)

#define ROT_STEP4_SET (ROT_A0 | ROT_A1 | ROT_B0)
#define ROT_STEP4_CLR (ROT_AP | ROT_BP | ROT_B1)

#define ROT_STEP6_SET (ROT_A0 | ROT_BP | ROT_B0 | ROT_B1)
#define ROT_STEP6_CLR (ROT_A1 | ROT_AP)

#define ROT_STEP_TABLE_LEN 4

unsigned long int STEP_MASK_SET[] = {
	ROT_STEP0_SET,
	ROT_STEP2_SET,
	ROT_STEP4_SET,
	ROT_STEP6_SET};

unsigned long int STEP_MASK_CLR[] = {
	ROT_STEP0_CLR,
	ROT_STEP2_CLR,
	ROT_STEP4_CLR,
	ROT_STEP6_CLR};

void InitStepper(){
	IO0SET = ROT_OFF_SET;
	IO0DIR |= ROT_OFF_SET;
}

void SetStep(int step_idx){
	IOSET0 = STEP_MASK_SET[step_idx];
	IOCLR0 = STEP_MASK_CLR[step_idx];
}

