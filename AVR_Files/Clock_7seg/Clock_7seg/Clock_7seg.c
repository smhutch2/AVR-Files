/*
 * Clock_7seg.c
 *
 * Created: 12/7/2014 10:28:08 AM
 *  Author: Spencer Hutchinson
 */ 
#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
int time[3] = {12,0,0}; // hour,minute,second
int d1;
int d2;
int d3;
int d4;
//*-----7SEG FUNCTIONS------*
void display(void)
{
	display_rst();
	PORTC = 0b00000001;
	if (d1 != 0)
	{
		numCall(d1);
	}
	display_rst();
	PORTC = 0b00000010;
	numCall(d2);
	display_rst();
	PORTC = 0b00000100;
	colon();
	display_rst();
	PORTC = 0b00001000;
	numCall(d3);
	display_rst();
	PORTC = 0b00010000;
	numCall(d4);
	
}
void set7(void)
{
	d1 = parse1(time[0]);
	d2 = parse2(time[0]);
	d3 = parse1(time[1]);
	d4 = parse2(time[1]);
}
void num0(void)
{
	PORTB = 0b00000100;
	PORTD = 0b01000000;
}
void num1(void)
{
	PORTB = 0b00101101;
	PORTD = 0b11000000;
}
void num2(void)
{
	PORTB = 0b00110000;
	PORTD = 0b01000000;
}
void num3(void)
{
	PORTB = 0b00100000;
	PORTD = 0b11000000;
}
void num4(void)
{
	PORTB = 0b00001001;
	PORTD = 0b11000000;
}
void num5(void)
{
	PORTB = 0b00000010;
	PORTD = 0b11000000;
}
void num6(void)
{
	PORTB = 0b00000010;
	PORTD = 0b01000000;
}
void num7(void)
{
	PORTB = 0b00100101;
	PORTD = 0b11000000;
}
void num8(void)
{
	PORTB = 0b00000000;
	PORTD = 0b01000000;
}
void num9(void)
{
	PORTB = 0b00000001;
	PORTD = 0b11000000;
}

void colon(void)
{
	PORTB = 0b00110101;
	PORTD = 0b11000000;
}
void display_rst(void)
{
	PORTB = 0xFF;
	PORTD = 0xFF;
}

int parse1(int pval) //turn the values to being usable by display
{
	int d1;
	//int d2;

	for (int i = 0; i <= 60; i = i + 10)
	{
		
		for (int f = 0; f <= 9; f = f + 1)
		{
			int g = i + f;
			if (pval == g)
			{
				d1 = i / 10;
				//d2 = f;
			}
		}
	}
	return d1;
}
int parse2(int pval) //turn the values to being usable by display
{
	//int d1;
	int d2;

	for (int i = 0; i <= 60; i = i + 10)
	{
		
		for (int f = 0; f <= 9; f = f + 1)
		{
			int g = i + f;
			if (pval == g)
			{
	//			d1 = i / 10;
				d2 = f;
			}
		}
	}
	return d2;
}

void numCall(int num)
{
	if (num == 0)
	{
		num0();
	}
	if (num == 1)
	{
		num1();
	}
	if (num == 2)
	{
		num2();
	}
	if (num == 3)
	{
		num3();
	}
	if (num == 4)
	{
		num4();
	}
	if (num == 5)
	{
		num5();
	}
	if (num == 6)
	{
		num6();
	}
	if (num == 7)
	{
		num7();
	}
	if (num == 8)
	{
		num8();
	}
	if (num == 9)
	{
		num9();
	}

}
void DisInit(void)
{
	DDRB = 0b00111111;
	DDRC = 0b00011111;
	DDRD = 0b11000000;
	display_rst();
}
//[--------BUTTON FUNCTIONS--------]
	int pressed;
void buttPress(void)
{
	pressed = 0;
	if(PIND & (1<<PD2))
	{
		time[0] = time[0] + 1;
		pressed = 1;
		set7();
	}
	else
	{
//		pressed = 0;
	}
	if(PIND & (1<<PD3))
	{
		time[1] = time[1] + 1;
		pressed = 1;
		set7();
	}
	else
	{
//		pressed = 0;
	}
	if(time[1] > 59)//60 minutes
	{
		time[1] = 0;
	//	time[0] = time[0] + 1; //add hour
		set7();
	}
	if(time[0] > 12)
	{
		time[0] = 1;
		set7();
	}	

}
//|--------CLOCK FUNCTIONS-------|
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
void count(void)
{
	time[2] = time[2] + 1; // count seconds
	if(time[2] > 59) //when 60 set back to 0
	{	
		//resets clock
		time[2] = 0;
		time[1] = time[1] + 1; //+1 minute
		set7();
	}
	if(time[1] > 59)//60 minutes
	{
		time[1] = 0;
		time[0] = time[0] + 1; //add hour
		set7();
	}
	if(time[0] > 12)
	{
		time[0] = 1;
		set7();
	}

}
ISR(TIMER2_OVF_vect)
{	
	count();
	asm volatile("nop"::);
	//_delay_us(10);
;

}

//<------MAIN------->
int main (void)
{
	DisInit();
	
	SMCR|=(1<<SM1)|(1<<SM0);
	//Initialize port
//	PortInit();
	//Initialize the timer2
	RTCInit();
	//Enable global interrupts
	sei();
	set7();
	int fast;
	while(1)
	{
		if (pressed = 1)
		{
		fast++;			
		}

		if(fast > 300)
		{

			buttPress();
			fast = 0;		
		}
		
		//SMCR|=(1<<SE);
		//asm volatile("sleep"::);
	//	SMCR&=~(1<<SE);
		//Do nothing
		display();
/*		if(pressed = 1)
		{
			delay(10);
		}*/

	}
}
