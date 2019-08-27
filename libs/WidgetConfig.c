/*
 * WidgetConfig.c
 *
 *  Created on: 21/ott/2015
 *      Author: admim
 */

#include "Widget.h"
#include "WidgetConfig.h"
#include "assets\pictures.h"
#include <stdio.h>
#include "stm32f4_discovery_lcd.h"
#include "fonts.h"

Image RightArrow = {
		RightArrow_ON, RightArrow_OFF
};

Image Oil = {
		Low_Oil, Low_Oil
};

Image Neut = {
		Neutral, Neutral
};



Text txt = {
		&Font32x48, White
};

Image backg = {
	ImageBuffer, ImageBuffer
};

const Widget MyWatchScr[NUMWIDGETS] = {
	/*  {x1, yt, xw, yh, WidgetType, void *ws}*/
		{0, 0, 320, 240, BACKGROUND, (void *)&backg},
		{200, 150, 40, 40, IMAGE, (void *)&Neut},
		//{270, 30, 40, 40, IMAGE, (void *)&RightArrow},
		{10, 10, 30, 16, IMAGE, (void *)&Oil},
/*		{244, 188, 35, 43, BUTTONICON, (void *)&buttonDashBoard},
		{270, 30, 40, 40, BUTTONICON, (void *)&buttonplus},
		{270, 105, 40, 40, BUTTONICON, (void *)&buttonminus},
//		{26, 76, 100, 32, RECTANGLE, (void *)&rectangle}
		{29, 70, 40, 40, TEXT, (void *)&txt},
		{99, 70, 40, 40, TEXT, (void *)&txt},
		{168, 70, 40, 40, TEXT, (void *)&txt},
		{243, 70, 40, 40, TEXT, (void *)&txt},
		{80, 70, 40, 40, TEXT, (void *)&txt},
		{149, 70, 40, 40, TEXT, (void *)&txt},*/
};

