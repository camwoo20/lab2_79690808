/*
 * GccApplication1.c
 *
 * Created: 2/4/2022 11:27:12 AM
 * Author : camwoo
 */ 
#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include "inc/uart.h"
#include <string.h>


char String[25];
char String1[25];
char String2[25];
char morseCode[5];
volatile int morseIndex = 0;

volatile int foundSpace = 1;
volatile int numOverflow = 0;
volatile const float prescaler = 1024.0;
volatile const float clockFreqMhz = 16.0;
volatile unsigned int maxTickCount = 65535;
volatile float timePerTick = 0.0;

volatile float elapsedTime1 = 0.0;
volatile int etime1 = 0;

volatile int isButtonPressed = 0;
volatile int pulseLength = 0;
volatile int spacePresent = 0;
volatile char ch;

volatile int buttonPressOverflow = 0;

void decodeMorse(void) {
	
	morseCode[morseIndex] = '\0';
	sprintf(String, "-----computing input----\n");
	UART_putstring(String);
	sprintf(String, morseCode);
	UART_putstring(morseCode);
	sprintf(String, "\n");
	UART_putstring(String);
	
	//begin decode
	if (morseCode[0] == '.') {
		if (morseCode[1] == '\0') {
			sprintf(String, "E");
			UART_putstring(String);
			} else if (morseCode[1] == '.') {
			if (morseCode[2] == '\0') {
				sprintf(String, "I");
				UART_putstring(String);
				} else if (morseCode[2] == '.') {
				if (morseCode[3] == '\0') {
					sprintf(String, "S");
					UART_putstring(String);
					} else if (morseCode[3] == '.') {
					if (morseCode[4] == '\0') {
						sprintf(String, "H");
						UART_putstring(String);
						} else if (morseCode[4] == '.') {
						if (morseCode[5] == '\0') {
							sprintf(String, "5");
							UART_putstring(String);
							} else {
							sprintf(String, "Morse code input error\n");
							UART_putstring(String);
						}
						} else if (morseCode[4] == '-') {
						if (morseCode[5] == '\0') {
							sprintf(String, "4");
							UART_putstring(String);
							} else {
							sprintf(String, "Morse code input error\n");
							UART_putstring(String);
						}
					}
					} else if (morseCode[3] == '-') {
					if (morseCode[4] == '\0') {
						sprintf(String, "V");
						UART_putstring(String);
						} else if (morseCode[4] == '-') {
						if (morseCode[5] == '\0') {
							sprintf(String, "3");
							UART_putstring(String);
							} else {
							sprintf(String, "Morse code input error\n");
							UART_putstring(String);
						}
					}
				}
				} else if (morseCode[2] == '-') {
				if (morseCode[3] == '\0') {
					sprintf(String, "U");
					UART_putstring(String);
					} else if (morseCode[3] == '.') {
					if (morseCode[4] == '\0') {
						sprintf(String, "F");
						UART_putstring(String);
					}
					} else if (morseCode[3] == '-') {
					if (morseCode[4] == '-') {
						if (morseCode[5] == '\0') {
							sprintf(String, "2");
							UART_putstring(String);
						}
					}
				}
			}
			} else if (morseCode[1] == '-') {
			if (morseCode[2] == '\0') {
				sprintf(String, "A");
				UART_putstring(String);
				} else if (morseCode[2] == '.') {
				if (morseCode[3] == '\0') {
					sprintf(String, "R");
					UART_putstring(String);
					} else if (morseCode[3] == '.') {
					if (morseCode[4] == '\0') {
						sprintf(String, "L");
						UART_putstring(String);
					}
				}
				} else if (morseCode[2] == '-') {
				if (morseCode[3] == '\0') {
					sprintf(String, "W");
					UART_putstring(String);
					} else if (morseCode[3] == '.') {
					if (morseCode[4] == '\0') {
						sprintf(String, "P");
						UART_putstring(String);
					}
					} else if (morseCode[3] == '-') {
					if (morseCode[4] == '\0') {
						sprintf(String, "J");
						UART_putstring(String);
						} else if (morseCode[4] == '-')  {
						if (morseCode[5] == '\0')  {
							sprintf(String, "1");
							UART_putstring(String);
						}
					}
				}
			}
		}
		} else if (morseCode[0] == '-') {
		if (morseCode[1] == '\0') {
			sprintf(String, "T");
			UART_putstring(String);
			} else if (morseCode[1] == '.') {
			if (morseCode[2] == '\0') {
				sprintf(String, "N");
				UART_putstring(String);
				} else if (morseCode[2] == '.') {
				if (morseCode[3] == '\0') {
					sprintf(String, "D");
					UART_putstring(String);
					} else if (morseCode[3] == '.') {
					if (morseCode[4] == '\0') {
						sprintf(String, "B");
						UART_putstring(String);
						} else if (morseCode[4] == '.') {
						if (morseCode[5] == '\0') {
							sprintf(String, "6");
							UART_putstring(String);
							} else {
							sprintf(String, "Morse code input error\n");
							UART_putstring(String);
						}
					}
					} else if (morseCode[3] == '-') {
					if (morseCode[4] == '\0') {
						sprintf(String, "X");
						UART_putstring(String);
					}
		
				}
				} else if (morseCode[2] == '-') {
				if (morseCode[3] == '\0') {
					sprintf(String, "K");
					UART_putstring(String);
					} else if (morseCode[3] == '.') {
					if (morseCode[4] == '\0') {
						sprintf(String, "C");
						UART_putstring(String);
					}
					} else if (morseCode[3] == '-') {
					if (morseCode[4] == '\0') {
						sprintf(String, "Y");
						UART_putstring(String);
					}
				}
			}
			} else if (morseCode[1] == '-') {
			if (morseCode[2] == '\0') {
				sprintf(String, "M");
				UART_putstring(String);
				} else if (morseCode[2] == '.') {
				if (morseCode[3] == '\0') {
					sprintf(String, "G");
					UART_putstring(String);
					} else if (morseCode[3] == '.') {
					if (morseCode[4] == '\0') {
						sprintf(String, "Z");
						UART_putstring(String);
					}
					} else if (morseCode[3] == '-') {
					if (morseCode[4] == '\0') {
						sprintf(String, "Q");
						UART_putstring(String);
					}
				}
				} else if (morseCode[2] == '-')  {
				if (morseCode[3] == '\0') {
					sprintf(String, "O");
					UART_putstring(String);
					} else if (morseCode[3] == '.') {
					if (morseCode[4] == '.') {
						if (morseCode[5] == '\0') {
							sprintf(String, "8");
							UART_putstring(String);
						}
					}
					} else if (morseCode[3] == '-')  {
					if (morseCode[4] == '.')  {
						
						if (morseCode[5] == '\0') {
							sprintf(String, "9");
							UART_putstring(String);
						}
						} else if (morseCode[4] == '-') {
						if (morseCode[5] == '\0')  {
							sprintf(String, "0");
							UART_putstring(String);
						}
					}
				}
			}
		}
		} else {
		sprintf(String, "no first char detected\n");
		UART_putstring(String);
	}
	
	
	for(int i = 0; i < 10; i++) {
		morseCode[i] = '\0';
	}
	morseIndex = 0;
}



float convertWidthToTime(unsigned int pulseWidth)
{
	return pulseWidth * timePerTick;
}

void dotOrDash(int pulseTimeMS)
{
	if (buttonPressOverflow) {
		 ch = '\0';
	}
	else if (pulseTimeMS > 200 && pulseTimeMS < 900) {
		ch = '-';
		morseCode[morseIndex] = ch;
		morseIndex++;
		PORTB |= (1<<PORTB1);
		_delay_ms(50);
		PORTB &= ~(1<<PORTB1);
	}
	else if (pulseTimeMS > 30 && pulseTimeMS <= 200) {
		ch = '.';
		morseCode[morseIndex] = ch;
		morseIndex++;
		PORTB |= (1<<PORTB2);
		_delay_ms(50);
		PORTB &= ~(1<<PORTB2);
	}
	else {
		ch = '\0';
	}
	sprintf(String, "%c", ch);
	UART_putstring(String);
}


ISR(TIMER1_COMPA_vect)
{
	if (isButtonPressed) {
		buttonPressOverflow = 1;
	} else {
		if (!spacePresent) {
			spacePresent = 1;
			sprintf(String, "%c", ' '); //if take too long prints a space
			UART_putstring(String);
			decodeMorse();
		}
	}
}

ISR(TIMER1_CAPT_vect)
{
	PORTB ^= (1<<PORTB5); //Toggle PB5
	if (!isButtonPressed) {
		isButtonPressed = 1;
		TCNT1 = 0;
		buttonPressOverflow = 0;
		spacePresent = 0;
	} else {
		isButtonPressed = 0;
		pulseLength = TCNT1;
		elapsedTime1 = convertWidthToTime(pulseLength);
		etime1 = (int) elapsedTime1;
		dotOrDash(etime1); // prints the dots or dashes
	}
 	TCCR1B ^= (1<<ICES1); //toggle the falling / rising edge detection
}



void Initialize()
{
	cli();
	UART_init(BAUD_PRESCALER); 
	
	timePerTick = 1.0 / (clockFreqMhz * 1000000.0) * prescaler * 1000;
	
	DDRB &= ~(1<<DDB0); //set pin PB0 to interrupt
	DDRB |= (1<<DDB5); //set PB5 (Built in LED) to output
	
	//looks for a falling edge
	TCCR1B &= ~(1<<ICES1);
	//prescale clock by 1024
	TCCR1B |= (1<<CS10);
	TCCR1B &= ~(1<<CS11);
	TCCR1B |= (1<<CS12);
	
	//clear interrupt flag
	TIFR1 |= (1<<ICF1);
	
	//enable input capture interrupt
	TIMSK1 |= (1<<ICIE1);
	//enable OCIE1A
	TIMSK1 |= (1<<OCIE1A);
	
	OCR1A = 9375; //9375 ticks in 600 ms
	sei();
	
}


int main(void)
{
    /* Replace with your application code */
    Initialize();
	while (1) 
    {
    }
}
