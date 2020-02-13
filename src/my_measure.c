/*
 * my_measure.c
 *
 *  Created on: Dec 13, 2019
 *      Author: jreiszer
 */
#include "my_include.h"

float bpmVector[BPM_WINDOW] = {0};

//void Calculate(pulse_t *pulse) {
//	bpm = calculateBPM(pulse[RED].Delta);
//	spo2 = calculateSpO2(pulse[IR], pulse[RED]);
//}

void Calculate (void) {
	uint8_t i, count = 0;
	float tmp;

	static float sum = 0;


	for (i = 0; i < BPM_WINDOW; i++) {
		tmp = bpmVector[i];
		if (tmp != 0) {
			sum += tmp;
			count++;
		}
	}

	memset(bpmVector, 0, sizeof(*bpmVector) * BPM_WINDOW);

	sum /= count;

	bpm = (uint32_t)((sum * 10 + 0.5) / 10 + 0.5);
	sum = 0;
}

void stackMeasure (pulse_t *pulse) {
	static uint8_t i = 0;

	bpmVector[i] = 	60000 / (float)(pulse[RED].Delta * SAMPLE_PERIOD);

	i++;
	i %= BPM_WINDOW;
}
uint32_t calculateBPM(uint32_t deltaN)
{
	static uint32_t memory[NUMBER_OF_BPMS];
	static uint8_t index = 0, count = 0;
	uint32_t new, prom = 0;
	float abs;

	new = ((60000 / (float)(deltaN * SAMPLE_PERIOD)) * 10 + 0.5) / 10 + 0.5;

//	if(count < NUMBER_OF_BPMS){
//		memory[index] = new;
//		index++;
//		index %= NUMBER_OF_BPMS;
//		count++;
//		return 0;
//	}
//
//	if(new > MAX_BPM_ACEPTED && new < MIN_BPM_ACEPTED)
//		return prom;
//
	memory[index] = new;
	index++;
	index %= NUMBER_OF_BPMS;

	prom = Average(memory, NUMBER_OF_BPMS);
	return prom;
}

uint8_t calculateSpO2(pulse_t pulseRed, pulse_t pulseIr)
{
	uint8_t i = 0;

#if(PROM == PROM_R)

	float R = 0;
	float sum = 0;
	uint8_t SpO2 = 0, cont=0;

	for(i = 0; i < N_PROM; i++){
		R = log10(pulseRed.Max[i] / pulseRed.Min[i]) / log10(pulseIr.Max[i] / pulseIr.Min[i]);
		if(R > 0.4 && R < 0.8){
			sum += R;
			cont++;
		}
	}

	R = (sum / cont);

	SpO2 = 110 - (25 * R);

#endif

#if(PROM == PROM_SPO2)

	float R = 0;
	uint16_t SpO2 = 0;

		for(i = 0; i < SAMPLES_LENGTH; i++)
	{
		R = log10(MAX[IR][i] / MIN[IR][i]) / log10(MAX[RED][i] / MIN[RED][i]);
		SpO2 += A - (B * R);
	}

	SpO2 /= SAMPLES_LENGTH;
#endif

	return SpO2;
}

uint32_t Average(uint32_t *array, uint8_t len){
	uint8_t i;
	uint32_t sum = 0;

	for(i=0; i<len; i++)
		sum += array[i];

	return sum/len;
}
