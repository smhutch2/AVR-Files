/*
 * _7seg.c
 *
 * Created: 12/6/2014 2:11:05 PM
 *  Author: Spencer
 */ 
#define F_CPU 1000000UL
#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

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
	int d2;

	for (int i = 0; i <= 60; i = i + 10)
	{
		
		for (int f = 0; f <= 9; f = f + 1)
		{
			int g = i + f;
			if (pval == g)
			{
				d1 = i / 10;
				d2 = f;
			}
		}
	}
	return d1;
}
int parse2(int pval) //turn the values to being usable by display
{
	int d1;
	int d2;

	for (int i = 0; i <= 60; i = i + 10)
	{
		
		for (int f = 0; f <= 9; f = f + 1)
		{
			int g = i + f;
			if (pval == g)
			{
				d1 = i / 10;
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
}
int main(void)
{
	DisInit();
	int time [3] = {5,15,0};
	int d1 = parse1(time[0]);
	int d2 = parse2(time[0]);
	int d3 = parse1(time[1]);
	int d4 = parse2(time[1]);
	display_rst();
    while(1)
    {
		//PORTB = 0b00111111;
		//PORTD = 0b11000000;

		PORTC = 0b00000001;	
		numCall(d1);
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
		display_rst();
		//PINB = 0b00000000;
		//PORTB |= (1<<PB0);  // |= adds without resetting PORTB        
		//PORTB &= ~(1<<PB0); // &= 
    }
}

