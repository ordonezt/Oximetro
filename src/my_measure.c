/*
 * my_measure.c
 *
 *  Created on: Dec 13, 2019
 *      Author: jreiszer
 */
#include "my_include.h"

void Calculate(pulse_t *pulse) {
	bpm = calculateBPM(pulse->Delta);
	//spo2 = calculateSpO2(pulse[RED], pulse[IR]);
}

uint8_t calculateBPM(uint8_t deltaN)
{
	static uint8_t memory[NUMBER_OF_BPMS];
	static uint8_t index = 0, count = 0, prom = 0;
	uint8_t new;
	float abs;

	new = 60000 / (deltaN * SAMPLE_PERIOD);

	if(count < NUMBER_OF_BPMS){
		memory[index] = new;
		index++;
		index %= NUMBER_OF_BPMS;
		count++;
		return 0;
	}

	if(new > MAX_BPM_ACEPTED && new < MIN_BPM_ACEPTED)
		return prom;

	//abs = new > prom? new-prom: prom-new;
	//if(abs < 20) {
			memory[index] = new;
			index++;
			index %= NUMBER_OF_BPMS;
	//}

	prom = Average(memory, NUMBER_OF_BPMS);
	return prom;
}

uint8_t calculateSpO2(pulse_t pulseRed, pulse_t pulseIr)
{
	uint8_t i = 0;

#if(PROM == PROM_R)

	float R = 0;
	uint8_t SpO2 = 0;

	for(i = 0; i < N_PROM; i++)
		R += log10(pulseRed.Max[i] / pulseRed.Min[i]) / log10(pulseIr.Max[i] / pulseIr.Min[i]);

	R = (R / N_PROM);

	SpO2 = A - (B * R);

#endif

#if(PROM == PROM_SPO2)

	float R = 0;
	uint16_t SpO2 = 0;

		for(i = 0; i < SAMPLES_LENGTH; i++)
	{
		R = log10(MAX[RED][i] / MIN[RED][i]) / log10(MAX[IR][i] / MIN[IR][i]);
		SpO2 += A - (B * R);
	}

	SpO2 /= SAMPLES_LENGTH;
#endif

	return SpO2;
}

uint8_t Average(uint8_t *array, uint8_t len){
	uint8_t i;
	uint16_t sum = 0;

	for(i=0; i<len; i++)
		sum += array[i];

	return sum/len;
}
