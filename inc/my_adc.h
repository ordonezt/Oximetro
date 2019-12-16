/*
 * my_adc.h
 *
 *  Created on: 7 oct. 2019
 *      Author: brousse
 */

#ifndef MY_ADC_H_
#define MY_ADC_H_

#define ADC_PORT	1
#define ADC_PIN		31

#define ADC_CH	ADC_CH5

#define FUNC	IOCON_FUNC3

#define BUFFER_ADC 256

#define SAMPLE_RATE_HZ 985

void initADC(void);

#endif /* MY_ADC_H_ */
