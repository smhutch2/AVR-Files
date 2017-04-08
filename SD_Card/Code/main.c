#include "spi.c"
#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 1000000UL

int main(int argc, char *argv[]) {
	char i;
	
	USARTInit(64); // 20 MHz / (16 * 19200 baud) - 1 = 64.104x
	SPI_init();

	// ]r:10
	CS_DISABLE();
	for(i=0; i<10; i++) // idle for 1 bytes / 80 clocks
		SPI_write(0xFF);
	
	while(1) {
		switch(USARTReadChar()) {
		case '1':
			SD_command(0x40, 0x00000000, 0x95, 8);
			break;
		case '2':
			SD_command(0x41, 0x00000000, 0xFF, 8);
			break;
		case '3':
			SD_command(0x50, 0x00000200, 0xFF, 8);
			break;
		}
	}	
	
	return 0;
}