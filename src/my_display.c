/*
 * display.c
 *
 *  Created on: 20 oct. 2019
 *      Author: ordonezt
 */


#include "my_include.h"
#include "my_OLED.h"

extern uint32_t cuenta_impresas;
extern float bpm;
extern float spo2;

#define ASDF 1


void updateDisplay(void) {


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

	for(i=1;i<=cuenta_impresas;i++,i++)
	{
		if(!par){
			for(j=0; j<SSD1306_HEIGHT; j++)
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

			if(smooth[IR][cuenta_impresas-i]>=smooth[IR][cuenta_impresas-i+ASDF])
			{
				for(j=(uint32_t)smooth[IR][cuenta_impresas-i+ASDF];j<=(uint32_t)smooth[IR][cuenta_impresas-i];j++)
					OLED_DrawPixel(x,(uint8_t)(SSD1306_HEIGHT-((SSD1306_HEIGHT*j)/6144)),White);
			}
			else
			{
				for(j=(uint32_t)smooth[IR][cuenta_impresas-i];j<=(uint32_t)smooth[IR][cuenta_impresas-i+ASDF];j++)
									OLED_DrawPixel(x,(uint8_t)(SSD1306_HEIGHT-((SSD1306_HEIGHT*j)/6144)),White);
			}


			x++;
			x= x%SSD1306_WIDTH;
			par=ASDF;
		}
		par--;



	}
	cuenta_impresas=0;

	OLED_SetCursor(0, 0);
	OLED_WriteString("Freq=",Font_7x10,White);
	OLED_WriteString(itoa(((int)bpm)%1000,str,10),Font_7x10,White);
	OLED_WriteString("bpm    ",Font_7x10,White);
	OLED_SetCursor(0, 11);
	OLED_WriteString("spO2=",Font_7x10,White);
	OLED_WriteString(itoa(((int)spo2%1000)%1000,str,10),Font_7x10,White);
	OLED_WriteString("%    ",Font_7x10,White);

	OLED_UpdateScreen();

}
