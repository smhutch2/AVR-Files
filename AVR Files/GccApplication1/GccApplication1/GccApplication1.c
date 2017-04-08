/*
 * GccApplication1.c
 *
 * Created: 11/20/2014 7:17:27 PM
 *  Author: Spencer
 */ 

#define F_CPU 16000000UL                                    /* Clock Frequency = 16Mhz */

#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

int main(){                         // The main function

	DDRB = 0b11111111;                    // Set all the pins of PortB as output

	while (1) {                        // Set up an infinite loop

		PORTB = 0b10000000;                    // Turn on LED1
		_delay_ms(50);                        // Wait
	}

}