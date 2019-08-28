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

Icon RightArrow = {
		RightArrow_ON, RightArrow_OFF, AR
};

Icon LeftArrow = {
		LeftArrow_ON, LeftArrow_OFF , AL
};

Icon Oil = {
		Low_Oil_ON, Low_Oil_OFF, OIL
};

Image_ARRAY RUN = {
		Neutral, First, Second, third, fourth, fifth, sixth
};



Icon Low_Fuel = {
		LowFuel_ON, LowFuel_OFF, FUEL
};



Text txt = {
		&Font32x48, White
};

Image backg = {
	ImageBuffer
};

const Widget MyWatchScr[NUMWIDGETS] = {
	/*  {x1, yt, xw, yh, WidgetType, void *ws}*/
		{0, 0, 320, 240, BACKGROUND, (void *)&backg},
		{270, 5, 40, 40, ICON, (void *)&RightArrow},
		{10, 5, 40, 40, ICON, (void *)&LeftArrow},
		{60, 20, 30, 16, ICON, (void *)&Oil},
		{230, 20, 30, 30, ICON, (void *)&Low_Fuel},
		{10, 80, 40, 40, IMAGE_ARR, (void *)&RUN},

};

