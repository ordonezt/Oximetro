/*
 * my_adc.c
 *
 *  Created on: 7 oct. 2019
 *      Author: ordonezt
 */


#include "my_include.h"

float ADC_Buffer[BUFFER_HEIGHT][BUFFER_ADC];
int In_Index[BUFFER_HEIGHT], Out_Index[BUFFER_HEIGHT];

extern volatile led_t led;

void initADC(void)
{
	static ADC_CLOCK_SETUP_T ADCSetup;

	Chip_IOCON_PinMux(LPC_IOCON, ADC_PORT, ADC_PIN, IOCON_MODE_INACT, IOCON_FUNC1);

	Chip_ADC_Init(LPC_ADC, &ADCSetup);

	/* Enable ADC Interrupt */
	NVIC_EnableIRQ(ADC_IRQn);

	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH0, ENABLE);
	Chip_ADC_Int_SetChannelCmd(LPC_ADC, ADC_CH0, ENABLE);
}

void ADC_IRQHandler(void){
	uint16_t data;

	Chip_ADC_ReadValue(LPC_ADC, ADC_CH0, &data);

	//Solo entro si ya lei el dato anterior
//	if(!flags.conversion_done)
//	{
//		flags.conversion_done = true;

	ADC_Buffer[led][In_Index[led]] = data;
	In_Index[led]++;
	In_Index[led] %= BUFFER_LARGE;

//	}
}
