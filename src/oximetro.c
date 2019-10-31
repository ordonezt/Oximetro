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

#include "my_include.h"

volatile flags_t flags = {0};

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#endif

    statePwr_t power_state = AWAKE;
    stateWork_t work_state = WAITING;

    pulse_t Red_p;
    Red_p.Led = RED;
    pulse_t Ir_p;
    Red_p.Led = IR;
    pulse_t *Data[2] = {&Red_p, &Ir_p};

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
    					if(isFinger())
    						work_state = WORKING;
    					break;
    				case WORKING:
    					if(flags.conversion_done)
						{
							flags.conversion_done = false;
//							process(Data[led]);
							led = !led;
						}
						if(flags.beat_detected)
						{
							if (cuenta_muestras > MAX_WINDOW) {
								flags.beat_detected = false;
								cuenta_muestras = 0;

								// obtenemos min y max
								// obtenemos R
								// obtenemos
		//						display(calculateSpO2(), calculateBPM());
							}

						}
						if(!isFinger())
							work_state = WAITING;
						break;
    				default:
    					work_state = WAITING;
    					break;
    				}

    				if(button.wasRelease || flags.no_finger_times == MAX_NO_FINGER_TIME)
    				{
    					button.wasRelease = false;
    					flags.no_finger_times = 0;
    					power_state = SLEEP;
    					setLedState(SLEEP);
    				}
    				break;

    			case SLEEP:
    				goToSleep();
    				//Si llegue aca es por que desperte, voy a estado normal
    				while(power_state == SLEEP){
    					//Espero a que suelte el boton aca, por que si no vuelvo a dormir
						if(button.wasRelease == true){
							button.wasRelease = false;
							power_state = AWAKE;
							work_state = WAITING;
							setLedState(AWAKE);
						}
    				}
    				break;

    			default:
    				power_state = AWAKE;
    				break;
    		}
    	}
    return 0 ;
}
