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

void initGpio(void);
void debounce(void);

typedef enum{ NO_PRESSED = 0, PRESSED}keyState_t;

typedef struct{
	keyState_t state;
	bool wasPressed;
	bool wasRelease;
}key_t;

extern volatile key_t button;

#endif /* MY_GPIO_H_ */
