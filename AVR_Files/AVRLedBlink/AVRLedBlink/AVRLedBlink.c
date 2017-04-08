/*
 * AVRLedBlink.c
 *
 * Created: 11/21/2014 5:54:44 PM
 *  Author: Spencer
 */ 
#define 	UART_RX_BUFFER_SIZE   128
#define 	UART_TX_BUFFER_SIZE   128
#define F_CPU 1000000UL
#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/uart.h>

int main (void)
{
	// set all pins on PORTB for output
	DDRB = 0b00100001;
//	uint16_t baudrate = 0x2580;
//	uart_init(baudrate);
	int count = 0;
	int wait = 50; //multiplier by 10 in delay in ms so 
	while (1)
	{	
//		uint8_t dat = 0b00010000;
//		uart_putc(dat);
		unsigned int val = PINB;
		int on;
		
		if (val & (1<<PB1))    // see comment #2
		{
			on = 1;
			PORTB |= (1<<PB0);  // |= adds without resetting PORTB
			count++;	
		}
		else
		{
			on = 0;
			PORTB &= ~(1<<PB0); // &= 
		}
		if(count >= wait && on == 1)
		{
			count = 0;
			PORTB |= (1<<PB5);
		}
		if (on == 0)
		{
			PORTB &= ~(1<<PB5);				
		}
		_delay_ms(10);

	}
//	return 0;
}