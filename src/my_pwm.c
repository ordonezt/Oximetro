/*
 * my_pwm.c
 *
 *  Created on: 8 oct. 2019
 *      Author: ordonezt
 */

#include "my_include.h"

void initPWM(void)
{
	//Uso timer 2

	//OC2A -> PB3
	//OC2B -> PD3
	DDRB |= 0x01 << DDB3;		//Pin B3 salida
	DDRD |= 0x01 << DDD3;		//Pin D3 salida

	TCCR2A = 0b10100011; 		//Pone en low OC2A cuando llega al match, lo mismo con OC2B, modo fast PWM
	TCCR2B = 0b00000001; 		//Pwm mode con limite en 0xFF, sin preescaler =>(1MHz / 256) = 3,9KHz

	OCR2A = 0;		//Arranco desde duty = 0
	OCR2B = 0;

	TIMSK2 = 0;				//No interrumpo por nada
}
