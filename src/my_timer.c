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
	 static uint16_t tick = 0;

	 tick++;

	 debounce();					//1 ms
//	 checkFinger(); Sacar ajuera

	 if(tick%(SAMPLE_PERIOD/2) == 0)					//2 ms
		 Chip_ADC_SetStartMode(LPC_ADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);		//Start conversion

//	 if(n%40 == 0)
//		 updateDisplay();	//40 ms

//	 if(n%1000 == 0){
//		if(!flags.is_finger)	//1 seg
//			flags.no_finger_times++;
//		else
//			flags.no_finger_times = 0;
//	 }

//	 graphic(); //DISPLAY
}
