/*
 * my_gpio.c
 *
 *  Created on: 6 oct. 2019
 *      Author: ordonezt
 */

#include "my_include.h"

volatile key_t button = {0};
volatile led_t led = RED;

void initGpio(void)
{
	Chip_GPIO_Init(LPC_GPIO);
	Chip_IOCON_Init(LPC_IOCON);

	//Salidas
	//Pin salida para estado P0.22
	Chip_IOCON_PinMuxSet(LPC_IOCON, STATE_PORT, STATE_PIN, IOCON_FUNC0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, STATE_PORT, STATE_PIN, true);

	//Pin salida para heartbeat P0.22
	Chip_IOCON_PinMuxSet(LPC_IOCON, HEARTBEAT_PORT, HEARTBEAT_PIN, IOCON_FUNC0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, HEARTBEAT_PORT, HEARTBEAT_PIN, true);

	//Entradas
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN_PORT, BTN_PIN, IOCON_FUNC0 | IOCON_MODE_PULLUP);
	Chip_GPIO_WriteDirBit(LPC_GPIO, BTN_PORT, BTN_PIN, false);

	Chip_IOCON_PinMuxSet(LPC_IOCON, DC_LEVEL_PORT, DC_LEVEL_PIN, IOCON_FUNC0 | IOCON_MODE_PULLUP);
	Chip_GPIO_WriteDirBit(LPC_GPIO, DC_LEVEL_PORT, DC_LEVEL_PIN, false);
}

void setLedState(bool state)
{
	Chip_GPIO_SetPinState(LPC_GPIO, STATE_PORT, STATE_PIN, !state); //Led activo bajo
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
			button.wasPressed = true;
		}
		else if(button.state == PRESSED && curr == NO_PRESSED)
		{
			button.state = NO_PRESSED;
			button.wasRelease = true;
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
