// Quick Demo of light sensor

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include "pinDefines.h"
#include "USART.h"
#include "USART.c"

// -------- Functions --------- //
static inline void initADC0(void) {
  ADMUX |= (1 << REFS0);//|(1 << REFS1);                  /* reference voltage on internal 1.1V */
  ADCSRA |= (1 << ADPS1)|(1 << ADPS0);     /* ADC clock prescaler /8 */
  ADCSRA |= (1 << ADEN);                                 /* enable ADC */
}

uint16_t readADC(uint8_t channel) {
  ADMUX = (0xf0 & ADMUX) | channel;
  ADCSRA |= (1 << ADSC);
  loop_until_bit_is_clear(ADCSRA, ADSC);
  return (ADC);
}

int main(void) {

  // -------- Inits --------- //
  uint16_t adcValue;
  uint8_t i = 0;
  uint16_t size = 100;
  uint16_t add = 0;
  uint16_t value =0;
  initUSART();
  initADC0();

  // ------ Event loop ------ //
  while (1) {
/*  	if(i>=(size-1))
	{
		printByte(adcValue/10);
		printString("\n");                                    // read ADC in 
		i = 0;
	}
	adcValue += readADC(PC0);  */
/* 	for(i = 0; i<size;i++)
	{
		value=readADC(PC0);
		add+=value;
		printString("\t");
		printByte(value);
		printString("\n");
		_delay_ms(10);
	}
	adcValue = add/size; */
	adcValue = readADC(PC0);
	printByte(adcValue);
	printString("\n");
	_delay_ms(10);
	//i++;
  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
