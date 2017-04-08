// Quick Demo of light sensor

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include "pinDefines.h"
#include "USART.h"
#include "USART.c"

// -------- Functions --------- //
static inline void initADC0(void) {
  ADMUX |= (1 << REFS0); //AVCC used	//|(1 << REFS1);                  /* reference voltage on internal 1.1V */
  ADMUX |= (0b00001111 & PC0); 
  ADCSRA |= (1 << ADPS0)|(1<<ADPS1);                   /* ADC clock prescaler /8 */
  
  ADMUX |= (1 << ADLAR);
  
  ADCSRA |= (1 << ADEN);                                 /* enable ADC */
  ADCSRA |= (1 << ADATE); //auto-trigger enable
  ADCSRA |= (1 << ADSC); //start
}

int main(void) {

  // -------- Inits --------- //
  uint8_t adcValue;
  uint8_t i;
  uint16_t sum = 0;
  initUSART();
  initADC0();

  // ------ Event loop ------ //
  while (1) {
	
/* 	for(i=0;i<100;i++){
		adcValue=ADCH;
	//	while(adcValue==255) adcValue=ADCH;
			sum+= adcValue;	
		_delay_ms(5);
		
	} */
	//printByte(sum/10);                                     /* read ADC in */
	adcValue=ADCH;
	printByte(adcValue);
	printString("\n");
	_delay_ms(100);
  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
