/*
 * my_timer.h
 *
 *  Created on: 4 oct. 2019
 *      Author: ordonezt
 */

#ifndef MY_TIMER_H_
#define MY_TIMER_H_

#define TICKRATE_HZ1 	1000 //1000 Hz => 1ms
#define SAMPLE_PERIOD 	4 //2ms

#define DEBOUNCE_TICKS		1
#define UART_TX_TICKS 		1
#define DISPLAY_TICKS		100
#define BLINK_TICKS			1000

extern uint32_t tick;

void initTimer(void);

#endif /* MY_TIMER_H_ */
