/*
===============================================================================
 Name        : Oximetro.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/
#include "my_include.h"

volatile flags_t flags = {0};

SPI_CONFIG_FORMAT_T spi_format;
SPI_DATA_SETUP_T spi_xf;

float bpm = 0;
float spo2 = 0;

pulse_t pulsos[BUFFER_HEIGHT] = {0};

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#endif

    statePwr_t power_state = AWAKE;

    static uint32_t beatTick = 0, debounceTick = 0, displayTick = 0, uartTxTick = 0, checkFingerTick = 0;



    initSystem();

#ifdef TEST
    static uint32_t blinkTick = 0;
#endif

    while(1)
    {
    	if(tick - debounceTick >= DEBOUNCE_TICKS)
		{
			debounceTick = tick;
			debounce();
		}
    	//Primera maquina de estados define el estado de poder /**TODO pasar a vector de punteros a funcion**/
		switch(power_state)
		{
			case AWAKE:
				while(!RingBuffer_IsEmpty(&RingBuffADC[IR]) && !RingBuffer_IsEmpty(&RingBuffADC[RED]))
				{
					RingBuffer_Pop(&RingBuffADC[RED], &pulsos[RED].muestra);
					RingBuffer_Pop(&RingBuffADC[IR], &pulsos[IR].muestra);
					process(pulsos);
				}
				if(flags.beat_detected)
				{
					flags.beat_detected = FALSE;
					Chip_GPIO_SetPinOutLow(LPC_GPIO, STATE_PORT, STATE_PIN);	//Prende led
					beatTick = tick;
					Calculate(pulsos);
				}
				if(button.wasRelease || flags.no_finger_times == MAX_NO_FINGER_TIME)
				{
					button.wasRelease = false;
					flags.no_finger_times = 0;
					power_state = SLEEP;
					setLedState(SLEEP);
				}

				if(tick - beatTick >= BEAT_TICKS)
					Chip_GPIO_SetPinOutHigh(LPC_GPIO, STATE_PORT, STATE_PIN);	//Apaga led
				if(tick - checkFingerTick >= CHECK_FINGER_TICKS)
				{
					checkFingerTick += CHECK_FINGER_TICKS;
					if(!checkFinger())
						flags.no_finger_times++;
					else
						flags.no_finger_times = 0;
				}
				if(tick - uartTxTick >= UART_TX_TICKS)
				{
					uartTxTick = tick;
					UartTransmit();
				}
				if(tick - displayTick >= DISPLAY_TICKS)
				{
					displayTick = tick;
					updateDisplay();
				}
#ifdef TEST
				if(tick - blinkTick >= BLINK_TICKS)
				{
					blinkTick = tick;
					Chip_GPIO_SetPinToggle(LPC_GPIO, BLINK_PORT, BLINK_PIN);
				}
#endif
				break;
			case SLEEP:
				goToSleep();
				//Si llegue aca es por que desperte, voy a estado normal
				while(power_state == SLEEP){
					if(tick - debounceTick >= DEBOUNCE_TICKS)
					{
						debounceTick = tick;
						debounce();
					}
					//Espero a que suelte el boton aca, porque si no vuelvo a dormir
					if(button.wasRelease == TRUE){
						button.wasRelease = FALSE;
						power_state = AWAKE;
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
