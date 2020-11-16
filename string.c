#include "string.h"

void * memset(void * memArea, int memValue, unsigned int memSize){
	unsigned int idx;
	unsigned char * dest = (unsigned char *)memArea;

	for(idx = 0; idx < memSize; idx++){
		dest[idx] = (unsigned char)memValue;
	}

	return(memArea);
}

void * memcpy(void * destMemArea, void * srcMemArea, unsigned int memSize){
	unsigned int idx;
	unsigned char * dest = (unsigned char *)destMemArea;
	unsigned char * src  = (unsigned char *)srcMemArea;

	for(idx = 0; idx < memSize; idx++){
		dest[idx] = src[idx];
	}

	return(destMemArea);
}

char * strcpy(char * destStr, const char * srcStr){
	unsigned int idx;
	for(idx = 0; srcStr[idx] != '\0'; idx++){
		destStr[idx] = srcStr[idx];
	}
	destStr[idx] = '\0';

	return(destStr);
}

unsigned int strlen(const char * subject){
	unsigned int idx = 0;
	while(subject[idx] != '\0') idx++;
	return(idx);
}

int strcmp(const char * subj1, const char * subj2){
	unsigned int l1 = strlen(subj1), l2 = strlen(subj2);
	if(l1 > l2) return( 1);
	if(l1 < l2) return(-1);

	unsigned int idx;
	for(idx = 0; idx < l1; idx++){
		if(subj1[idx] > subj2[idx]) return( 1);
		if(subj1[idx] < subj2[idx]) return(-1);
	}

	return(0);
}

int strncmp(const char * subj1, const char * subj2, unsigned int len){
	unsigned int l1 = strlen(subj1), l2 = strlen(subj2);
	if(len > l2) if(l1 > l2) return( 1);
	if(len > l1) if(l1 < l2) return(-1);

	unsigned int idx;
	for(idx = 0; (idx < len) && (idx < l1); idx++){
		if(subj1[idx] > subj2[idx]) return( 1);
		if(subj1[idx] < subj2[idx]) return(-1);
	}

	return(0);
}

char * strcat(char * subject, const char * concatStr){
	unsigned int idx, l = strlen(subject);
	for(idx = 0; concatStr[idx] != '\0'; idx++){
		subject[idx + l] = concatStr[idx];
	}
	subject[idx + l] = '\0';

	return(subject);
}

