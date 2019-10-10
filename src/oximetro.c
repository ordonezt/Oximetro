/*
 * powerDownTest.c
 *
 *	Es una aplicacion de 2 estados, normal y sleep
 *	en normal blinkea un led y prende otro indicando que esta encendido y en sleep se va a dormir y apaga todo
 *	mediante un boton entramos y salimos del estado sleep
 *
 *  Created on: 4 oct. 2019
 *      Author: Tomas
 */

#define TEST1 (1)
#define TEST2 (2)

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



#if(TEST == TEST2)
int main(void)
{
//	stateMain_t state;
//
//	while(1)
//	{
//		switch(state)
//		{
//
//		}
//
//		if(flags.bits.adc_time)
//		{
//
//		}
//
//		if(flags.bits.show_time)
//		{
//			calculateBPM();
//			calculateSpO2();
//			updateScreen();
//		}
//
//		if(flags.bits.watchdog_time)
//		{
//			if(isFinger())
//				feedWatchdog();
//		}
//	}
	return 0;
}
#endif
