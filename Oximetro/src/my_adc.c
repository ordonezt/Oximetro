/*
 * my_adc.c
 *
 *  Created on: 7 oct. 2019
 *      Author: brousse
 */


#include "my_include.h"

static ADC_CLOCK_SETUP_T ADCSetup;

void initADC(void)
{
	Chip_IOCON_PinMux(LPC_IOCON, ADC_PORT, ADC_PIN, IOCON_MODE_INACT, IOCON_FUNC1);

	Chip_ADC_Init(LPC_ADC, &ADCSetup);

	/* Enable ADC Interrupt */
	NVIC_EnableIRQ(ADC_IRQn);

	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH0, ENABLE);
	Chip_ADC_Int_SetChannelCmd(LPC_ADC, ADC_CH0, ENABLE);

}

void ADC_IRQHandler(void)
{
	//Solo entro si ya lei el dato anterior
	if(!flags.conversion_done)
	{
		flags.conversion_done = true;

		//get raw signal
		shiftBuffer(raw[led], N_RAW);
		Chip_ADC_ReadValue(LPC_ADC, ADC_CH0, raw[led]);
	}
 }
