/*
 * my_gpio.c
 *
 *  Created on: 6 oct. 2019
 *      Author: ordonezt
 */

#include "my_include.h"

volatile key_t button = {0};
volatile led_t led = IR;

void initGpio(void)
{
	Chip_GPIO_Init(LPC_GPIO);
	Chip_IOCON_Init(LPC_IOCON);

	//Salidas
	//Pin salida para estado P0.22
	Chip_IOCON_PinMuxSet(LPC_IOCON, STATE_PORT, STATE_PIN, IOCON_FUNC0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, STATE_PORT, STATE_PIN, TRUE);
//	Chip_GPIO_SetPinOutHigh(LPC_GPIO, STATE_PORT, STATE_PIN);
	Chip_GPIO_SetPinOutLow(LPC_GPIO, STATE_PORT, STATE_PIN);

//	//Pin salida para estado P0.22
//	Chip_IOCON_PinMuxSet(LPC_IOCON, STATE_GABINETE_PORT, STATE_GABINETE_PIN, IOCON_FUNC0);
//	Chip_GPIO_WriteDirBit(LPC_GPIO, STATE_GABINETE_PORT, STATE_GABINETE_PIN, TRUE);
//	Chip_GPIO_SetPinOutHigh(LPC_GPIO, STATE_GABINETE_PORT, STATE_GABINETE_PIN);

	//Pin salida para blink P3.26
	Chip_IOCON_PinMuxSet(LPC_IOCON, BLINK_PORT, BLINK_PIN, IOCON_FUNC0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, BLINK_PORT, BLINK_PIN, TRUE);
	Chip_GPIO_SetPinOutHigh(LPC_GPIO, BLINK_PORT, BLINK_PIN);

	//Entradas
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN_PORT, BTN_PIN, IOCON_FUNC0 | IOCON_MODE_PULLUP);
	Chip_GPIO_WriteDirBit(LPC_GPIO, BTN_PORT, BTN_PIN, FALSE);

	Chip_IOCON_PinMuxSet(LPC_IOCON, DC_LEVEL_PORT, DC_LEVEL_PIN, IOCON_FUNC0 | IOCON_MODE_PULLUP);
	Chip_GPIO_WriteDirBit(LPC_GPIO, DC_LEVEL_PORT, DC_LEVEL_PIN, FALSE);

	//Configuro pines de los leds como gpio
	Chip_GPIO_WriteDirBit(LPC_GPIO, RED_PORT, RED_PIN, TRUE);
	Chip_GPIO_WriteDirBit(LPC_GPIO, IR_PORT, IR_PIN, TRUE);

	Chip_IOCON_PinMuxSet(LPC_IOCON, RED_PORT, RED_PIN, IOCON_FUNC0);
	Chip_IOCON_PinMuxSet(LPC_IOCON, IR_PORT, IR_PIN, IOCON_FUNC0);

	Chip_GPIO_SetPinState(LPC_GPIO, RED_PORT, RED_PIN, FALSE);
	Chip_GPIO_SetPinState(LPC_GPIO, IR_PORT, IR_PIN, FALSE);
}

void setLedState(bool state)
{
	Chip_GPIO_SetPinState(LPC_GPIO, STATE_PORT, STATE_PIN, !state); //Led activo bajo
}

void toggleLed(void)
{
	__disable_irq();
	led = !led;
	setLed(led);
	__enable_irq();
}

void debounce(void)
{
	static keyState_t prev = NO_PRESSED;
	keyState_t curr;
	static uint8_t counter_equals = 0;

	curr = getKey()? NO_PRESSED : PRESSED;

	counter_equals = (prev == curr)? counter_equals + 1 : 0;

	if(counter_equals == N_TIMES)
	{
		counter_equals = 0;
		if(button.state == NO_PRESSED && curr == PRESSED)
		{
			button.state = PRESSED;
			button.wasPressed = TRUE;
		}
		else if(button.state == PRESSED && curr == NO_PRESSED)
		{
			button.state = NO_PRESSED;
			button.wasRelease = TRUE;
		}
		else
			button.state = curr;
	}

	prev = curr;
}

bool getKey(void)
{
	return Chip_GPIO_GetPinState(LPC_GPIO, BTN_PORT, BTN_PIN);
}
