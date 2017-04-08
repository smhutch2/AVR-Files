/*
 * Menorah_t2313.c
 *
 * Created: 12/14/2014 12:17:52 AM
 *  Author: Spencer
 */ 
#define F_CPU 1000000UL
#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	int count = 0;
	DDRB = 0xff;
    while(1)
    {

		if (PIND & (1<<PD2))
		{
			count++;

		}
		if(count > 7)
		{
			count = 0;
			PORTB =0b00000001;
		}
        PORTB |= (1<<count);
		_delay_ms(100);
    }
}
