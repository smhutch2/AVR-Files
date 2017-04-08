// Quick and dirty adjustable-threshold night-light.

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include "pinDefines.h"

uint16_t readADC(uint8_t channel) {
  ADMUX = (0xf0 & ADMUX) | channel;
  ADCSRA |= (1 << ADSC);
  loop_until_bit_is_clear(ADCSRA, ADSC);
  return (ADC);
}

static inline void initTimerServo(void) {
                   /* Set up Timer1 (16bit) to give a pulse every 20ms */
                             /* Use Fast PWM mode, counter max in ICR1 */
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM12) | (1 << WGM13);
  TCCR1B |= (1 << CS10);  /* /1 prescaling -- counting in microseconds */
  ICR1 = 20000;                                    /* TOP value = 20ms */
  TCCR1A |= (1<<COM1B1)|(1 << COM1A1);              /* Direct output on PB1 / OC1A */
  DDRB |= (1 << PB1)|(1<<PB2);                            /* set pin for output */
}

// static inline void initTimer2Servo(void) {
                   // /* Set up Timer1 (16bit) to give a pulse every 20ms */
                             // /* Use Fast PWM mode, counter max in ICR1 */
  // TCCR2A |= (1 << WGM11);
  // TCCR2B |= (1 << WGM12) | (1 << WGM13);
  // TCCR2B |= (1 << CS10);  /* /1 prescaling -- counting in microseconds */
  // ICR2 = 20000;                                    /* TOP value = 20ms */
  // TCCR2A |= (1 << COM1A1);              /* Direct output on PB1 / OC1A */
  // DDRB |= (1 << PB3);                            /* set pin for output */
// }

static inline void moveServo1(uint16_t pulse1, uint16_t pulse2, uint16_t duration)
{
	OCR1A = pulse1;
    DDRB |= (1 << PB1);                        /* re-enable output pin */
    _delay_ms(duration);
	OCR1A = pulse2;
    // while (TCNT1 < 3000) {;
    // }                          /* delay until pulse part of cycle done */
    // DDRB &= ~(1 << PB1);                         /* disable output pin */

}

static inline void moveServo2(uint16_t pulse1, uint16_t pulse2, uint16_t duration)
{
	OCR1B = pulse1;
    DDRB |= (1 << PB2);                        /* re-enable output pin */
    _delay_ms(duration);
	OCR1B = pulse2;
    // while (TCNT1 < 3000) {;
    // }                          /* delay until pulse part of cycle done */
    // DDRB &= ~(1 << PB2);                         /* disable output pin */

}


int main(void) {
  // -------- Inits --------- //
  uint16_t RedThreshold = 500;
  uint16_t BlueThreshold = 500;
  uint16_t threshold = 500; 
  uint16_t valueR1;
  uint16_t valueB1;
  uint16_t valueR2;
  uint16_t valueB2;
  uint8_t color; //true red, false blue
  // Set up ADC
  ADMUX |= (1 << REFS0);                  /* reference voltage on AVCC */
  ADCSRA |= (1 << ADPS1) | (1 << ADPS0);     /* ADC clock prescaler /8 */
  ADCSRA |= (1 << ADEN);                                 /* enable ADC */
	
 initTimerServo();
 OCR1A = 500;
  OCR1B = 500;
// initTimer2Servo();
 // LED_DDR = 0xff;
  
 // uint8_t lit = 0b00001100;
  DDRB |= ((1<<5)|(1<<4));   //1 is output, 0 is input (really its about the pull-ups)
  PORTD |= (1<<PD5)|(1<<PD6)|(1<<PD7);
  // ------ Event loop ------ //
  while (1) {
	
	//calibrate button press
	if((PIND & (1<<PD6)) == 0)
	{
		RedThreshold = readADC(PC0);
		BlueThreshold= readADC(PC1);
		while((PIND & (1<<PD6)) == 0);
		uint16_t temp = readADC(PC0);
		RedThreshold = (RedThreshold - temp)/2 + temp;
		BlueThreshold= (readADC(PC1) - BlueThreshold)/2 + BlueThreshold;
	}
	
	//hypothetical start button press
	if((PIND & (1<<PD7)) == 0)
	{
		PORTB &= ~(1<<4);
		PORTB &= ~(1<<5);
		if((PIND & (1<<PD5)) == 0) color = 1;
		else color = 0;
		//Get averages to ensure accurate data
		valueR1 = 0;
		valueB1 = 0;
		valueR2 = 0;
		valueB2 = 0;	
		uint16_t i;
		for(i = 0; i<10; i++)
		{
			valueR1 += readADC(PC0);
			valueB1 += readADC(PC1);
			valueR2 += readADC(PC2);
			valueB2 += readADC(PC3);	
		}
		valueR1= valueR1/i;
		valueB1= valueB1/i;		
		valueR2= valueR2/i;
		valueB2= valueB2/i;
		
		uint8_t rColor;
		uint8_t lColor;
		
		if(valueR1 > RedThreshold && valueB1 < BlueThreshold) rColor = 1;
		else if(valueR1 < RedThreshold && valueB1 > BlueThreshold) rColor = 0;
		
		if(valueR2 > RedThreshold && valueB2 < BlueThreshold) lColor = 1;
		else if(valueR2 < RedThreshold && valueB2 > BlueThreshold) lColor = 0;
		
		if(rColor == color)
		{
			PORTB |= (1<<4);
			moveServo1(1000,2000,1000);
		}
		if(lColor == color)
		{
			PORTB |= (1<<5);
			moveServo2(2000,1000,1000);			
		}
	}
	

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
