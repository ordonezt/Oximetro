/*
 * my_extInt.c
 *
 *  Created on: 4 oct. 2019
 *      Author: ordonezt
 */
#include "my_include.h"

void initExtInt(void)
{
	//Borro el flag por las dudas
	LPC_SYSCTL->EXTINT |= 1 << EINT0;

	//Interrumpe por nivel
	LPC_SYSCTL->EXTMODE |= 1 << EINT0;

	//Interrumpe por low-level
	LPC_SYSCTL->EXTPOLAR &= ~(1 << EINT0);

	//Habilito la interrupcion
	NVIC_EnableIRQ(EINT0_IRQn);

	//Pongo el boton como interrupcion externa con pull up
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN_PORT, BTN_PIN, IOCON_FUNC1 | IOCON_MODE_PULLUP);
}

void EINT0_IRQHandler(void)
{
	//Borrar flag de EINT0
	LPC_SYSCTL->EXTINT |= 1 << EINT0; //Por las dudas hacerlo tambien cuando detecto que suelto el dedo

	//Desactivo interrupciones
	NVIC_DisableIRQ(EINT0_IRQn);
}
