/*
 * my_uart.c
 *
 *  Created on: Nov 26, 2019
 *      Author: jreiszer
 */

#include <my_include.h>

RINGBUFF_T txring, rxring;

/* Transmit  ring buffer size */
#define UART_SRB_SIZE 128	/* Send */
#define UART_RRB_SIZE 128	/* Receive */
/* Transmit buffers */
static uint8_t  txbuff[UART_SRB_SIZE], rxbuff[UART_RRB_SIZE];

#define UART_Def (LPC_UART0)	/* Defined uart */

extern float freq, bpm;

void initUart(void){
	Chip_IOCON_PinMux(LPC_IOCON, 0, 2, IOCON_MODE_INACT, IOCON_FUNC1);	// P0.2 as Txd0
    Chip_IOCON_PinMux(LPC_IOCON, 0, 3, IOCON_MODE_INACT, IOCON_FUNC1); //  P0.3 as Rxd0

    /* Setup UART for 115.2K8N1 */
	Chip_UART_Init(UART_Def);
	Chip_UART_SetBaud(UART_Def, 115200);
	Chip_UART_ConfigData(UART_Def, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
	Chip_UART_TXEnable(UART_Def);

	RingBuffer_Init(&txring, txbuff, 1, UART_SRB_SIZE);
	RingBuffer_Init(&rxring, rxbuff, 1, UART_RRB_SIZE);
	Chip_UART_SetupFIFOS(UART_Def, (UART_FCR_FIFO_EN | UART_FCR_RX_RS |	UART_FCR_TX_RS | UART_FCR_TRG_LEV2));
}

void UartTransmit(void)
{
	uint8_t data;
	if(!Chip_UART_CheckBusy(UART_Def))
		if(RingBuffer_Pop(&txring, &data))
			Chip_UART_SendByte(UART_Def, data);
}

void graphic(void){

	uint32_t data;
	int initial = '@';
	static int cont = 0;

	int frecuencia = (uint8_t) smooth[0][0];

	if(!Chip_UART_CheckBusy(UART_Def)){
		//if(RingBuffer_Pop(&txring, &data)){
		if (cont)
			Chip_UART_SendByte(UART_Def, frecuencia);
		else
			Chip_UART_SendByte(UART_Def, initial);
		//}
	}
	cont = !cont;
}
