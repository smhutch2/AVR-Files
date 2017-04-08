/*
 * AVR_Clock.c
 *
 * Created: 12/2/2014 10:11:50 PM
 *  Author: Spencer
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
int time[3] = {0,0,0}; // hour,minute,second
//Prepare LED pin
void PortInit(void)
{
	//Initially LED OFF
	PORTB&=~(1<<PB0);
	//PD0 as output
	DDRB|=(1<<PB0)|(1<<PB5);
}
//Timer2 init according datasheet
void RTCInit(void)
{
	//Disable timer2 interrupts
	TIMSK2  = 0;
	//Enable asynchronous mode
	ASSR  = (1<<AS2);
	//set initial counter value
	TCNT2=0;
	//set prescaller 128
	TCCR2B |= (1<<CS22)|(1<<CS00);
	//wait for registers update
	while (ASSR & ((1<<TCN2UB)|(1<<TCR2BUB)));
	//clear interrupt flags
	TIFR2  = (1<<TOV2);
	//enable TOV2 interrupt
	TIMSK2  = (1<<TOIE2);
}
//Overflow ISR
ISR(TIMER2_OVF_vect)
{
	//Toggle pin PD0 every second
	PINB=(1<<PB0);
	asm volatile("nop"::);
	//_delay_us(10);
	count();
}
int count(void)
{
	time[2] = time[2] + 1; // count seconds
	if(time[2] > 59) //when 60 set back to 0
	{
		PORTB |= (1<<PB5);
		time[2] = 0;
		time[1] = time[1] + 1; //+1 minute
	}
	if(time[1] > 59)//60 minutes
	{
		time[1] = 0;
		time[0] = time[0] + 1; //add hour
	}
	if(time[0] > 12)
	{
		time[0] = 0;
	}

}	
int main (void)
{
	SMCR|=(1<<SM1)|(1<<SM0);
	//Initialize port
	PortInit();
	//Initialize the timer2
	RTCInit();
	//Enable global interrupts
	sei();
	while(1)
	{
		SMCR|=(1<<SE);
		asm volatile("sleep"::);
		SMCR&=~(1<<SE);
		//Do nothing
		
	}
}
