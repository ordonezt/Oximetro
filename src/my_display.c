/*
 * display.c
 *
 *  Created on: 20 oct. 2019
 *      Author: ordonezt
 */


#include "my_include.h"

extern uint32_t cuenta_impresas;
extern float bpm;
extern float spo2;
extern float ThresholdInf;
extern float ThresholdSup;
extern float envolventeMin;
extern float envolventeMax;

extern pulse_t pulsos[BUFFER_HEIGHT];

#define ASDF 5


void updateDisplay(void) {

	float data;

	static uint32_t par=0;
	//TODO hacer la rutina de refresco según display elegido
	//OledSignal(smooth[IR][0]);

//	OLED_Fill(Black);
//	OLED_SetCursor(0, 0);
//	OLED_WriteString("Hola OLED",Font_11x18,White);
//

	static uint32_t x=0;
	uint32_t i,j;
	char str[10];

	RingBuffSmooth[IR].tail -= cuenta_impresas;
	for(i=1;i<=cuenta_impresas;i++)
	{
		RingBuffer_Pop(&RingBuffSmooth[IR], &data);

		if(!par){
			for(j=0; j<SSD1306_HEIGHT; j++)  //BORRA TODA LA COLUMNA
				OLED_DrawPixel(x,j,Black);

//			if(gradient[IR][cuenta_impresas-i]>=gradient[IR][cuenta_impresas-i+ASDF])
//			{
//				for(j=(uint32_t)gradient[IR][cuenta_impresas-i+ASDF];j<=(uint32_t)gradient[IR][cuenta_impresas-i];j++)
//					OLED_DrawPixel(x,(uint8_t)(SSD1306_HEIGHT-7-((SSD1306_HEIGHT*j)/200)),White);
//			}
//			else
//			{
//				for(j=(uint32_t)gradient[IR][cuenta_impresas-i];j<=(uint32_t)gradient[IR][cuenta_impresas-i+ASDF];j++)
//									OLED_DrawPixel(x,(uint8_t)(SSD1306_HEIGHT-7-((SSD1306_HEIGHT*j)/200)),White);
//			}

/*			if(smooth[IR][cuenta_impresas-i]>=smooth[IR][cuenta_impresas-i+ASDF])
			{
				for(j=(uint32_t)smooth[IR][cuenta_impresas-i+ASDF];j<=(uint32_t)smooth[IR][cuenta_impresas-i];j++)
					OLED_DrawPixel(x,(uint8_t)(SSD1306_HEIGHT-((SSD1306_HEIGHT*j)/6144)),White);
			}
			else
			{
				for(j=(uint32_t)smooth[IR][cuenta_impresas-i];j<=(uint32_t)smooth[IR][cuenta_impresas-i+ASDF];j++)
									OLED_DrawPixel(x,(uint8_t)(SSD1306_HEIGHT-((SSD1306_HEIGHT*j)/6144)),White);
			}*/
//			OLED_DrawPixel(x,(uint8_t)(((SSD1306_HEIGHT*(data - globMin))/globMax)),White);
//			OLED_DrawPixel(x,(uint8_t)(((SSD1306_HEIGHT*(envolventeMax - globMin))/globMax)),White);
//			OLED_DrawPixel(x,(uint8_t)(((SSD1306_HEIGHT*(envolventeMin - globMin))/globMax)),White);

			OLED_DrawPixel(x,(uint8_t)(((SSD1306_HEIGHT*(data - globMin))/globMax)),White);
			OLED_DrawPixel(x,(uint8_t)(((SSD1306_HEIGHT*(ThresholdInf - globMin))/globMax)),White);
			OLED_DrawPixel(x,(uint8_t)(((SSD1306_HEIGHT*(ThresholdSup - globMin))/globMax)),White);

//			OLED_DrawPixel(x,(uint8_t)(SSD1306_HEIGHT-((SSD1306_HEIGHT*data)/6144)),White);
//			OLED_DrawPixel(x,(uint8_t)(((SSD1306_HEIGHT*(ThresholdInf - globMin))/globMax)),White);
//			OLED_DrawPixel(x,(uint8_t)(((SSD1306_HEIGHT*(ThresholdSup - globMin))/globMax)),White);

			x++;
			x= x%SSD1306_WIDTH;
			par=ASDF;
		}
		par--;



	}
	cuenta_impresas=0;

//	for(j=25; j<SSD1306_HEIGHT; j++)
//			OLED_DrawPixel(x-pulsos[IR].pos_Dmax/ASDF,j,White);

	OLED_SetCursor(0, 0);
	OLED_WriteString("Freq=",Font_7x10,White);
	//OLED_SetCursor(42, 0);
	OLED_WriteString(itoa(((int)bpm)%1000,str,10),Font_7x10,White);
	OLED_WriteString("bpm ",Font_7x10,White);
	OLED_SetCursor(0, 11);
	OLED_WriteString("spO2=",Font_7x10,White);
	//OLED_SetCursor(42, 11);
	OLED_WriteString(itoa(((int)spo2%1000)%1000,str,10),Font_7x10,White);
	OLED_WriteString("% ",Font_7x10,White);

	OLED_UpdateScreen();

}

void turnOffDisplay(void)
{
	OLED_Fill(Black);
	OLED_UpdateScreen();
}
