/*
 * my_measure.h
 *
 *  Created on: Dec 13, 2019
 *      Author: jreiszer
 */

#define PROM_R 0
#define PROM_SPO2 1
#define PROM PROM_R
#define NUMBER_OF_BPMS 5
#define MAX_BPM_ACEPTED 300
#define MIN_BPM_ACEPTED 20

#define BPM_WINDOW	20

//void Calculate(pulse_t*);
uint32_t calculateBPM(uint32_t);
uint8_t calculateSpO2(pulse_t pulseRed, pulse_t pulseIr);
uint32_t Average(uint32_t*, uint8_t);

void stackMeasure (pulse_t *pulse);
void Calculate (void);

extern uint32_t bpm;
extern float spo2;
