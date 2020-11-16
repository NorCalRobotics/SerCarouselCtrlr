#ifndef GLOBAL_DEFS_H_INCLUDED
 #define GLOBAL_DEFS_H_INCLUDED

 typedef int bool;
 typedef int BOOL;
 #define false 0
 #define FALSE 0
 #define true ~false
 #define TRUE ~FALSE

 #ifndef NULL
  #define NULL	((void *)0L)
 #endif

 typedef unsigned char uchar;
 typedef unsigned char byte;

 #define MENU
 #define AESC	0x1b		// ASCII Escape char

 #define SER_PORT0 0
 #define SER_PORT1 1

#endif
