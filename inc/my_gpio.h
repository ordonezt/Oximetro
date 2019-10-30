/*
 * my_gpio.h
 *
 *  Created on: 6 oct. 2019
 *      Author: brousse
 */

#ifndef MY_GPIO_H_
#define MY_GPIO_H_

#include <stdbool.h>

#define N_TIMES	6

#define HEARTBEAT_PORT		0
#define HEARTBEAT_PIN		22

#define BTN_PORT		2
#define BTN_PIN			10

void initGpio(void);
void debounce(void);
bool getKey(void);

typedef enum{NO_PRESSED, PRESSED}keyState_t;

typedef struct{
	keyState_t state;
	bool wasPressed;
	bool wasRelease;
}key_t;

typedef enum{RED, IR}led_t;

extern volatile led_t led;

extern volatile key_t button;

#endif /* MY_GPIO_H_ */