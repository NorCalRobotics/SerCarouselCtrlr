#include "LPC.h"                  
#include "Sensors.h"

#define HOPPER_SENSOR_1	0x00200000		// p0.21
#define HOPPER_SENSOR_2	0x00400000		// p0.22

int isSensorBlocked(){
	if((IOPIN0 & HOPPER_SENSOR_1) != 0){
		return(1);
	}
	else{
		return(0);
	}
}


void InitSensors(){
	IODIR0 &= ~(HOPPER_SENSOR_1 | HOPPER_SENSOR_2);
}

