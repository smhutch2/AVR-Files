/*
 * Serial.c
 *
 * Created: 1/17/2015 7:03:20 PM
 *  Author: Spencer
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "uart.c"
#include "uart.h"
#define F_CPU 1000000UL
int main(void) {
	char serialCharacter;


	// -------- Inits --------- //

	UART_init();
    /* to test */
	UART_puts("H");
	// ------ Event loop ------ //
	while (1) {

		/*serialCharacter = receiveByte();
		transmitByte(serialCharacter);
		LED_PORT = serialCharacter;*/
		// display ascii/numeric value of character 
			
		_delay_ms(1000);
		}                                                  /* End event loop */
		return (0);
	}
