/*
 * my_extInt.c
 *
 *  Created on: 4 oct. 2019
 *      Author: brousse
 */
#include "my_include.h"

void initExtInt(void)
{
	//Boton con pull up a masa
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN_PORT, BTN_PIN, IOCON_FUNC1 | IOCON_MODE_PULLUP);

}

void delExtInt(void)
{
	//Desactivo interrupciones
}

void EINT0_IRQHandler(void)
{
	//Desactivar modo sleep
	//Borrar flag de EINT0
	LPC_SYSCTL->EXTINT |= 1 << EINT0; //Por las dudas hacerlo tambien cuando detecto que suelto el dedo
	delExtInt();
}
