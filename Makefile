# Use gnu ARM7 cross-compilation kit:
GNUARM_VERSION = 4.0.2
include gnuarm.ubuntu.mk

# Define the Device options:
DEVICE = SerCarouselCtrlr
FW_VERSION = 0.0.3
HW_VERSION = 1.0.0
FW = $(DEVICE).v$(FW_VERSION)
DEVICE_DEFINES = -D 'FW_VERSION="$(FW_VERSION)"' -D 'HW_VERSION="$(HW_VERSION)"'

# Compile the output to load into Flash Memory:
LD_TARGET = Flash
LDSCRIPT = $(LD_TARGET).ld

# Set up cross-compilation env. vars:
INCLUDES = -I . $(GNUARM_INCLUDES)
LIBS = -L . $(GNUARM_LIBS)
DEFINES = $(DEVICE_DEFINES)
CFLAGS = $(INCLUDES) $(DEFINES) -T $(LDSCRIPT) -mcpu=arm7tdmi -O0
LDFLAGS = -T $(LDSCRIPT) $(LIBS) -O0

# Define the cross-compilation commands:
ASM_COMPILE = $(GNUARM_BIN)/arm-elf-as
COMPILE     = $(GNUARM_BIN)/arm-elf-gcc $(CFLAGS)
LINK        = $(GNUARM_BIN)/arm-elf-ld $(LDFLAGS)
O_C         = $(GNUARM_BIN)/arm-elf-objcopy

# List *.h files:
C_S_A_HEADERS = GlobalDefs.h LPC.h
C_SRC_HEADERS = atoi.h packetModeComm.h main.h TimerInt.h TimerNoInt.h string.h Stepper.h Sensors.h pcProtocol.h
C_HEADERS = $(C_SRC_HEADERS) $(C_S_A_HEADERS)
# List *.s files:
ASM_SRC =       Startup.flash.s
# List *.c files:
C_SRC =         atoi.c packetModeComm.c main.c TimerInt.c TimerNoInt.c string.c Stepper.c Sensors.c pcProtocol.c
# List *.o files:              
ASM_OBJ =       Startup.flash.o
C_OBJ =         atoi.o packetModeComm.o main.o TimerInt.o TimerNoInt.o string.o Stepper.o Sensors.o pcProtocol.o
OBJ = $(ASM_OBJ) $(C_OBJ)

# Define the dependancy rules:
$(FW).hex: $(FW).bin
	$(O_C) -I binary -O ihex $(FW).bin $(FW).hex

$(FW).bin: $(FW).elf
	$(O_C) -O binary $(FW).elf $(FW).bin

$(FW).elf: $(OBJ)
	$(LINK) -o $(FW).elf $(OBJ) $(GNUARM_LIB_GCC)

clean:
	-rm -f *.firmware.bin *.firmware.elf $(OBJ)

$(ASM_OBJ): $(ASM_SRC)

$(C_OBJ): $(C_SRC) $(C_HEADERS)

$(C_SRC): $(C_HEADERS)

.SUFFIXES: .c .s .o

.c.o:
	$(COMPILE) -c -o $@ $<

.s.o:
	$(ASM_COMPILE) -o $@ $<
