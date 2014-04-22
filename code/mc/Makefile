###############################################################################
# Makefile for the project basis
###############################################################################
# Source
# Name your Project
PROJECT = lamp

# Sourcefiles
SOURCE = main.c
SOURCE += humidity.c frequency_meter.c
SOURCE += communication.c
SOURCE += colours.c psc.c
SOURCE += sleepy.c
# Libraries
LIBS = -lm

INCLUDES = -I".."

# Optimization
# O0 - no optimization
# O1 and 02 - optimize for speed
# O3 - faster but not always correct
# Os - optimize for programmsize (default)
OPT=Os





#keep the next line
all:


.PHONY: tags
tags:
	ctags -R --exclude=/usr/avr/include/avr ${SOURCE} $(CURDIR) /usr/avr/include

# Programmer
PROGRAMMER_PORT = /dev/ttyUSB0	# for crispavr usb
#PROGRAMMER_PORT = /dev/ttyACM0	# for diamex usb
#PROGRAMMER_PORT = /dev/ttyS0	# for the serial port based


#select programmer
#prog: prog-avrisp_mkII	## and all programmers compatible to it
#prog: prog-pony_serial	## The pony serial cable is a low budget solution for those that still have a serial port
prog: prog-stk500v2		## All programmers compatible with the stk500v2 (includes those based on usb AND those attached to the serial port)

# Hardware
MCU   = at90pwm3b
F_OSC = 1000000ul		## Because a 16MHz crystal and a clock division by 8 results in 1MHz. Riiiiiiiiiight..... However I've got no clue why this is the actual frequency :(

# How to use this Makefile:
# 'make all' Compiles everything
# 'make prog' downloads programm to the MC
# 'make clean' deletes objectfiles
# 'make file.lss' dissambles file.o


## General Flags
TARGET = $(PROJECT)
#CC = avr-gcc.exe
CC = avr-gcc

#
# Paths
#

OBJDIR = obj
BITDIR = bit
OBJ = $(patsubst %.c, $(OBJDIR)/%.o, $(SOURCE))


## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU) -DF_CPU=$(F_OSC)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON) $(INC)
CFLAGS += -std=c99
#CFLAGS += -Wall -Wunused -Wextra -$(OPT) -fsigned-char -Wno-pragmas -Wpedantic
CFLAGS += -Wall -Wunused -Wextra -Wpedantic -$(OPT) -fsigned-char
CFLAGS += -MD -MP -MT $@ -MF dep/$(@F).d
#CFLAGS += -DMC_SIDE
CFLAGS += -I../
CFLAGS += -I.
CFLAGS += -Wfatal-errors


## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
##LDFLAGS +=


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0



## Objects that must be built in order to link
OBJECTS = $(OBJ)

## Objects explicitly added by the user
##LINKONLYOBJECTS =

## Build
all: $(OBJDIR) $(TARGET).elf $(TARGET).hex $(TARGET).eep
#size

## Compile
$(OBJDIR):
	@mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.c
	@echo Compiling $< ...
	@$(CC) $(INCLUDES) $(CFLAGS) -c  $< -o $@

##Link
$(TARGET).elf: $(OBJECTS)
	 @echo Linking $@ ...
	 @$(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET).elf

%.hex: $(TARGET).elf
	@echo Creating $@ ...
	@avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET).elf
	@echo Creating $@ ...
	@avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@

%.lss: $(TARGET).elf
	@echo Creating $@ ...
	@avr-objdump -h -S $< > $@

size: $(TARGET).elf
	@echo size ${TARGET}.elf
	@avr-size --mcu=${MCU} ${TARGET}.elf

## Clean target
.PHONY: clean prog 
clean:
	rm -rf $(OBJECTS) dep/ obj/ $(TARGET).eep $(TARGET).hex $(TARGET).elf

## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)


prog-avrisp_mkII: all #original avr programmer
	avrdude -p $(MCU) -c avrispmkII -P usb -U flash:w:$(TARGET).hex:i
prog-pony_serial: all #pony serial programmer cable (low-budget)
	avrdude -p $(MCU) -c ponyser -P $(SERPORT) -U flash:w:$(TARGET).hex:i
prog-funk: all #funk by jschleic
	avrdude -p $(MCU) -c stk500v2 -P $(SERPORT) -U flash:w:$(TARGET).hex:i
prog-stk500v2: all #usb programmer
	avrdude -p $(MCU) -c stk500v2 -P $(PROGRAMMER_PORT) -U flash:w:$(TARGET).hex:i

.PHONY: getfuse
getfuse:
ifeq ($(MCU),at90pwm3b)
	#avrdude -p $(MCU) -c avrispmkII -P usb -U lfuse:w:0xff:m -U hfuse:w:0xd7:m
	avrdude -p at90pwm3b -c stk500v2 -P /dev/ttyUSB0 -U efuse:r:-:b -U hfuse:r:-:b -U lfuse:r:-:b
#	avrdude -p $(MCU) -c stk500v2 -P $(PROGRAMMER_PORT) -U lfuse:w:0xff:m -U hfuse:w:0xd7:m
else
	@echo Controller $(MCU) for make getfuse not supported in this makefile, only at90pwm3b
endif

.PHONY: fuse
fuse:
ifeq ($(MCU),at90pwm3b)
	avrdude -p at90pwm3b -c stk500v2 -P $(PROGRAMMER_PORT) -U efuse:r:0b00001001:b -U lfuse:r:0b01111111:b
else
	@echo Controller $(MCU) for make fuse not supported in this makefile, only at90pwm3b
endif


###  set to:
##
##EFUSE= PSC2RB   | PSC1RB | PSC0RB | PSCRV |   -    | BOOTSZ1   | BOOTSZ0   | BOOTRST	## FORCE PSCs to high to disable the led drivers
##         0          0        0        0       1         0           0           1		## whose input is active low
##HFUSE= RSTDISBL | DWEN   | SPIEN  | WDTON | EESAVE | BODLEVEL2 | BODLEVEL1 | BODLEVEL0	## I just won't touch these...
##         1          1        0        1       1         1           1           1		## There be dragons...
##LFUSE= CKDIV8   | CKOUT  | SUT1   | SUT0  | CKSEL3 | CKSEL2    | CKSEL1    | CKSEL0		## Divide the 16MHz clock by 8, no CKOUT, max startup time
##         0          1        1        1       1         1           1           1		## CKSEL to 16MHz crystal oscillator
##avrdude -p at90pwm3b -c stk500v2 -P /dev/ttyUSB0 -U efuse:w:0b111 0 1 001:b -U hfuse:r:-:b -U lfuse:r:-:b
##


##  original get fuses output:
##
## ❯ avrdude -p at90pwm3b -c stk500v2 -P /dev/ttyUSB0 -U efuse:r:-:b -U hfuse:r:-:b -U lfuse:r:-:b
##
##avrdude: AVR device initialized and ready to accept instructions
##
##Reading | ################################################## | 100% 0.07s
##
##avrdude: Device signature = 0x1e9383
##avrdude: reading efuse memory:
##
##Reading | ################################################## | 100% 0.02s
##
##avrdude: writing output file "<stdout>"
##0b11111001													EFUSE=0b11111001 = PSC2RB | PSC1RB | PSC0RB | - | BOOTSZ1 | BOOTSZ0 | BOOTRST
##avrdude: reading hfuse memory:
##
##Reading | ################################################## | 100% 0.02s
##
##avrdude: writing output file "<stdout>"
##0b11011111													HFUSE=0b11011111 = RSTDISBL | DWEN | SPIEN | WDTON | EESAVE | BODLEVEL2 | BODLEVEL1 | BODLEVEL0
##avrdude: reading lfuse memory:
##
##Reading | ################################################## | 100% 0.02s
##
##avrdude: writing output file "<stdout>"
##0b1100010														LFUSE=0b01100010 = CKDIV8 | CKOUT | SUT1 | SUT0 | CKSEL3 | CKSEL2 | CKSEL1 | CKSEL0
##
##avrdude: safemode: Fuses OK (E:F9, H:DF, L:62)
##
##avrdude done.  Thank you.
##
