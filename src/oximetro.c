/*
 * oximetro.c
 *
 *	Es una aplicacion de 2 estados, normal y sleep
 *	en normal blinkea un led y prende otro indicando que esta encendido y en sleep se va a dormir y apaga todo
 *	mediante un boton entramos y salimos del estado sleep
 *
 *  Created on: 4 oct. 2019
 *      Author: ordonezt
 */
#ifndef F_CPU					// if F_CPU was not defined in Project -> Properties
#define F_CPU 1000000UL			// define it now as 1 MHz unsigned long
#endif

#define TEST1 (1)
#define TEST2 (2)
#define TEST3 (3)

#define TEST TEST2



#include "my_include.h"

volatile flags_t flags = {0};

#if(TEST == TEST1)

int main(void)
{
	statePwr_t state = NORMAL;

	initTimer();	//Inicializo el timer cada 1 ms
	initGpio();		//Inicializo entradas y salidas

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);//Selecciono power-down mode

	sei();	//Habilito las interrupciones

	while(1)
	{
		switch(state)
		{
			case NORMAL:

				if(flags.bits.led_flag)
				{
					flags.bits.led_flag = 0;
					PORTD ^= 0x01 << PORTD3;
				}

				if(button.wasRelease)
				{
					button.wasRelease = false;
					flags.bits.sleep_time = 1;
				}

				if(flags.bits.sleep_time)
				{
					flags.bits.sleep_time = 0;
					state = SLEEP;

					goToSleep();
				}

				break;

			case SLEEP:
				//Si llegue aca es por que desperte, voy a estado normal

				if(button.wasRelease == true) //Espero a que suelte el boton aca, por que si no vuelvo a dormir
				{
					button.wasRelease = false;
					state = NORMAL;
				}

				break;

			default:
				state = NORMAL;
		}

		if(flags.bits.debounce_time)
		{
			flags.bits.debounce_time = 0;
			debounce();
		}

	}
}
#endif


/*
 * Conectar un led a PB3, PD3 y PB0
 * Conectar potenciometro entre Vcc y GND y el punto del medio a PC4
 */
#if(TEST == TEST2)
int main(void)
{
	initTimer();
	initPWM();
	initADC();
	DDRB |= 0x01 << DDB0;		//Pin B3 salida

	sei();	//Habilito las interrupciones

	while(1)
	{

		if(flags.bits.adc_time)
		{
			flags.bits.adc_time = 0;
			ADCSRA |= 1 << ADSC;	//Start conversion
		}

		if(flags.bits.show_time)
		{
			flags.bits.show_time = 0;
//			calculateBPM();
//			calculateSpO2();
//			updateScreen();

		}
		if(flags.bits.debounce_time)
		{
			if(ADC_buff[RED][20] > 512)	//Tarda mucho en reflejarse por que se tardan 2 seg en actualizar ADC_buff[RED][20]
			{
				if(OCR2A + 1 < 255)
					OCR2A += 1;
				if(OCR2B - 1 > 0)
					OCR2B -= 1;
			}
			if(ADC_buff[RED][20] < 512)
			{
				if(OCR2B + 1 < 255)
					OCR2B += 1;
				if(OCR2A - 1 > 0)
					OCR2A -= 1;
			}
		}

		if(flags.bits.check_finger_time)
		{
			static uint8_t no_finger_time = 0;	//En segundos

			if(isFinger() == false)
				no_finger_time++;
			else
				no_finger_time = 0;

			if(no_finger_time >= 10)
			{
				//goToSleep();
				PORTB |= 0x01 << PORTB0;
				//si llego aca es por que desperte
				//no_finger_time = 0;
			}
			else
				PORTB &= ~(0x01 << PORTB0);

		}
	}
	return 0;
}
#endif


#if(TEST == TEST3)

int main(void)
{
	statePwr_t state = NORMAL;

	initTimer();	//Inicializo el timer cada 1 ms
	initGpio();		//Inicializo entradas y salidas

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);//Selecciono power-down mode

	sei();	//Habilito las interrupciones

	while(1)
	{
		switch(state)
		{
			case NORMAL:

				if(flags.bits.led_flag)
				{
					flags.bits.led_flag = 0;
					PORTD ^= 0x01 << PORTD3;
				}

				if(button.wasRelease)
				{
					button.wasRelease = false;
					flags.bits.sleep_time = 1;
				}

				if(flags.bits.sleep_time)
				{
					flags.bits.sleep_time = 0;
					state = SLEEP;

					goToSleep();
				}

				break;

			case SLEEP:
				//Si llegue aca es por que desperte, voy a estado normal

				if(button.wasRelease == true) //Espero a que suelte el boton aca, por que si no vuelvo a dormir
				{
					button.wasRelease = false;
					state = NORMAL;
				}

				break;

			default:
				state = NORMAL;
		}

		if(flags.bits.debounce_time)
		{
			flags.bits.debounce_time = 0;
			debounce();
		}

		if(flags.bits.show_time)
		{
			flags.bits.show_time = 0;
//			calculateBPM();
//			calculateSpO2();
//			updateScreen();

		}
	}
}
#endif
