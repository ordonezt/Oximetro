/*
 * my_measure.h
 *
 *  Created on: Dec 13, 2019
 *      Author: jreiszer
 */

#define PROM_R 0
#define PROM_SPO2 1
#define PROM PROM_R
#define NUMBER_OF_BPMS 10
#define MAX_BPM_ACEPTED 300
#define MIN_BPM_ACEPTED 20

void Calculate(pulse_t*);
uint8_t calculateBPM(uint8_t);
uint8_t calculateSpO2(pulse_t pulseRed, pulse_t pulseIr);
uint8_t Average(uint8_t*, uint8_t);

extern float bpm;
extern float spo2;
