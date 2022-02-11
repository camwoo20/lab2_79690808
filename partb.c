/*
 * GccApplication1.c
 *
 * Created: 2/4/2022 11:27:12 AM
 * Author : camwoo
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL

ISR(TIMER1_CAPT_vect)
{
	PORTB ^= (1<<PORTB5); //Toggle PB5
	if (TCCR1B & (1<<ICES1)) //if the interrupt is set to look for a rising edge make it look for falling edge
	{
		TCCR1B &= ~(1<<ICES1); //looks for a falling edge
	}
	else {
		TCCR1B |= (1<<ICES1);
	}
}

void Initialize()
{
	cli();
	DDRB &= ~(1<<DDB0); //set pin PB0 to interrupt
	DDRB |= (1<<DDB5); //set PB5 (Built in LED) to output
	
	//looks for a falling edge
	TCCR1B &= ~(1<<ICES1);
	//set up clock
	TCCR1B &= ~(1<<CS10);
	TCCR1B |= (1<<CS11);
	TCCR1B &= ~(1<<CS12);
	
	//clear interrupt flag
	TIFR1 |= (1<<ICF1);
	
	//enable input capture interrupt
	TIMSK1 |= (1<<ICIE1);
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
