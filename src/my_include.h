/*
 * my_include.h
 *
 *  Created on: 4 oct. 2019
 *      Author: ordonezt
 */

#ifndef MY_INCLUDE_H_
#define MY_INCLUDE_H_

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "my_power.h"
#include "my_extInt.h"
#include "my_timer.h"
#include "my_gpio.h"
#include "my_adc.h"
#include "my_pwm.h"
#include "oximetro.h"
#include "my_dataManagment.h"
#include "my_display.h"
#include "my_watchdog.h"
#include "my_system.h"

#define MAX_NO_FINGER_TIME 7

typedef struct		//Estructura para registro CTIME
{
	uint16_t sleep_time:1;	//No tiene uso
	uint16_t led_flag:1;
	uint16_t debounce_time:1;
	uint16_t show_time:1;
	uint16_t check_finger_time:1;
	uint16_t adc_time:1;
	uint16_t is_finger:1;
	uint16_t conversion_done:1;
	uint16_t sample_buffer_full:1;
	uint16_t no_finger_times:3;
	uint16_t RESERVED:7;
}flags_t;

volatile flags_t flags;

#endif /* MY_INCLUDE_H_ */
