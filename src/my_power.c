/*
 * my_power.c
 *
 *  Created on: 4 oct. 2019
 *      Author: ordonezt
 */
#include "my_include.h"

void goToSleep(void)
{
	/*TODO seccion 4.8 manual lpc*/
	//Apagar ADC
	//Poner puertos como entradas pull down
	initExtInt();
	//Activar sleep

	/*
	 * Como se pone en power down mode y como lo despierto?
	 *
	 * Segun datasheet tengo que reconfigurar el pll y los divisores de clock... muy complejo
	 * conviene eso?
	 * o apago las cosas?
	 */
}
