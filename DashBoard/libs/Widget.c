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

unsigned char contains( const Widget *w, TPoint *point){
	if	((point->x >= w->xl) && (point->x <= w->xl + w->xw) &&
		 (point->y >= w->yt) && (point->y <= w->yt + w->yh)) {
/*		debuginfo(5, point->x, point->y, 0);
		debuginfo(6, w->xl, w->yt, 0); */
		return 1;
	}
	else
		return 0;
}


unsigned char OnTouch(const Widget ws[], TPoint *press){
	unsigned char i, res;

	res = 0;
	for(i=0; i<NUMWIDGETS; i++) {
		if (ws[i].wt == BUTTONICON) {
			if(contains(&ws[i], press)) {
				SetEvent(biconinfo(&ws[i])->onpress);
 			    res = 1;
 			}
		}
		if (ws[i].wt == ICON) {
			if(contains(&ws[i], press)) {
			if(IsEvent(iconinfo(&ws[i])->onevent)){
				ClearEvent(iconinfo(&ws[i])->onevent);
				}else{
				SetEvent(iconinfo(&ws[i])->onevent);
				}
			res = 1;
			}
		}
	}
	return res;
}

unsigned char DrawInit( const Widget ws[])
{LCD_SetTextColor(White);
	unsigned char i;
	for(i=0; i<NUMWIDGETS; i++) {
		DrawOff(&ws[i]);
	}
	ChangeGear(&MyDashBoardScr[5], 0);
/*Draw the string "km" in order to avoid useless refreshing*/
	LCD_SetTextColor(Black);
	LCD_SetBackColor(Black);
	LCD_DrawFullRect(50, 200, 280, 30);
	LCD_SetTextColor(White);
    LCD_DisplayStringXY(210, 210, "Km");
    LCD_DisplayStringXY(40, 210, "Km");
    LCD_DisplayStringXY(150, 100, "Km");
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



