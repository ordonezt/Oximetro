/*
 * my_gpio.h
 *
 *  Created on: 6 oct. 2019
 *      Author: brousse
 */

#ifndef MY_GPIO_H_
#define MY_GPIO_H_

#define N_TIMES	6

#define STATE_PORT			0
#define STATE_PIN			22

#define BLINK_PORT			3
#define BLINK_PIN			26

#define BTN_PORT			2
#define BTN_PIN				10

#define DC_LEVEL_PORT		2
#define DC_LEVEL_PIN		2

void initGpio(void);
void debounce(void);
bool getKey(void);
void setLedState(bool state);
void toggleLed(void);

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
