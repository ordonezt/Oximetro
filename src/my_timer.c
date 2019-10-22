/*
 * my_timer.c
 *
 *  Created on: 4 oct. 2019
 *      Author: ordonezt
 */


#include "my_include.h"

void initTimer(void)
{
	 //Interumpe cada 1 ms
	TCCR0A = 0b00000000; //Funcionamiento normal
	TCCR0B = 0b00000010; //Funcionamiento normal prescaler 8
	OCR0A = 125;		 //Cada 1 ms
	TIMSK0 = 0b00000010; //Interrupcion Compare Match A
}

ISR(TIMER0_COMPA_vect)
{
	 static uint16_t n = 0;

	 debounce();					//1 ms

//	 if(n == 10000)					//20 seg
//		 flags.bits.sleep_time = 1;

	 if(n%5 == 0)					//5 ms
		 ADCSRA |= 1 << ADSC;		//Start conversion

	 if(n%40 == 0)
		 UpdateDisplay();	//40 ms

	 if(n == 1000) {
		if (!IsFinger()) {	//1 seg
			flags.no_finger_times++;
		}
	 }

	 n %= 10000;
	 n++;
}
