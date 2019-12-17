/*
 * my_include.h
 *
 *  Created on: 4 oct. 2019
 *      Author: ordonezt
 */

#ifndef MY_INCLUDE_H_
#define MY_INCLUDE_H_

#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
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
#include "my_twi.h"
#include "my_uart.h"
#include "my_OLED.h"
#include "filters.h"
#include "my_measure.h"

#define MAX_NO_FINGER_TIME 20

typedef struct		//Estructura para manejo de flags del sistema
{
	uint16_t sleep_time:1;	//No tiene uso
	uint16_t led_flag:1;
	uint16_t debounce_time:1;
	uint16_t show_time:1;
	uint16_t check_finger_time:1;
	uint16_t adc_time:1;
	uint16_t is_finger:1;
	uint16_t conversion_done:1;
	uint16_t beat_detected:1;
	uint16_t no_finger_times:3;
	uint16_t adc_buffer_error:1;
	uint16_t RESERVED:6;
}flags_t;

extern volatile flags_t flags;

#endif /* MY_INCLUDE_H_ */
