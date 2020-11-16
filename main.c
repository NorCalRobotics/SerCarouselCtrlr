#include "Stepper.h"
#include "Sensors.h"
#include "main.h"
#include "pcProtocol.h"
#include <LPC.h>

volatile long timeval;

const long int max_uSecsToSleep = 50000;
const long int accel_uSecs      = 10000;
const long int min_uSecsToSleep =  5000;

char respBuffer[MAX_CMD_LEN], * resp = &(respBuffer[0]);
int stepIdx = 0;

void sleepUSecs(long int uSecsToSleep){
	const int scaler = 1, uSecsPerLoop = 1000;
	int scaler_idx, uSecs;

	for(uSecs = 0; uSecs < uSecsToSleep; uSecs += uSecsPerLoop){
		for(scaler_idx = 0; scaler_idx < scaler; scaler_idx++){
			SleepMilliSeconds(1);
		}
	}
}

int step_backward(int stepIdx, int stepCount){
	stepIdx --;
	if(stepIdx < 0){
		return(stepCount - 1);
	}
	return(stepIdx);
}

int step_forward(int stepIdx, int stepCount){
	stepIdx ++;
	if(stepIdx >= stepCount){
		return(0);
	}
	return(stepIdx);
}

long int faster(long int us, const long int min_us){
	us -= accel_uSecs;
	if(us < min_us) return(min_us);
	return(us);
}

long int slower(long int us, const long int max_us){
	us += accel_uSecs;
	if(us > max_us) return(max_us);
	return(us);
}

void carousel_home(){
	long int uSecsToSleep;
	int stepCount = STEPS_PER_CYCLE;

	uSecsToSleep = max_uSecsToSleep;
	if(isSensorBlocked()){
		// Carousel Start position is near HOME.
		// Leaving Carousel HOME position...
		while(isSensorBlocked()){
			SetStep(stepIdx);
			stepIdx = step_forward(stepIdx, stepCount);
			sleepUSecs(uSecsToSleep);
		}
	}
	else{
		// Carousel Start position is unknown.
		// Finding Carousel HOME position...
		while(!isSensorBlocked()){
			SetStep(stepIdx);
			stepIdx = step_backward(stepIdx, stepCount);
			sleepUSecs(uSecsToSleep);
			uSecsToSleep = faster(uSecsToSleep, min_uSecsToSleep);
		}

		// Decelerating Carousel...
		while(uSecsToSleep < max_uSecsToSleep){
			SetStep(stepIdx);
			stepIdx = step_backward(stepIdx, stepCount);
			sleepUSecs(uSecsToSleep);
			uSecsToSleep = slower(uSecsToSleep, max_uSecsToSleep);
		}

		// Re-Aquiring Carousel HOME position...
		while(!isSensorBlocked()){
			SetStep(stepIdx);
			stepIdx = step_forward(stepIdx, stepCount);
			sleepUSecs(uSecsToSleep);
		}

		// Leaving Carousel HOME position...
		while(isSensorBlocked()){
			SetStep(stepIdx);
			stepIdx = step_forward(stepIdx, stepCount);
			sleepUSecs(uSecsToSleep);
		}
	}

	// Finding Final Carousel HOME position...
	while(!isSensorBlocked()){
		SetStep(stepIdx);
		stepIdx = step_backward(stepIdx, stepCount);
		sleepUSecs(uSecsToSleep);
	}

	// Carousel HOMED.
	while(isSensorBlocked()){
		SetStep(stepIdx);
		stepIdx = step_forward(stepIdx, stepCount);
		sleepUSecs(uSecsToSleep);
	}
}

void carousel_move_cw(int stepCount){
	int idx;

	for(idx = 0; idx < stepCount; idx++){
		stepIdx = step_forward(stepIdx, STEPS_PER_CYCLE);
		SetStep(stepIdx);
		sleepUSecs(max_uSecsToSleep);
	}
}

void carousel_move_ccw(int stepCount){
	int idx;

	for(idx = 0; idx < stepCount; idx++){
		stepIdx = step_backward(stepIdx, STEPS_PER_CYCLE);
		SetStep(stepIdx);
		sleepUSecs(max_uSecsToSleep);
	}
}

char * executePcCommand(char * command){
	char * arg;
	int stepCount;

	if(strcmp(command, "H") == 0){
		carousel_home();
		strcpy(resp, "X");
		return(resp);
        }

	if(strncmp(command, "M-", 2) == 0){
		arg = command + 2;
		stepCount = aHexToI(arg);
		carousel_move_cw(stepCount);
		strcpy(resp, "X");
		return(resp);
        }

        if(strncmp(command, "M", 1) == 0){
		arg = command + 1;
		stepCount = aHexToI(arg);
		carousel_move_ccw(stepCount);
		strcpy(resp, "X");
		return(resp);
	}

	if(strcmp(command, "S") == 0){
		if(isSensorBlocked()){
			strcpy(resp, "B");
		}
		else{
			strcpy(resp, "C");
		}
		return(resp);
	}

	strcpy(resp, "?");
	return(resp);
}

int main(){
	int forever = 1;

  	IODIR1 = 0xFF0000;          // P1.16..23 defined as Outputs
	InitSensors();
	InitStepper();
	initPcProtocol(&executePcCommand);

  	init_timer();
	TimerInit();

	carousel_home();

	// Carousel READY.

	while(forever){
		pollPcProtocol();
	}
}
