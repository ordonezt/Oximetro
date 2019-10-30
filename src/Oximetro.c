/*
===============================================================================
 Name        : Oximetro.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/
/*
 * TODO:
 * 		-Inicializar bien
 * 		-Hacer las maquinas con punteros a funcion
 * 		-Seguir pensando mejor forma de chequear si hay dedo
 * 			Lo ideal es medir con ADC2 y ver si es cerca de 5V, pero complica la logica, por simplicidad lo hago con entrada digital
 * 		-Pantalla o displays
 * 		-PROBARLO
 */
#include <cr_section_macros.h>

#include "my_include.h"

volatile flags_t flags = {0};

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#endif

    statePwr_t power_state = AWAKE;
    stateWork_t work_state = WAITING;

    initSystem();

    while(1)
    	{
    		//Primera maquina de estados define el estado de poder /**TODO pasar a vector de punteros a funcion**/
    		switch(power_state)
    		{
    			case AWAKE:

    				//Segunda maquina de estados /*TODO pasar a puntero a funcion*/
    				switch(work_state)
    				{
    				case WAITING:
    					led = IR;
    //					display(NA, NA);	/*TODO*/
    					if(flags.is_finger)
    						work_state = WORKING;
    					break;
    				case WORKING:
    					if(flags.conversion_done)
						{
							flags.conversion_done = false;
							led = !led;
							processData();
						}
						if(flags.sample_buffer_full)
						{
							flags.sample_buffer_full = false;
	//						display(calculateSpO2(), calculateBPM());
						}
						if(!flags.is_finger)
							work_state = WAITING;
						break;
    				default:
    					work_state = WAITING;
    					break;
    				}

    				if(button.wasRelease || flags.no_finger_times == MAX_NO_FINGER_TIME)
    				{
    					button.wasRelease = false;
    					power_state = SLEEP;
    				}
    				break;

    			case SLEEP:
    				goToSleep();
    				//Si llegue aca es por que desperte, voy a estado normal
    				if(button.wasRelease == true) //Espero a que suelte el boton aca, por que si no vuelvo a dormir
    				{
    					button.wasRelease = false;
    					power_state = AWAKE;
    					work_state = WAITING;
    				}
    				break;

    			default:
    				power_state = AWAKE;
    				break;
    		}
    	}
    return 0 ;
}