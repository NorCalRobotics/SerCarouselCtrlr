#ifndef STRING_H_INCLUDED
 #define STRING_H_INCLUDED

 void * memset(void * memArea, int memValue, unsigned int memSize);
 void * memcpy(void * destMemArea, void * srcMemArea, unsigned int memSize);
 char * strcpy(char * destStr, const char * srcStr);
 unsigned int strlen(const char * subject);
 int strcmp(const char * subj1, const char * subj2);
 int strncmp(const char * subj1, const char * subj2, unsigned int len);
 char * strcat(char * subject, const char * concatStr);

#endif

