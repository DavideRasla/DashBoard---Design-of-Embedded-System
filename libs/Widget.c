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

unsigned char contains( Widget *w, TPoint *point){
	if	((point->x >= w->xl) && (point->x <= w->xl + w->xw) &&
		 (point->y >= w->yt) && (point->y <= w->yt + w->yh)) {
/*		debuginfo(5, point->x, point->y, 0);
		debuginfo(6, w->xl, w->yt, 0); */
		return 1;
	}
	else
		return 0;
}

unsigned char OnTouch( Widget ws[], TPoint *press){
	unsigned char i, res;

	res = 0;
	for(i=0; i<NUMWIDGETS; i++) {
		if (ws[i].wt == BUTTONICON) {
			if(contains(&ws[i], press)) {
				SetEvent(biconinfo(&ws[i])->onpress);
 			    res = 1;
 			}
		}
	}
	return res;
}

unsigned char DrawInit( Widget ws[])
{LCD_SetTextColor(White);
	unsigned char i;
	for(i=0; i<NUMWIDGETS; i++) {
		DrawOff(&ws[i]);
	}
	return 1;
}

unsigned char DrawOn( Widget *w)
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

unsigned char DrawOff( Widget *w)
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
/**
  * @brief  Change the gear on the screem
  * @param  ws[5], N = 0; 1 = 1; 2 = 2...
  * @retval None
  */
unsigned char ChangeGear( Widget *w, int Gear)//ws[5],
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

unsigned char DrawIcons( Widget ws[]){
	LCD_SetTextColor(White);
	unsigned char i;
	char *imgptr = 0;
	for(i=0; i<NUMWIDGETS; i++) {
		if(ws[i].wt == ICON){
			if(IsEvent(iconinfo(ws[i])->onevent)){
				imgptr = iconinfo(ws[i])->image_on;
			}
			else{
				imgptr = iconinfo(ws[i])->image_off;
			}
		}
		if (imgptr != 0) 
			LCD_DrawPicture(ws[i].xl, ws[i].yt, ws[i].xw, ws[i].yh, imgptr);
		
			imgptr = 0;
	}
	return 1;
}

unsigned char WPrint( Widget *w, char *s)
{
	if (w->wt == TEXT) {
		LCD_SetTextColor(txtinfo(w)->color);
		LCD_SetFont(txtinfo(w)->font);
		LCD_DisplayStringXY(w->xl, w->yt, s);
		return 1;
	} else
		return 0;
}




