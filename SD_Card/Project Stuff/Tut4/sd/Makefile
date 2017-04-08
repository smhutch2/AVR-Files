CC = avr-gcc
OBJCOPY = avr-objcopy
DUDE = avrdude

MCU=atmega328p
TARGET= fat88
CFLAGS = -Os -mmcu=$(MCU)
OBJFLAGS = -j .text -j .data -O ihex
DUDEFLAGS = -p $(MCU) -c usbasp -q -B 1 -

all: $(TARGET).hex

%.hex: %.elf
	$(OBJCOPY) -R .eeprom -O ihex $< $@

%.elf: $(SRC)
	$(CC) $(CFLAGS) $(SRC) --output $@ 

%.eeprom: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@ 

debug:
	@echo
	@echo "Source files:"   $(SRC)
	@echo "MCU, F_CPU, BAUD:"  $(MCU), $(F_CPU), $(BAUD)
	@echo	

flash: $(TARGET).hex 
	$(AVRDUDE) -c usbasp -F -p  $(MCU) -U flash:w:$<