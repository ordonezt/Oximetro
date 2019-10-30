/*
 * my_extInt.c
 *
 *  Created on: 4 oct. 2019
 *      Author: ordonezt
 */
#include "my_include.h"

void initExtInt(void)
{
	//Interrumpe por nivel
	LPC_SYSCTL->EXTMODE |= 1 << EINT0;

	//Interrumpe por low-level
	LPC_SYSCTL->EXTPOLAR &= ~(1 << EINT0);

	//Habilito la interrupcion
	NVIC_EnableIRQ(EINT0_IRQn);
}

void delExtInt(void)
{
	//Desactivo interrupciones
	NVIC_DisableIRQ(EINT0_IRQn);
}

void EINT0_IRQHandler(void)
{
	//Desactivar modo sleep
	//Borrar flag de EINT0
	LPC_SYSCTL->EXTINT |= 1 << EINT0; //Por las dudas hacerlo tambien cuando detecto que suelto el dedo
	delExtInt();
}
