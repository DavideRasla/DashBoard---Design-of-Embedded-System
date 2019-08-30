

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
#include "libs/rtwtypes.h"
#include "libs/debug.h"



const char *RT_MEMORY_ALLOCATION_ERROR;


uint8_T hours=0, minutes=0, seconds=0, tenths=0, mode=0;

boolean_T DashBoardMode;
uint8_T IstantSpeed = 0;
uint32_t Speedometer = 128000;
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
		LCD_SetTextColor(Yellow);
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

void UpdateMotorRPM(int value){

	if( value<=5000 ){
		LCD_SetTextColor(Black);
		LCD_SetBackColor(Black);
		LCD_DrawFullRect(241, 50, 44,180);
		LCD_SetTextColor(Green);
		LCD_SetBackColor(Black);
		LCD_DrawFullRect(241, 190 - (value/100) , 44, (value/100) );
	}else if(value >=5000 && value< 8000){
		LCD_SetTextColor(Black);
		LCD_SetBackColor(Black);
		LCD_DrawFullRect(241, 50, 44,190);
		LCD_SetTextColor(Yellow);
		LCD_SetBackColor(Black);
		LCD_DrawFullRect(241, 190 - (value/100), 44,(value/100));
		LCD_SetTextColor(Green);
		LCD_SetBackColor(Black);
		LCD_DrawFullRect(241, 140, 44, 50);
	}else{
		LCD_SetTextColor(Black);
		LCD_SetBackColor(Black);
		LCD_DrawFullRect(241, 50, 44,190);
		LCD_SetTextColor(Red);
		LCD_SetBackColor(Black);
		LCD_DrawFullRect(241, 190 - (value/100), 44,(value/100));
		LCD_SetTextColor(Yellow);
		LCD_SetBackColor(Black);
		LCD_DrawFullRect(241, 110, 44, 30);
		LCD_SetTextColor(Green);
		LCD_SetBackColor(Black);
		LCD_DrawFullRect(241, 140, 44, 50);


	}
}

void UpdateSpeed(){
char text[20];
	LCD_SetTextColor(Black);
	LCD_SetBackColor(Black);
	LCD_DrawFullRect(90, 100, 120, 30);
	LCD_SetTextColor(White);
   	sprintf((char*)text,"%d Km/h", IstantSpeed);
    LCD_DisplayStringXY(90, 100, text);
}

void UpdateSpeedoMeter(){
char text[20];
	LCD_SetTextColor(Black);
	LCD_SetBackColor(Black);
	LCD_DrawFullRect(100, 200, 100, 30);
	LCD_SetTextColor(White);
   	sprintf((char*)text,"%d Km", Speedometer);
    LCD_DisplayStringXY(100, 210, text);
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
	static uint32_t RPM_Value = 3000;
	static uint8_T gear = 0;
	UpdateTime();
	UpdateFuel(Fuel_Value++);
	UpdateMotorRPM(RPM_Value);
	ChangeGear(&MyWatchScr[5], gear++);
	UpdateSpeed();
	UpdateSpeedoMeter();
	DrawIcons(MyWatchScr);

	IstantSpeed++;
	Speedometer++;
	RPM_Value = 10500;

	if(Fuel_Value == 100 )
		Fuel_Value = 0;

	if(gear == 7 )
		gear = 0;

	if(IstantSpeed == 50){
		IstantSpeed = 0;
		RPM_Value = 5000;
	}



	if(Speedometer == 130000)
		Speedometer = 0;
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
	

	LCD_SetTextColor(White);
	//WPrint(&MyWatchScr[SEP1STR], ":");
	//WPrint(&MyWatchScr[SEP2STR], ":");

	/* Program cyclic alarms which will fire after an initial offset,
	 * and after that periodically
	 * */
	SetRelAlarm(AlarmTaskLCD, 10, 50);
	SetRelAlarm(AlarmTaskGuiDashboard, 10, 100);

  /* Forever loop: background activities (if any) should go here */
	for (;;) {
	}

}


