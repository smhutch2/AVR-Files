/*
 * FakePWMm328p.c
 *
 * Created: 11/24/2014 6:27:47 PM
 *  Author: Spencer
 */ 


#define F_CPU 1000000UL
#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/uart.h>

int main (void)
{
	DDRB = 0b00100001;
	int count = 0;
	int sample = 10; // between 0 and 10
	unsigned char LED1 = PB5;
	while (1)
	{	
		if(count > 10)
		{
			count = 0;
		}
		if(count == sample)
		{
			PORTB |= (1<<LED1);  // |= adds without resetting PORTB			
		}
		if(count > sample)
		{
			PORTB &= ~(1<<LED1);
		}
		count = count + 1;
	}
		return 0;
}