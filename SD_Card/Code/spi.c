#define CS (1<<PB2)
#define MOSI (1<<PB3)
#define MISO (1<<PB4)
#define SCK (1<<PB5)
#define CS_DDR DDRB
#define CS_ENABLE() (PORTB &= ~CS)
#define CS_DISABLE() (PORTB |= CS)



void SPI_init() {
	CS_DDR |= CS; // SD card circuit select as output
	DDRB |= MOSI + SCK; // MOSI and SCK as outputs
	PORTB |= MISO; // pullup in MISO, might not be needed
	
	// Enable SPI, master, set clock rate fck/128
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) | (1<<SPR1);
}

unsigned char SPI_write(unsigned char ch) {
	SPDR = ch;
	while(!(SPSR & (1<<SPIF))) {}	
	return SPDR;
}

void SD_command(unsigned char cmd, unsigned long arg, unsigned char crc, unsigned char read) {
	unsigned char i, buffer[8];
	
	uwrite_str("CMD ");
	uwrite_hex(cmd);
	
	CS_ENABLE();
	SPI_write(cmd);
	SPI_write(arg>>24);
	SPI_write(arg>>16);
	SPI_write(arg>>8);
	SPI_write(arg);
	SPI_write(crc);
		
	for(i=0; i<read; i++)
		buffer[i] = SPI_write(0xFF);
		
	CS_DISABLE();		
		

}

int main(int argc, char *argv[]) {
	char i;
	


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