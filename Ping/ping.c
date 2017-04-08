//Used to refresh memory after break

#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	DDRB = 0b00000110;   //1 is output, 0 is input (really its about the pull-ups)
	
    DDRD &= ~(1 << DDD2);     // Clear the PD2 pin
    // PD2 (PCINT0 pin) is now an input

    PORTD |= (1 << PORTD2);    // turn On the Pull-up
    // PD2 is now an input with pull-up enabled



    EICRA |= (1 << ISC10);    // set INT0 to trigger on rising edge change
    EIMSK |= (1 << INT0);     // Turns on INT0

    sei();                    // turn on interrupts

    while(1)
    {
		pingTrig();
        _delay_ms(1000);
    }
}

ISR (INT0_vect)
{
    PORTB = 0b00000010;
}

void pingTrig(void)
{
	PORTB = 0b00000100; 	//1 high, 0 low
	_delay_ms(20); 		//delays
	PORTB = 0x00; 			// clears pins
}