/*
 * ATTiny85_LEDBlink.c
 *
 * Created: 11/21/2014 8:36:38 PM
 *  Author: Spencer
 */ 


#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

int main (void)
{

	DDRB = 0b001100;
	PORTB = 0b010000;
	
	while(1)
	{
		/* Define pull-ups and set outputs high */
		/* Define directions for port pins */
	
		if(PINB == 0b010000)
		{
			PORTB = 0b001100;		
		}
		else
		{
			PORTB = 0b000000;
		}
		
		_delay_ms(10);
	}
}