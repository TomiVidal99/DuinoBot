# AVR-GCC Makefile for ATmega328P

# Project name
TARGET = main

# Define the microcontroller
MCU = atmega328p

# Define the clock frequency
F_CPU = 16000000UL

# Source files
SRC = src/main.c src/adc.c src/usart.c src/temperature.c

# Compiler and linker flags
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -Wextra
LDFLAGS = -mmcu=$(MCU)

# Output directory
BUILD_DIR = build

# Tools
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

# AVRDUDE options
AVRDUDE_MCU = m328p
AVRDUDE_PROGRAMMER = arduino
AVRDUDE_PORT = /dev/ttyUSB0
AVRDUDE_BAUDRATE = 115200

# Output file names
ELF = $(BUILD_DIR)/$(TARGET).elf
HEX = $(BUILD_DIR)/$(TARGET).hex

# Default target
all: $(HEX)

# Build hex from elf
$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

# Build elf from source
$(ELF): $(SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Clean up build directory
clean:
	rm -rf $(BUILD_DIR)

# Flash the microcontroller
flash: $(HEX)
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMER) -p $(AVRDUDE_MCU) -P $(AVRDUDE_PORT) -b $(AVRDUDE_BAUDRATE) -U flash:w:$<

# Phony targets
.PHONY: all clean flash