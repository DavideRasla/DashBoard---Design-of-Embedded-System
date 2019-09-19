/*
 * debug.c
 *
 *  Created on: 25/ott/2015
 *      Author: admim
 */
#include "stm32f4_discovery_lcd.h"
#include <stdio.h>
#include "../libs_custom/types.h"
void debuginfo(int id, int a, int b, int c)
{
	char text[80];

    sprintf((char*)text,"a = %d b = %d c = %d", a, b, c);
	LCD_DisplayStringLine(LINE(id),text);
}

void debugInt(int Xpos, int Ypos, int a, int b, int c, int d, int e)
{
	char text[80];
	LCD_SetTextColor(Black);
	LCD_SetBackColor(Black);
	LCD_DrawFullRect(Xpos, Ypos, 200, 40);
	LCD_SetTextColor(White);
   sprintf((char*)text,"%d, %d,%d,%d,%d", a, b, c, d, e);

    LCD_DisplayStringXY(Xpos, Ypos, text);
}

void debug(uint32_T a){
char text[20];
	LCD_SetTextColor(Black);
	LCD_SetBackColor(Black);
	LCD_DrawFullRect(50, 130, 100, 40);
	LCD_SetTextColor(White);
   	sprintf((char*)text,"%d", a);
    LCD_DisplayStringXY(50, 130, text);
}
