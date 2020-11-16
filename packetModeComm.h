#ifndef  PACKETS_MODE_COMM_H_INCLUDED
 #define PACKETS_MODE_COMM_H_INCLUDED

 #define SERIAL_RX_BUFFER_SIZE    0x200

 #include "GlobalDefs.h"
 #include <string.h>

 typedef unsigned char packetizedByte;

  typedef struct s_PacketFifoControl{
	int numBytesBuffered, idxIn, idxOut, iBufferSize;
	packetizedByte * Buffer;
  } PacketFifoControl;

 void initializePacketedMode();

 BOOL isPcRxFifoEmpty();
 BOOL doesPcRxFifoHaveBytes(int numBytesRequired);
 packetizedByte getByteFromPcRxFifo();
 void PcTxByte(packetizedByte txByte);

 void setPcSerialSpeed9600bps();

 PacketFifoControl * getPcRxFifo();

 void pcSerialRxInterrupt() __attribute__ ((interrupt("IRQ")));

#endif


