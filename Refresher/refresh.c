//Used to refresh memory after break

#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	uint8_t lit = 0b00000100;
	DDRB = lit;   //1 is output, 0 is input (really its about the pull-ups)
	while(1){
		PORTB = lit; 	//1 high, 0 low
		_delay_ms(1000); 		//delays for a second
		PORTB = 0; 			// clears pins
		_delay_ms(1000);
	}
}
