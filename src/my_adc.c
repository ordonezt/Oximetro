/*
 * my_adc.c
 *
 *  Created on: 7 oct. 2019
 *      Author: ordonezt
 */


#include "my_include.h"

uint16_t ADC_Buffer[BUFFER_HEIGHT][BUFFER_LENGTH] = {0};

void initADC(void)
{
	static ADC_CLOCK_SETUP_T ADCSetup;

	RingBuffer_Init(&RingBuffADC[RED], &ADC_Buffer[RED], sizeof(ADC_Buffer[RED][0]), BUFFER_LENGTH);
	RingBuffer_Init(&RingBuffADC[IR], &ADC_Buffer[IR], sizeof(ADC_Buffer[IR][0]), BUFFER_LENGTH);

	Chip_IOCON_PinMux(LPC_IOCON, ADC_PORT, ADC_PIN, IOCON_MODE_INACT, IOCON_FUNC1);

	Chip_ADC_Init(LPC_ADC, &ADCSetup);

	/* Enable ADC Interrupt */
	NVIC_EnableIRQ(ADC_IRQn);

	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH0, ENABLE);
	Chip_ADC_Int_SetChannelCmd(LPC_ADC, ADC_CH0, ENABLE);

	Chip_ADC_SetSampleRate(LPC_ADC, &ADCSetup, SAMPLE_RATE_HZ);

	Chip_ADC_SetBurstCmd(LPC_ADC, ENABLE);
}

void ADC_IRQHandler(void){
	uint16_t data;

	Chip_ADC_ReadValue(LPC_ADC, ADC_CH0, &data);

	flags.adc_buffer_error = !RingBuffer_Insert(&RingBuffADC[led], &data);

	toggleLed();
}
