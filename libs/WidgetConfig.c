/*
 * WidgetConfig.c
 *
 *  Created on: 21/ott/2015
 *      Author: admim
 */

#include "Widget.h"
#include "WidgetConfig.h"
#include "../assets/pictures.h"
#include <stdio.h>
#include "stm32f4_discovery_lcd.h"
#include "fonts.h"

Image RightArrow = {
		RightArrow_ON, RightArrow_OFF
};

Image LeftArrow = {
		LeftArrow_ON, LeftArrow_OFF
};
Image Oil = {
		Low_Oil_ON, Low_Oil_OFF
};

Image RUN = {
		Neutral, First, Second, third, fourth, fifth, sixth
};



Image Low_Fuel = {
		LowFuel_ON, LowFuel_OFF
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
		{270, 5, 40, 40, IMAGE, (void *)&RightArrow},
		{10, 5, 40, 40, IMAGE, (void *)&LeftArrow},
		{60, 20, 30, 16, IMAGE, (void *)&Oil},
		{110, 20, 30, 30, IMAGE, (void *)&Low_Fuel},
		{140, 200, 40, 40, IMAGE, (void *)&RUN},

};

