/*
* Sonar Headset with HC-SR04
* (C)2012 Radu Motisan, www.pocketmagic.net
*
* Based on http://www.wzona.info/2012/09/echolokacija-naudojant-hc-sr04.html sonar implementation
*/

#define F_CPU 16000000
#include "lcd/hd44780.h"
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#define SONAR_TRIGGER_DDR DDRC
#define SONAR_TRIGGER_PORT PORTC
#define SONAR_TRIGGER_PIN PINC
//--define pin where LED sensor is connected
#define SONAR_TRIGGER_DQ 0	//Pc0
//--
#define SONAR_TRIGGER_INPUT_MODE() SONAR_TRIGGER_DDR&=~(1<<SONAR_TRIGGER_DQ)
#define SONAR_TRIGGER_OUTPUT_MODE() SONAR_TRIGGER_DDR|=(1<<SONAR_TRIGGER_DQ)
#define SONAR_TRIGGER_LOW() SONAR_TRIGGER_PORT&=~(1<<SONAR_TRIGGER_DQ)
#define SONAR_TRIGGER_HIGH() SONAR_TRIGGER_PORT|=(1<<SONAR_TRIGGER_DQ)

#define INSTR_PER_US 16						// instructions per microsecond (depends on MCU clock, 16MHz current)
#define INSTR_PER_MS 16000					// instructions per millisecond (depends on MCU clock, 16MHz current)
#define MAX_RESP_TIME_MS 200				// timeout - max time to wait for low voltage drop (higher value increases measuring distance at the price of slower sampling)
#define DELAY_BETWEEN_TESTS_MS 50			// echo cancelling time between sampling

volatile unsigned long result = 0;
volatile unsigned char up = 0;
volatile unsigned char running = 0;
volatile uint32_t timerCounter = 0;

// timer overflow interrupt, each time when timer value passes 255 value
SIGNAL(TIMER0_OVF_vect)
{
        if (up) {       // voltage rise was detected previously
        
                timerCounter++; // count the number of overflows
                
                // dont wait too long for the sonar end response, stop if time for measuring the distance exceeded limits
                uint32_t ticks = timerCounter * 256 + TCNT0;
                uint32_t max_ticks = (uint32_t)MAX_RESP_TIME_MS * INSTR_PER_MS; // this could be replaced with a value instead of multiplying
                if (ticks > max_ticks) {
                        // timeout
                        up = 0;          // stop counting timer values
                        running = 0; // ultrasound scan done
                        result = -1; // show that measurement failed with a timeout (could return max distance here if needed)
                }
        }
}

// interrupt for INT1 pin, to detect high/low voltage changes
/** 
        We assume, that high voltage rise comes before low drop and not vice versa - 
        however this should be implemented more correctly using both interrupts INT0/INT1,
        (i.e. INT0 configured for high rise, and INT1 - for low rise, thus the code must be separated also)     
*/
SIGNAL(INT1_vect) 
{
        if (running) { //accept interrupts only when sonar was started
        
                if (up == 0) { // voltage rise, start time measurement
                        up = 1;
                        timerCounter = 0;
                        TCNT0 = 0; // reset timer counter
                } else {
                        // voltage drop, stop time measurement
                        up = 0;
                        // convert from time to distance(millimeters): d = [ time_s * 340m/s ] / 2 = time_us/58
                        result = (timerCounter * 256 + TCNT0) / 58; 
                        running = 0;
                }
        }
}

/**
        Sonar interfacing:
                1. Send high impulse to Trig input for minimum 10us
                2. Sonar automatically sends eight 40kHz inpulses
                3. Sonar rises high on Echo output and then after some time drops 
                   output to low (can take a while on long distances! - must include timeouts)
                4. Based on output time difference deltaT = lowT-highT calculate: 
                        distance = [ deltaT * sound_speed(340m/s) ] / 2
                5. Make a delay before starting the next cycle to compensate for late echoes
*/

// generate an impulse for the Trig input (starts the sonar)
void sonar() {

        //PORTB = 0x00; // clear to zero for 1 us
		SONAR_TRIGGER_LOW();
        _delay_us(1);
                
        //PORTB = 0x01; // set high for 10us
		SONAR_TRIGGER_HIGH();
        running = 1;  // sonar launched
        _delay_us(10);
        
		SONAR_TRIGGER_LOW();
        //PORTB = 0x00; // clear  
}

int duty = 100;
int main(void)
{
		lcd_init();
		lcd_clrscr();
        // ------------------- ultrasonic init code --------------------
        SONAR_TRIGGER_OUTPUT_MODE();
		
		//  1.CONFIGURE INTERRUPT INT1
        // turn on interrupts for INT1, connect Echo to INT1
        MCUCR |= (0 << ISC11) | (1 << ISC10); // enable interrupt on any(rising/droping) edge
        GICR |= (1 << INT1);      // Turns on INT1
        
		// 2.CREATE Timer T0 to count seconds
        // setup 8 bit timer & enable interrupts, timer increments to 255 and interrupts on overflow
        TCCR0 = (0<<CS02)|(0<<CS01)|(1<<CS00); // select internal clock with no prescaling
        TCNT0 = 0; // reset counter to zero
        TIMSK = 1<<TOIE0; // enable timer interrupt
 
        sei(); // enable all(global) interrupts
        
		// 3.
		// 3.CREATE PWM with Timer T1 
		// Connect headset to PB1!
		TCCR1A = 0;     // disable all PWM on Timer1 whilst we set it up
		// set the frequency FCPU/(ICR1 * PRESCALLING) Hz . Prescalling is 8x here.
		// ICR1 = 50  ---> 40KHz
		// ICR1 = 100 ---> 20KHz
		// ICR1 = 200  --> 10KHz
		// ICR1 = 1000 --> 2KHz
		// ICR1 = 10000 -> 200Hz
		// ICR1 = 20000 -> 100Hz
		// ICR1 = 200000 > 10Hz
		// prescalling 1x:
		// ICR1 = 100 ---> 160KHz
		// ICR1 = 400 --->  40KHz
		// ICR1 = 800 --->  20KHz
		// ICR1 = 1600 -->  10KHz
		// ICR1 = 3200 -->  5KHz
		ICR1 = 200000;  // 20KHz 74
		// Configure timer 1 for Fast PWM mode via ICR1, with no prescaling
		TCCR1B = (1 << WGM13) | (1<<WGM12) | (1 << CS11); //CS11 = 8x prescalling , CS10 = 1x
		// Set PB1 a as outputs
		DDRB |= (1 << DDB1);
		TCCR1A = (1 << WGM11) | (1 << COM1A1) | (1<< CS11); // set none-inverting mode and start PWM
		OCR1A = (int)((float)ICR1 * (float)duty/1000.0); //50% duty


        int numDelays = 0;
        
    for(;;){  /* main event loop */
        
                //TODO: do some other stuff here if needed
				lcd_home();
				lcd_string_format("Distance: %4.0dmm   ", result);
				//2500 max .. 50mm min
				if (result < 2500) {
					
					// ADJUST THE PWM TO CHANGE THE SOUND PRODUCED
					ICR1 = result * 100;//1000 + (5000 * (cm * 100)/250);
					OCR1A = (int)((float)ICR1 * (float)duty/1000.0); //50% duty
				}					

                if (running == 0) { // launch only when next iteration can happen
                
                        // configurable delay count
                        _delay_ms(1);
                        numDelays++;
                
                        // create a delay between tests, to compensate for old echoes
                        if (numDelays > DELAY_BETWEEN_TESTS_MS) 
						{ 
                                sonar(); // launch ultrasound measurement!
                                numDelays = 0;
                        }
                }
    }
}

