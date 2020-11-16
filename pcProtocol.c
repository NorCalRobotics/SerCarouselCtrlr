#include "pcProtocol.h"

int pcSerPort, cmdSize, isCmdComplete;
char cmdBuffer[MAX_CMD_LEN], * cmd;
cmdExecutor stored_execFunc;

void initPcProtocol(cmdExecutor execFunc){
    initializePacketedMode();

    stored_execFunc = execFunc;

    isCmdComplete = 0;
    cmdSize = 0;
    cmd = &(cmdBuffer[0]);
    memset(cmd, 0, MAX_CMD_LEN);
}

void pollPcProtocol(){
    packetizedByte rcvdByte;
    char * cmdResp;

    if(!isPcRxFifoEmpty()){
        rcvdByte = getByteFromPcRxFifo();
        if(rcvdByte > 0){
            if((rcvdByte == C_R) || (rcvdByte == EOL)){
                cmdBuffer[cmdSize] = '\0';
                isCmdComplete = 1;
            }
            else{
                cmdBuffer[cmdSize] = (char)rcvdByte;
                cmdSize++;
            }
        }
    }

    if(isCmdComplete){
        if(stored_execFunc != NULL){
            cmdResp = stored_execFunc(cmd);
            pcProtocolSendStringResp(cmdResp);
        }

        isCmdComplete = 0;
        cmdSize = 0;
    }
}

void pcProtocolSendStringResp(char * resp){
    int idx;
    packetizedByte byteToSend;

    for(idx = 0; resp[idx] != '\0'; idx++){
        byteToSend = (packetizedByte)(resp[idx]);
        PcTxByte(byteToSend);
    }
    PcTxByte(EOL);
}

