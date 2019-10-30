/*
 * my_include.h
 *
 *  Created on: 4 oct. 2019
 *      Author: ordonezt
 */

#ifndef MY_INCLUDE_H_
#define MY_INCLUDE_H_

<<<<<<< HEAD:src/my_include.h
#include <avr/io.h>
#include <compat/twi.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <string.h>

=======
#include "board.h"
>>>>>>> d17e67473b46f8075424f19aa6d8c4e035d858d1:inc/my_include.h
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

#define MAX_NO_FINGER_TIME 7

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
	uint16_t RESERVED:7;
}flags_t;

extern volatile flags_t flags;

#endif /* MY_INCLUDE_H_ */
