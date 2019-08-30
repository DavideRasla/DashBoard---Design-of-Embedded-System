/*!	\file	Button.h
 	\author Daniel Casini
 	\date	10/03/2014
	\brief	Button handling file for the watch
*/

#ifndef WIDGET_H_
#define WIDGET_H_

#include "Event.h"
#include "Touch.h"
#include "fonts.h"


typedef enum  {
	BACKGROUND, BUTTONICON, TEXT, IMAGE,IMAGE_ARR, ICON
} WidgetType;

typedef struct {
	unsigned char *image_on;
	unsigned char *image_off;
	Event 		  onevent;
} Icon;

typedef struct {
	unsigned char *image;
} Image;


typedef struct {
	unsigned char *N;
	unsigned char *One;
	unsigned char *Two;
	unsigned char *Three;
	unsigned char *Four;
	unsigned char *Five;
	unsigned char *Six;

} Image_ARRAY;


typedef struct {
	unsigned char *iconp;
	unsigned char *iconr;
	Event		  onpress;
} ButtonIcon;

typedef struct {
	sFONT *font;
	unsigned short int	color;
} Text;

typedef struct {
	unsigned short int xl;
	unsigned short int yt;
	unsigned short int xw;
	unsigned short int yh;
	WidgetType wt;
	void     *ws;
} Widget;

#define txtinfo(w) ((Text *)((w)->ws))
#define biconinfo(w) ((ButtonIcon *)((w)->ws))
#define iconinfo(w) ((Icon *)((w)->ws))
#define imginfo(w) ((Image *)((w)->ws))
#define imgGearinfo(w) ((Image_ARRAY *)((w)->ws))


unsigned char OnTouch( const Widget ws[], TPoint *press);
unsigned char DrawInit(const Widget ws[]);
unsigned char DrawOn( const Widget *w);
unsigned char DrawOff( const Widget *w);
unsigned char DrawIcons(const Widget w[]);
unsigned char DrawIcon(const Widget *w);
unsigned char WPrint( const Widget *w, char *s);
unsigned char ChangeGear( Widget *w, int Gear);

#endif /* BUTTON_H_ */
