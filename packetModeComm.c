#include "packetModeComm.h"
#include <LPC.h>

packetizedByte PcCommPortRxBuffer[SERIAL_RX_BUFFER_SIZE];
volatile PacketFifoControl pcRxFifo;

BOOL isInPacketedMode = FALSE;

void ZeroPcFifoBuffers(){
	memset(&(PcCommPortRxBuffer[0]), 0,     SERIAL_RX_BUFFER_SIZE);
	memset((void *)&pcRxFifo,        0, sizeof(PacketFifoControl));

	pcRxFifo.iBufferSize = SERIAL_RX_BUFFER_SIZE;
	pcRxFifo.Buffer      = &(PcCommPortRxBuffer[0]);
}

void setPcSerialSpeed9600bps(){
	PINSEL0 |= 0x00000005; /* Enable RxD0 and TxD0                             */
	U0LCR = 0x83;          /* 8 bits, no Parity, 1 Stop bit                    */
	U0DLL = 0x62;        /* 9600  bps Baud Rate @ 15MHz VPB Clock (60/4Mhz)  */
	U0LCR = 0x03;          /* Divisor Latch Access Bit (DLAB) = 0              */
	U0FCR = 0x01;          // enable the on-chip FIFOs
}

void EnablePcSerRxInt(unsigned long intFunctionAddress){
	VICVectAddr0 = intFunctionAddress;
	VICVectCntl0 = 0x20 | 0x06; // 0x20 enables the irq slot, interrupt source 0x06 is UART0

	long unsigned int curIntsEnabledMask;
	curIntsEnabledMask = U0IER;
	curIntsEnabledMask &= 0x7; // binary x111 (INTS_ENABLED_MASK = bits 0-2)
	curIntsEnabledMask |= 0x1; // binary x001 (ENABLE_RX         = bit    0)
	U0IER = curIntsEnabledMask;

	curIntsEnabledMask = VICIntEnable;
	curIntsEnabledMask |= 0x40; // Bit 6 is UART0
    VICIntEnable = curIntsEnabledMask;
}

void initializePacketedMode(){
	if(isInPacketedMode) return;

	ZeroPcFifoBuffers();
	setPcSerialSpeed9600bps();
	EnablePcSerRxInt((unsigned long)pcSerialRxInterrupt);

	isInPacketedMode = TRUE;
}

int nextPositionInFIFO(PacketFifoControl * fifo, int idx){
	int nextIdx = idx + 1;

	if(nextIdx < fifo->iBufferSize){
		return(nextIdx);
	}
	else{
		return(0);
	}
}

BOOL isPcRxFifoEmpty(){
	return(pcRxFifo.numBytesBuffered <= 0);
}

BOOL doesPcRxFifoHaveBytes(int numBytesRequired){
	return(pcRxFifo.numBytesBuffered >= numBytesRequired);
}

unsigned long int PcRxFifo_savedInterrupts;
void PcRxFifo_LockCpuInterrupts(){
	PcRxFifo_savedInterrupts = VICIntEnable;

	VICIntEnClr = 0xFFFFFFFF;
	// unsigned long int curIntsEnabledMask = PcRxFifo_savedInterrupts & (~(0x40 | 0x10)); // Bit 5 is Timer0, 6 is UART0
    // VICIntEnable = curIntsEnabledMask;
}

void PcRxFifo_UnlockCpuInterrupts(){
	VICIntEnable = PcRxFifo_savedInterrupts;
}

packetizedByte getByteFromPcRxFifo(){
	PcRxFifo_LockCpuInterrupts();

	if(pcRxFifo.numBytesBuffered <= 0) return(0);
		
	packetizedByte ByteFromBuffer = pcRxFifo.Buffer[pcRxFifo.idxOut];		
	pcRxFifo.idxOut = nextPositionInFIFO((PacketFifoControl *)&pcRxFifo, pcRxFifo.idxOut);
	pcRxFifo.numBytesBuffered--;

	PcRxFifo_UnlockCpuInterrupts();

	return(ByteFromBuffer);
}

void PcTxByte(packetizedByte txByte){
	while((U0LSR & 0x20) == 0){ }

        U0THR = txByte;
}

PacketFifoControl * getPcRxFifo(){
	return((PacketFifoControl *)&pcRxFifo);
}

void pcSerialRxInterrupt(){
	if(U0LSR & 0x01){
		packetizedByte rcvdByte = U0RBR;

		pcRxFifo.Buffer[pcRxFifo.idxIn] = rcvdByte;

		int nextIdx = pcRxFifo.idxIn + 1;
		if(nextIdx >= pcRxFifo.iBufferSize) nextIdx = 0;
		pcRxFifo.idxIn = nextIdx;

		pcRxFifo.numBytesBuffered++;
  	}

	VICVectAddr = 0; // Acknowledge Interrupt
}
