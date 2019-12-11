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

	Chip_Clock_SetPCLKDiv(SYSCTL_PCLK_ADC,SYSCTL_CLKDIV_8);
	Chip_ADC_Init(LPC_ADC, &ADCSetup);

	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH0, ENABLE);
	Chip_ADC_Int_SetChannelCmd(LPC_ADC, ADC_CH0, ENABLE);

	Chip_ADC_SetSampleRate(LPC_ADC, &ADCSetup, SAMPLE_RATE_HZ);

//	LPC_ADC->CR = LPC_ADC->CR & (~ADC_SAMPLE_RATE_CONFIG_MASK);
//	LPC_ADC->CR |= ADC_CR_CLKDIV(737);

	Chip_ADC_SetBurstCmd(LPC_ADC, ENABLE);

	/* Enable ADC Interrupt */

	NVIC_EnableIRQ(ADC_IRQn);
}

void ADC_IRQHandler(void){
	uint16_t data;
	static uint8_t count = 0;

	Chip_ADC_ReadValue(LPC_ADC, ADC_CH0, &data);

	if(count){
		flags.adc_buffer_error = !RingBuffer_Insert(&RingBuffADC[led], &data);

		toggleLed();
	}
	count = !count;
}
