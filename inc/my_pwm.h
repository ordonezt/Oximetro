/*
 * my_pwm.h
 *
 *  Created on: 8 oct. 2019
 *      Author: ordonezt
 */

#ifndef MY_PWM_H_
#define MY_PWM_H_

#define RED_PORT 	2
#define RED_PIN		0

#define IR_PORT		2
#define IR_PIN		1

#define RED_PWM 	PWM1
#define IR_PWM		PWM2

#define DUTY		0.8
#define RED_DUTY 	DUTY
#define IR_DUTY		DUTY

#define BIT_COUNTER_ENABLE 0
#define BIT_COUNTER_RESET  1
#define BIT_PWM_ENABLE 3

#define BIT_COUNT_INPUT 2

#define MAX_DUTY 	1

#define PWM_FREQ	100000		//100KHz

typedef enum{M_INTERRUP = 0b001, M_RESET = 0b010, M_STOP = 0b100} match_action;

typedef enum{PWM_TIMER = 0, COUNTER_RISING, COUNTER_FALLING, COUNTER_BOTH} pwm_modes;

typedef enum{PCAP10 = 0, PCAP11} pwm_capture_inputs;

typedef enum{MATCH0 = 0, MATCH1, MATCH2, MATCH3, MATCH4, MATCH5, MATCH6} pwm_matchs;

typedef enum{PWM1 = 1, PWM2, PWM3, PWM4, PWM5, PWM6}pwm_outs;

typedef enum{SINGLE_EDGE = 0, DOUBLE_EDGE = !SINGLE_EDGE} pwm_edge_types;

/**
 * @brief PWM register block structure
 */
typedef struct {					/*!< PWM Structure        		*/
	__IO  uint32_t  IR;				/*!< PWM Interrupt register 	*/
	__IO  uint32_t  TCR;			/*!< Timer control register 	*/
	__IO  uint32_t  TC;				/*!< Timer counter register 	*/
	__IO  uint32_t  PR;				/*!< Prescale register 			*/
	__IO  uint32_t  PC;				/*!< Prescale counter register 	*/
	__IO  uint32_t  MCR;			/*!< Match control register 	*/
	__IO  uint32_t 	MRA[4];			/*!< Match register parte A		*/	//Se podria mejorar con una union
	__IO  uint32_t 	CCR;			/*!< Capture control register   */
	__I   uint32_t 	CR[4];			/*!< Capture register         	*/
	__I   uint32_t 	RESERVED1;		/*!< Reserved register         	*/
	__IO  uint32_t  MRB[3];			/*!< Match register parte B		*/ //CUIDADO ACAAAAA
	__IO  uint32_t  PCR;			/*!< PWM control register 		*/
	__IO  uint32_t 	LER;			/*!< Load enable register       */
	__I   uint32_t 	RESERVED2[8];	/*!< Reserved registers        	*/
	__IO  uint32_t  CTCR;			/*!< Count control register 	*/
}LPC_PWM1_T;

#define	LPC_PWM1	((LPC_PWM1_T	*) LPC_PWM1_BASE)

/**
 * @brief	Define el valor del prescaler
 * @param	pr		: Valor por el que se quiere dividir el PCLOCK
 * @return	Nothing
 * @note	El TC se incrementa cada Prescaler + 1
 */
void Chip_PWM_SetPrescale(uint32_t pr);

/**
 * @brief	Lee el valor del prescaler
 * @return	Valor por el que se divide el PCLOCK
 * @note	El TC se incrementa cada Prescaler + 1
 */
uint32_t Chip_PWM_GetPrescale(void);

/**
 * @brief	Habilita o deshabilita el contador
 * @param	state		: Estado en el que se desea poner al contador ENABLE o DISABLE
 * @return	Nothing
 * @note	Si se habilita el contador, se comienza a contar, pero no significa que el PWM este funcionando
 */
void Chip_PWM_CounterEnable(uint8_t state);

/**
 * @brief	Establece o borra un reset en el contador TC y el del prescaler
 * @param	state		: ENABLE o DISABLE
 * @return	Nothing
 * @note	Para hacer un reset se debe usar esta funcion con ENABLE y luego DISABLE
 */
void Chip_PWM_CounterReset(uint8_t state);

/**
 * @brief	Habilita o deshabilita el perisferico de PWM
 * @param	state		: ENABLE o DISABLE
 * @return	Nothing
 * @note	-
 */
void Chip_PWM_Enable(uint8_t state);

/**
 * @brief	Establece el modo de cuenta
 * @param	mode		: PWM_TIMER, COUNTER_RISING, COUNTER_FALLING o COUNTER_BOTH
 * @param	input		: PCAP10 O PCAP11
 * @return	Nothing
 * @note	En modo timer el TC se incrementa cuando el PC se igual al PR, da igual el valor de input
 * 			En modo contador el TC se incrementa con los flancos de la fuente definida por input
 */
void Chip_PWM_CountMode(uint8_t mode, uint8_t input);

/**
 * @brief	Selecciona la accion que tendra el match
 * @param	match		: Match objetivo entre 0 a 6 MATCHn
 * @param	accion		: Puede ser simultaneamente M_INTERRUPT, M_RESET el contador y M_STOP el contador
 * @return	Nothing
 * @note	Si se quiere hacer mas de una accion simplemente M_INTERRUPT | M_STOP | M_RESET
 */
void Chip_PWM_SetMatchControl(uint8_t match, uint8_t accion);

/**
 * @brief	Limpia la accion que tenia el match
 * @param	match		: Match objetivo entre 0 a 6
 * @param	accion		: Puede ser simultaneamente INTERRUPT, RESET el contador y STOP el contador
 * @return	Nothing
 * @note	Si se quiere hacer mas de una accion simplemente INTERRUPT | STOP | RESET
 */
void Chip_PWM_ClearMatchControl(uint8_t match, uint8_t accion);

/**
 * @brief	Selecciona el tipo de flanco de salida
 * @param	salida		: Salida PWMn donde n pertenece a [2, 6]
 * @param	modo		: Puede ser SINGLE_EDGE o DOUBLE_EDGE
 * @return	Nothing
 * @note	La salida PWM1 no puede ser double edge
 */
void Chip_PWM_SelectEdge(uint8_t salida, uint8_t modo);

/**
 * @brief	Habilita o deshabilita la salida de PWM
 * @param	salida		: Salida PWMn donde n pertenece a [2, 6]
 * @param	estado		: Puede ser PWM_OUT_ENABLE o PWM_OUT_DISABLE
 * @return	Nothing
 * @note	-
 */
void Chip_PWM_OutputEnable(uint8_t salida, uint8_t estado);

/**
 * @brief	Habilita el cambio del valor del match
 * @param	salida		: Salida PWMn donde n pertenece a [2, 6]
 * @param	estado		: Puede ser PWM_OUT_ENABLE o PWM_OUT_DISABLE
 * @return	Nothing
 * @note	-
 */
void Chip_PWM_LatchEnable(uint8_t match);

/**
 * @brief	Configuracion de los registros de match
 * @param	match		: Match objetivo del 0 al 6
 * @param	valor		: valor del match
 * @return	Nothing
 * @note	El match 0 se debe inicializar antes de habilitar el PWM
 */
void Chip_PWM_SetMatch(uint8_t match, uint32_t valor);

/**
 * @brief	Devuelve el valor del match
 * @param	match		: Match objetivo del 0 al 6
 * @return	valor del match
 * @note	-
 */
uint32_t Chip_PWM_GetMatch(uint8_t match);

void PWM_TurnOn(void);

void PWM_SetDuty(uint8_t output, float duty);

void PWM_SetFreq(uint32_t freq);

float PWM_GetDuty(uint8_t output);

void initPWM(void);

void setLed(led_t led);

#endif /* MY_PWM_H_ */
