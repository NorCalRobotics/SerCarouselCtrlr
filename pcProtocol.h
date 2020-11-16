#ifndef PC_PROTOCOL_H_INCLUDED
 #define PC_PROTOCOL_H_INCLUDED

 #include "string.h"
 #include "packetModeComm.h"

 typedef char * (* cmdExecutor)(char *);

 #define C_R    '\r'
 #define EOL    '\n'

 #define MAX_CMD_LEN    64

 void initPcProtocol(cmdExecutor execFunc);
 void pollPcProtocol();

 void pcProtocolSendStringResp(char * resp);

#endif

