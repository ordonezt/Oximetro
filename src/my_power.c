/*
 * my_power.c
 *
 *  Created on: 4 oct. 2019
 *      Author: ordonezt
 */
#include "my_include.h"

void goToSleep(void)
{
	//Desactivo las interrupciones del sistema
	NVIC_DisableIRQ(ADC_IRQn);
	NVIC_DisableIRQ(SysTick_IRQn);

	//Habilito la interrupcion externa para poder despertar
	initExtInt();

	//Activar sleep
	Chip_PMU_Sleep(LPC_PMU, PMU_MCU_SLEEP);
}

void restoreSystem(void)
{
	//Vuelvo a poner al boton como GPIO
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN_PORT, BTN_PIN, IOCON_FUNC0 | IOCON_MODE_PULLUP);
	Chip_GPIO_WriteDirBit(LPC_GPIO, BTN_PORT, BTN_PIN, false);

	//Reactivo las interrupciones del sistema
	NVIC_EnableIRQ(ADC_IRQn);
	NVIC_EnableIRQ(SysTick_IRQn);
}
