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
#include "my_watchdog.h"

typedef struct		//Estructura para registro CTIME
{
	union
	{
		uint8_t byte;
		struct
		{
			uint8_t sleep_time:1;
			uint8_t led_flag:1;
			uint8_t debounce_time:1;
			uint8_t show_time:1;
			uint8_t check_finger_time:1;
			uint8_t adc_time:1;
			uint8_t RESERVED:2;
		}bits;
	};
}flags_t;

volatile flags_t flags;

#endif /* MY_INCLUDE_H_ */
