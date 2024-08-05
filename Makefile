# AVR-GCC Makefile for ATmega328P

# Project name
TARGET = main

# Define the microcontroller
MCU = atmega1284p

# Define the clock frequency
F_CPU = 16000000UL

# Source files
SRC = src/*.c

# AVR Libc dependencies
INCLUDE_AVR = lib/avr
INCLUDE_UTIL = lib/util

# Compiler and linker flags
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -Wextra -Werror  -I$(INCLUDE_AVR) -I$(INCLUDE_UTIL)
LDFLAGS = -mmcu=$(MCU)

# Output directory
BUILD_DIR = build

# Tools
CC = avr-gcc
OBJCOPY = avr-objcopy
HID = hiduploader.exe

# HID options
HID_MCU = atmega1284p

# Output file names
ELF = $(BUILD_DIR)/$(TARGET).elf
HEX = $(BUILD_DIR)/$(TARGET).hex

# Default target
all: $(HEX)

	mkdir $(BUILD_DIR)

# Build elf from source
$(ELF): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

# Build hex from elf
$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@


# Clean up build directory
clean:
	rm -rf $(BUILD_DIR)/*

# Flash the microcontroller
flash: $(HEX)
	$(HID) -mmcu=$(HID_MCU) -v -usb=0x2842,0x0001 "$(HEX)"

# Phony targets
.PHONY: all clean flash