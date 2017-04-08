
#define F_CPU 1000000UL

#include <avr/iom328.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pinDefines.h"
//#include "iom328p.h"

const int ServoSpeed = 50; //Data rate of servo in Hz

void setAngle(int angle)
{ //angle acceptable from 0 to 180

	OCR1A = ICR1-((angle/180) * (F_CPU*.001)+(F_CPU*.001));	
}

int main(void){
	
//	DDRD |= 0xFF;
//	TCCR1A |= 1<<WGM11 | 1<<COM1A1 | 1<<COM1A0;
//	TCCR1B |= 1<<WGM13 | 1<<WGM12 | 1<<CS10;
	
	TCCR1A |= (1<<com1A1) | (1<<wgm11); // non-inverting mode for OC1A
	TCCR1B |= (1<<wgm13) | (1<<wgm12) | (1<<cs11); // Mode 14, Prescaler 8

	ICR1 = F_CPU / ServoSpeed; //

	DDRB |= (1<<pb1); // OC1A set to output

	while(1){

		setAngle(90);
		_delay_ms(100);
	}
}

