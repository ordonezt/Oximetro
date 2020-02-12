/*
 * my_pwm.c
 *
 *  Created on: 8 oct. 2019
 *      Author: ordonezt
 */

#include "my_include.h"

void initPWM(void)
{

	//Configuro pines como gpio
	Chip_GPIO_WriteDirBit(LPC_GPIO, RED_PORT, RED_PIN, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, IR_PORT, IR_PIN, true);

	Chip_IOCON_PinMuxSet(LPC_IOCON, RED_PORT, RED_PIN, IOCON_FUNC0);
	Chip_IOCON_PinMuxSet(LPC_IOCON, IR_PORT, IR_PIN, IOCON_FUNC0);

//	//Configuro pines como PWM
//	Chip_IOCON_PinMuxSet(LPC_IOCON, RED_PORT, RED_PIN, IOCON_FUNC1);
//	Chip_IOCON_PinMuxSet(LPC_IOCON, IR_PORT, IR_PIN, IOCON_FUNC1);
//
//	//Habilito el clock del perisferico
//	if(!Chip_Clock_IsPeripheralClockEnabled(SYSCTL_CLOCK_PWM1))
//		Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_PWM1);
//
//	//Establezco el prescaler del perisferico
//	Chip_Clock_SetPCLKDiv(SYSCTL_CLOCK_PWM1,SYSCTL_CLKDIV_4);
//
//	//Prescaler del PWM
//	Chip_PWM_SetPrescale(25);
//
//	//Establezco el modo PWM
//	Chip_PWM_CountMode(PWM_TIMER, 0);
//
//	//El match 0 resetea la cuenta
//	Chip_PWM_SetMatchControl(MATCH0, M_RESET);
//
//	//Defino la frecuencia de salida del PWM
//	PWM_SetFreq(PWM_FREQ);
//
//	//Habilito las salidas
//	Chip_PWM_OutputEnable(RED_PWM, ENABLE);
//	Chip_PWM_OutputEnable(IR_PWM, ENABLE);
//
//	PWM_SetDuty(RED_PWM, RED_DUTY);
//	PWM_SetDuty(IR_PWM, IR_DUTY);
//
//	PWM_TurnOn();
}

void Chip_PWM_SetPrescale(uint32_t pr)
{
	LPC_PWM1->PR = pr - 1;
}

uint32_t Chip_PWM_GetPrescale(void)
{
	return LPC_PWM1->PR + 1;
}

void Chip_PWM_CounterEnable(uint8_t state)
{
	if(state == ENABLE)
		LPC_PWM1->TCR |= 1 << BIT_COUNTER_ENABLE;
	else
		LPC_PWM1->TCR &= ~(1 << BIT_COUNTER_ENABLE);
}

void Chip_PWM_CounterReset(uint8_t state)
{
	if(state == ENABLE)
		LPC_PWM1->TCR |= 1 << BIT_COUNTER_RESET;
	else
		LPC_PWM1->TCR &= ~(1 << BIT_COUNTER_RESET);

}

void Chip_PWM_Enable(uint8_t state)
{
	if(state == ENABLE)
		LPC_PWM1->TCR |= 1 << BIT_PWM_ENABLE;
	else
		LPC_PWM1->TCR &= ~(1 << BIT_PWM_ENABLE);

}

void Chip_PWM_CountMode(uint8_t mode, uint8_t input)
{
	LPC_PWM1->CTCR &= ~0xF;	//Limpio los bits
	LPC_PWM1->CTCR |= mode | (input << BIT_COUNT_INPUT);
}

void Chip_PWM_SetMatchControl(uint8_t match, uint8_t accion)
{
	LPC_PWM1->MCR &= ~(accion << match*3); 	//Limpio el bit
	LPC_PWM1->MCR |= accion << match*3;		//Cargo la accion
}

void Chip_PWM_ClearMatchControl(uint8_t match, uint8_t accion)
{
	LPC_PWM1->MCR &= ~(accion << match*3); 	//Limpio la accion
}

void Chip_PWM_SelectEdge(uint8_t salida, uint8_t modo)
{
	if(modo == DOUBLE_EDGE)
		LPC_PWM1->PCR |= 1 << salida;
	else
		LPC_PWM1->PCR &= ~(1 << salida);
}

void Chip_PWM_OutputEnable(uint8_t salida, uint8_t estado)
{
	if(estado == ENABLE)
		LPC_PWM1->PCR |= 1 << (salida + 8);
	else
		LPC_PWM1->PCR &= ~(1 << (salida + 8));
}

void Chip_PWM_LatchEnable(uint8_t match)
{
	LPC_PWM1->LER |= 1 << match;
}

void Chip_PWM_SetMatch(uint8_t match, uint32_t valor)
{
	if(match <= 3)
		LPC_PWM1->MRA[match] = valor;
	else
		LPC_PWM1->MRB[match-4] = valor;
}

uint32_t Chip_PWM_GetMatch(uint8_t match)
{
	if(match <= 3)
		return LPC_PWM1->MRA[match];
	else
		return LPC_PWM1->MRB[match - 4];
}

void InitPWM(void)
{
	//Paso 1
	if(!Chip_Clock_IsPeripheralClockEnabled(SYSCTL_CLOCK_PWM1))
		Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_PWM1);

	//Paso 2
	Chip_Clock_SetPCLKDiv(SYSCTL_CLOCK_PWM1,SYSCTL_CLKDIV_4); //25MHz
}

void ConfigPWM(void)
{
	Chip_PWM_SetPrescale(25);	//1MHz

	Chip_PWM_CountMode(PWM_TIMER, 0);

	Chip_PWM_SetMatchControl(MATCH0, M_RESET);

	//Se debe establecer el periodo del PWM (Match 0) antes de habilitarlo si no, no arranca
	Chip_PWM_SetMatch(MATCH0, PWM_FREQ);

	//Habilito el latch haciendo efectivo los cambios en el match
	Chip_PWM_LatchEnable(MATCH0);

	//Reseteo la cuenta
	Chip_PWM_CounterReset(ENABLE);

	//PWM listo para ser encendido y funcionar
}

void PWM_SetFreq(uint32_t freq)
{
	uint32_t ticks;

	ticks = ((Chip_Clock_GetPeripheralClockRate(SYSCTL_PCLK_PWM1) / Chip_PWM_GetPrescale())) / freq;

	Chip_PWM_SetMatch(MATCH0, ticks);

	Chip_PWM_LatchEnable(MATCH0);
}

void PWM_SetDuty(uint8_t output, float duty)
{
	uint32_t ticks;

	ticks = (uint32_t)(duty * Chip_PWM_GetMatch(MATCH0)) / MAX_DUTY;

	Chip_PWM_SetMatch(output, ticks);

	Chip_PWM_LatchEnable(output);
}

float PWM_GetDuty(uint8_t output)
{
	//duty numero entre 0 y MAX_DUTY
	//Asumo funcionamiento en single edge => output = match

	return (Chip_PWM_GetMatch(output) / Chip_PWM_GetMatch(MATCH0)) * MAX_DUTY;
}

void PWM_TurnOn(void)
{
	//Habilito el contador y lo saco del estado de reset
	Chip_PWM_CounterEnable(ENABLE);
	Chip_PWM_CounterReset(DISABLE);

	//Habilito el PWM
	Chip_PWM_Enable(ENABLE);
}

void setLed(led_t led)
{
	if(led == RED)
	{
		Chip_GPIO_SetPinState(LPC_GPIO, IR_PORT, IR_PIN, false);
		Chip_GPIO_SetPinState(LPC_GPIO, RED_PORT, RED_PIN, true);
	}

	else
	{
//		Chip_GPIO_SetPinState(LPC_GPIO, RED_PORT, RED_PIN, true);
//		Chip_GPIO_SetPinState(LPC_GPIO, IR_PORT, IR_PIN, false);
	}
}
