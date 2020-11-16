#include "atoi.h"

int aHexToI(char * subject){
	int idx, digitValue, total = 0, multiplier = 0x1;
	BOOL isZeroToNine, isUcAToF, isLcAToF, isHexDigit;

	for(idx = strlen(subject) - 1; idx >= 0; idx--){
		isZeroToNine = ((subject[idx] >= '0') && (subject[idx] <= '9'));
		isUcAToF     = ((subject[idx] >= 'A') && (subject[idx] <= 'F'));
		isLcAToF     = ((subject[idx] >= 'a') && (subject[idx] <= 'f'));
		isHexDigit = (isZeroToNine || isUcAToF || isLcAToF);

		if(isHexDigit){
			if(isZeroToNine) digitValue = (subject[idx] - '0') +  0;
			if(isUcAToF)     digitValue = (subject[idx] - 'A') + 10;
			if(isLcAToF)     digitValue = (subject[idx] - 'a') + 10;
		}
		else{
			return(-1);
		}

		total += (multiplier * digitValue);
		multiplier *= 0x10;
	}

	return(total);
}

int aDecToI(char * subject){
	int idx, digitValue, total = 0, multiplier = 1;
	BOOL isZeroToNine;

	for(idx = strlen(subject) - 1; idx >= 0; idx--){
		isZeroToNine = ((subject[idx] >= '0') && (subject[idx] <= '9'));

		if(isZeroToNine){
			digitValue = (subject[idx] - '0');
		}
		else{
			return(-1);
		}

		total += (multiplier * digitValue);
		multiplier *= 10;
	}

	return(total);
}

int aBinToI(char * subject){
	int idx, digitValue, total = 0, multiplier = 1;
	BOOL isZeroToOne;

	for(idx = strlen(subject) - 1; idx >= 0; idx--){
		isZeroToOne = ((subject[idx] >= '0') && (subject[idx] <= '1'));

		if(isZeroToOne){
			digitValue = (subject[idx] - '0');
		}
		else{
			return(-1);
		}

		total += (multiplier * digitValue);
		multiplier *= 2;
	}

	return(total);
}

static char digitsBuffer[64], * itoaDigits = &(digitsBuffer[0]);

char * iToAHex(int subject){
	int digitIdx, idx, digitValue, divisor = 0x1, highestDivisor;
	BOOL isZeroToNine, isAToF;

	digitIdx = 0;
	if(subject < 0){
		strcpy(itoaDigits, "-");
		digitIdx++;
		subject = 0 - subject;
	}
	else if(subject == 0){
		strcpy(itoaDigits, "0");
		return(itoaDigits);
	}

	for(idx = 0; idx < (sizeof(digitsBuffer) - 1); idx++){
		if(divisor <= subject){
			highestDivisor = divisor;
		}
		else{
			break;
		}
		divisor *= 0x10;
	}
	
	for(divisor = highestDivisor; divisor >= 0x1; divisor /= 0x10){
		digitValue = (subject / divisor) % 0x10;
		isZeroToNine = ((digitValue >= 0x0) && (digitValue <= 0x9));
		isAToF       = ((digitValue >= 0xA) && (digitValue <= 0xF));

		if(isZeroToNine) itoaDigits[digitIdx] = '0' + (digitValue -  0);
		if(isAToF)       itoaDigits[digitIdx] = 'A' + (digitValue - 10);
		digitIdx++;
	}
	itoaDigits[digitIdx] = '\0';
	return(itoaDigits);
}

char * iToADec(int subject){
	int digitIdx, idx, digitValue, divisor = 1, highestDivisor;

	digitIdx = 0;
	if(subject < 0){
		strcpy(itoaDigits, "-");
		digitIdx++;
		subject = 0 - subject;
	}
	else if(subject == 0){
		strcpy(itoaDigits, "0");
		return(itoaDigits);
	}

	for(idx = 0; idx < (sizeof(digitsBuffer) - 1); idx++){
		if(divisor <= subject){
			highestDivisor = divisor;
		}
		else{
			break;
		}
		divisor *= 10;
	}
	
	for(divisor = highestDivisor; divisor >= 1; divisor /= 10){
		digitValue = (subject / divisor) % 10;
		itoaDigits[digitIdx] = '0' + digitValue;
		digitIdx++;
	}
	itoaDigits[digitIdx] = '\0';
	return(itoaDigits);
}

char * iToABin(int subject){
	int digitIdx, idx, digitValue, divisor = 1, highestDivisor;

	digitIdx = 0;
	if(subject < 0){
		strcpy(itoaDigits, "-");
		digitIdx++;
		subject = 0 - subject;
	}
	else if(subject == 0){
		strcpy(itoaDigits, "0");
		return(itoaDigits);
	}

	for(idx = 0; idx < (sizeof(digitsBuffer) - 1); idx++){
		if(divisor <= subject){
			highestDivisor = divisor;
		}
		else{
			break;
		}
		divisor *= 2;
	}
	
	for(divisor = highestDivisor; divisor >= 1; divisor /= 10){
		digitValue = (subject / divisor) % 2;
		itoaDigits[digitIdx] = '0' + digitValue;
		digitIdx++;
	}
	itoaDigits[digitIdx] = '\0';
	return(itoaDigits);
}
