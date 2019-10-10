/*
 * my_pwm.h
 *
 *  Created on: 8 oct. 2019
 *      Author: brousse
 */

#ifndef MY_PWM_H_
#define MY_PWM_H_

typedef enum{ RED = 0, IR}led_t;

extern volatile led_t led;

#endif /* MY_PWM_H_ */
