//Used to refresh memory after break

#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	Light bright;
	bright.lit1 = 0b00000100;
	bright.num = 5;
	DDRB = bright.lit1;   //1 is output, 0 is input (really its about the pull-ups)
	while(1){
		PORTB = bright.lit1; 	//1 high, 0 low
		_delay_ms(bright.time()); 		//delays for a second
		PORTB = ~bright.lit1; 			// clears pins
		_delay_ms(bright.time());
	}
}
class Light()
{
	uint8_t lit1;
	int num;
	
	int time()
	{
		return num*1000;
	}
}
