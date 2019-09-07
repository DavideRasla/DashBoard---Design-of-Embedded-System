/*
 * Widget.c
 *
 *  Created on: 21/ott/2015
 *      Author: admim
 */

#include "Widget.h"
#include "WidgetConfig.h"
#include "Event.h"
#include "debug.h"
#include "stm32f4_discovery_lcd.c"
#include "config.h"




unsigned char DrawInit( const Widget ws[])
{

	LCD_SetTextColor(White);
	unsigned char i;
	unsigned char watchstr[20];

	for(i=0; i<NUMWIDGETS; i++) {
		DrawOff(&ws[i]);
	}
	ChangeGear(&MyDashBoardScr[5], 0);
/*Draw the string "km" in order to avoid useless refreshing*/
	LCD_SetTextColor(Black);
	LCD_SetBackColor(Black);
	LCD_DrawFullRect(50, 200, 280, 30);
	LCD_DrawFullRect(100, 20, 100, 30);
	LCD_SetTextColor(White);
    LCD_DisplayStringXY(210, 210, "Km");
    LCD_DisplayStringXY(40, 210, "Km");
    LCD_DisplayStringXY(150, 100, "Km");

 /*Draw the clock for the first time*/
 uint8_t hours=Initial_Hours, minutes=Initial_Minutes, seconds=Initial_Seconds;
 sprintf(watchstr, "%2d:%2d:%2d", hours, minutes, seconds);
 LCD_DisplayStringXY(100, 20, watchstr);
	return 1;
}


unsigned char DrawOn(const Widget *w)
{
	char *imgptr = 0;
	switch (w->wt) {
	case BUTTONICON:
		imgptr = biconinfo(w)->iconp;
		break;
	case BACKGROUND:
	case IMAGE:
		imgptr = imginfo(w)->image;
		break;
	}
	if (imgptr != 0) {
		LCD_DrawPicture(w->xl, w->yt, w->xw, w->yh, imgptr);
		//debugInt(0,0, (int)w->xl,(int) w->yt,(int) imgptr);
		return 1;
	} else
	//debugInt(0,0, (int)w->xl,(int)w->yt, (int)imgptr);
		return 0;
}

unsigned char DrawOff(const Widget *w)
{
	char *imgptr = 0;
LCD_SetTextColor(White);
	switch (w->wt) {
	case BUTTONICON:
		imgptr = biconinfo(w)->iconr;
		break;
	case BACKGROUND:
	case IMAGE:
		imgptr = imginfo(w)->image;
		break;
	}
	if (imgptr != 0) {
		LCD_DrawPicture(w->xl, w->yt, w->xw, w->yh, imgptr);
		//debugInt(0,0, (int)w->xl, (int)w->yt, 0);
		return 1;
	}
	//debugInt(0,0,(int) w->xl,(int) w->yt, 1);
		return 0;
}

unsigned char ChangeGear(  Widget *w, int Gear)
 {
char *imgptr = 0;
 LCD_SetTextColor(White);
      switch (Gear) {
               case 0:
                       imgptr = imgGearinfo(w)->N;
                       break;
               case 1:
                       imgptr = imgGearinfo(w)->One;
                       break;
               case 2:
                       imgptr = imgGearinfo(w)->Two;
                       break;
               case 3:
                       imgptr = imgGearinfo(w)->Three;
                       break;
               case 4:
                       imgptr = imgGearinfo(w)->Four;
                       break;
               case 5:
                       imgptr = imgGearinfo(w)->Five;
                       break;
               case 6:
                       imgptr = imgGearinfo(w)->Six;
                       break;
       }
       if (imgptr != 0) {
               LCD_DrawPicture(w->xl, w->yt, w->xw, w->yh, imgptr);
               return 1;
       }
                return 0;
}


unsigned char DrawIcon(const Widget *w){
	char *imgptr = 0;
	if(w->wt == ICON){
		if(IsEvent(iconinfo(w)->onevent)){
			imgptr = iconinfo(w)->image_on;
		}
		else{
			imgptr = iconinfo(w)->image_off;
		}
	}
	if (imgptr != 0)
		LCD_DrawPicture(w->xl, w->yt, w->xw, w->yh, imgptr);

	return 1;
}


unsigned char DrawIcons(const Widget ws[]){
	LCD_SetTextColor(White);
	unsigned char i;

	for(i=0; i<NUMWIDGETS; i++) {
		DrawIcon(&ws[i]);
	}
	return 1;
}

unsigned char WPrint(const Widget *w, char *s)
{
	if (w->wt == TEXT) {
		LCD_SetTextColor(White);
		LCD_SetFont(txtinfo(w)->font);
		LCD_DisplayStringXY(w->xl, w->yt, s);
		return 1;
	} else
		return 0;
}




