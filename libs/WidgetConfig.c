/*
 * WidgetConfig.c
 *
 *  Created on: 21/ott/2015
 *      Author: admim
 */

#include "Widget.h"
#include "WidgetConfig.h"
#include "assets/pictures.h"
#include <stdio.h>
#include "stm32f4_discovery_lcd.h"
#include "fonts.h"

ButtonIcon RightArrow = {
		RightArrow_ON, RightArrow_OFF
};


Text txt = {
		&Font32x48, White
};

Image backg = {
	ImageBuffer
};

const Widget MyWatchScr[NUMWIDGETS] = {
		{0, 0, 320, 240, BACKGROUND, (void *)&backg},
		{70, 70, 25, 21, BUTTONICON, (void *)&RightArrow},
/*		{98, 188, 39, 43, BUTTONICON, (void *)&buttontimeset},
		{169, 188, 38, 43, BUTTONICON, (void *)&buttonalarm},
		{244, 188, 35, 43, BUTTONICON, (void *)&buttonDashBoard},
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

