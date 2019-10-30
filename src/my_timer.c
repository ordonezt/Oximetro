/*
 * my_timer.c
 *
 *  Created on: 4 oct. 2019
 *      Author: ordonezt
 */


#include "my_include.h"

void initTimer(void)
{
	//Inicializacion del SysTick
	SysTick_Config(SystemCoreClock / TICKRATE_HZ1); //Comprobar
}

void SysTick_Handler(void)
{
	 static uint16_t n = 0;

	 debounce();					//1 ms

//	 if(n == 10000)					//20 seg
//		 flags.bits.sleep_time = 1;

	 if(n%5 == 0)					//5 ms
		 Chip_ADC_SetStartMode(LPC_ADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);		//Start conversion

	 if(n%40 == 0)
		 updateDisplay();	//40 ms

	 if(n == 1000) {
		if (!isFinger()) {	//1 seg
			flags.no_finger_times++;
		}
	 }

	 n %= 10000;
	 n++;
}
