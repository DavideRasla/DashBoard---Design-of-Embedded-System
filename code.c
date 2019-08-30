

#include "ee.h"
#include "ee_irq.h"
#include <stdio.h>
#include "stm32f4xx_conf.h"
#include "stm32f4_discovery.h"
#include "libs/stm32f4_discovery_lcd.h"
#include "stm32f4xx.h"

#include "libs/STMPE811QTR.h"
#include "assets/pictures.h"
#include "libs/Widget.h"
#include "libs/WidgetConfig.h"
#include "libs/Touch.h"
#include "libs/Event.h"
#include "libs/debug.h"
#include "libs_custom/io.h"
#include "libs_custom/types.h"



const char *RT_MEMORY_ALLOCATION_ERROR;


uint8_T hours=0, minutes=0, seconds=0, tenths=0, mode=0;
bool_t plusButton;
bool_t minusButton;
bool_t timeMode;
bool_t timesetmode;
bool_t alarmMode;
bool_t DashBoardMode;

/*
 * SysTick ISR2
 */
ISR2(systick_handler)
{
	/* count the interrupts, waking up expired alarms */
	CounterTick(myCounter);
}

/*
 * TASK LCD
 */
TASK(TaskLCD)
{
	unsigned int px, py;
	TPoint p;
	if (GetTouch_SC_Async(&px, &py)) {
		p.x = px;
		p.y = py;
		OnTouch(MyWatchScr, &p);
	}
}

void setTimeString(char *watchstr, uint8_T hours, uint8_T minutes, uint8_T seconds, uint8_T tenths, uint8_T mode)
{
	sprintf(watchstr, "%2d:%2d:%2d", hours, minutes, seconds);
}
/*
 * TASK Clock
 */
unsigned char IsUpdateTime()
{
	unsigned char res;
	static unsigned char oh=0, om=0, os=0;
	if (hours!=oh || minutes!=om || seconds!= os)
		res = 1;
	else
		res = 0;
	oh = hours;
	om = minutes;
	os = seconds;
	return res;
}

void UpdateTime()
{
	unsigned char watchstr[20];
	setTimeString(watchstr, hours, minutes, seconds, tenths, mode);


	//WPrint(&MyWatchScr[1], watchstr); 
}
/**
  * @brief  Updates the fuel bar
  * @param  Value: specifies the quantity of fuel [0, 105]
  * @retval None
  */
void UpdateFuel(int value){

	if( value < 30){
		LCD_SetTextColor(Red);
		LCD_SetBackColor(Black);
		LCD_DrawFullRect(110, 180, value, 10);
		LCD_SetBackColor(Black);
		LCD_SetTextColor(Black);
		LCD_DrawFullRect(110 + value, 180, 100 - value, 10);
	}else if(value >30 && value< 70){
		LCD_SetTextColor(yellow);
		LCD_SetBackColor(Black);
		LCD_DrawFullRect(110, 180, value, 10);
		LCD_SetBackColor(Black);
		LCD_SetTextColor(Black);
		LCD_DrawFullRect(110 + value, 180, 100 - value, 10);
	}else{
		LCD_SetTextColor(Green);
		LCD_SetBackColor(Black);
		LCD_DrawFullRect(110, 180, value, 10);
		LCD_SetBackColor(Black);
		LCD_SetTextColor(Black);
		LCD_DrawFullRect(110 + value, 180, 100 - value, 10);
	}
}

void checkButton(){
	if(button_read() == true){
		LCD_SetTextColor(White);
		LCD_SetBackColor(Black);
		debugInt(60, 60, 8, 8, 8);
	}
}

void strencode2digit(char *str, int digit)
{
	str[2]=0;
	str[0]=digit/10+'0';
	str[1]=digit%10+'0';
}
void UpdateArrows(){
	static int Angle = 0;
	//LCD_DrawLineByAngle(160, 120, 100, 270);

Angle++;
	if(Angle < 360 && Angle > 0){
		//LCD_DrawLineByAngle(160, 120, 100, Angle);
	}
	if(Angle == 360)
		Angle = 0;
}


TASK(TaskGuiDashboard)
{
	static uint8_T Fuel_Value = 0;
	static uint8_T gear = 0;
	UpdateTime();
	UpdateFuel(Fuel_Value++);
	ChangeGear(&MyWatchScr[5], gear++);
	if(Fuel_Value == 100 ){
		Fuel_Value = 0;
	}
	if(gear == 7 ){
		gear = 0;
	}
	checkButton();
} 

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */



/*
 * MAIN TASK
 */
int main(void)
{
	//GPIO_InitTypeDef GPIO_InitStructure;

	SystemInit();
  /*Initializes Erika related stuffs*/
	EE_system_init();



	/*Initialize systick */
	EE_systick_set_period(MILLISECONDS_TO_TICKS(1, SystemCoreClock));
	EE_systick_enable_int();
	EE_systick_start();

	/* Initializes LCD and touchscreen */
	IOE_Config();
	/* Initialize the LCD */
	STM32f4_Discovery_LCD_Init();

	InitTouch(-0.102, 0.0656, -310, 16);

	/* Draw the background */
	DrawInit(MyWatchScr);
	/*Draw the icons*/
	DrawIcons(MyWatchScr);

	LCD_SetTextColor(White);
	//WPrint(&MyWatchScr[SEP1STR], ":");
	//WPrint(&MyWatchScr[SEP2STR], ":");

	//Initialize gpio
	io_init();

	/* Program cyclic alarms which will fire after an initial offset,
	 * and after that periodically
	 * */
	SetRelAlarm(AlarmTaskLCD, 10, 50);
	SetRelAlarm(AlarmTaskGuiDashboard, 10, 100);

  /* Forever loop: background activities (if any) should go here */
	for (;;) {
	}

}


