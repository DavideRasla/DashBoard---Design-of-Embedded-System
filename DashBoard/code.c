

#include "ee.h"
#include "ee_irq.h"
#include <stdio.h>
#include "stm32f4xx_conf.h"
#include "stm32f4_discovery.h"
#include "libs/stm32f4_discovery_lcd.h"
#include "stm32f4xx.h"

#include "config.h"
#include "libs/STMPE811QTR.h"
#include "assets/pictures.h"
#include "libs/Widget.h"
#include "libs/WidgetConfig.h"
#include "libs/Event.h"
#include "libs/debug.h"
#include "libs_custom/io.h"
#include "libs_custom/types.h"
#include "libs_custom/GUI.h"
#include "libs_custom/Sensors.h"


/*Global Variables*/


uint8_T hours=Initial_Hours, minutes=Initial_Minutes, seconds=Initial_Seconds;		// Clock variables
uint8_T   Fuel_Value   			= FULL_FUEL;										// Fuel value, inizializated to full
uint8_T   InstantSpeed  		= ZERO;												// Speed on the scren
uint32_T  Speedometer  			= SPEEDOMETER_INITIAL;								// Total KM counter
uint32_T  Partial_Speedometer  	= ZERO;												// Total partial KM counter. This can be resetted
uint16_T  RPM_Value    			= RPM_INITIAL;										// RPM of the engine
uint8_T   Actual_Gear 			= Neutral_Gear;										// Actual gear of the motorbike. Is shown on the screen
double    Actual_Accel 			= ZERO;												// Actual accelleration readed by the sensors
uint32_T  MetersTraveled 		= ZERO;												// Meters traveled since the Power On
uint8_T	  KmTraveled 			= ZERO;												// KM traveled since the Power On
bool_t 	  StopEngine 			= ZERO; 											// If equals to zero the engine can work, otherwise the engine has a problem or the motorbike is without fuel
bool_t Blink_Left 				= ZERO;												// If equals to one the left arrow can blink
bool_t Blink_Right				= ZERO;												// If equals to one the right arrow can blink
uint8_T time_Arrow 				= ZERO;												// Used to count the time in order to simulate the blinking of the arrow
bool_t Brights_Status 			= ZERO;												// Used to store the status of the brights

/*Events variables: When the task ReadSensor reads a valid input they are set to one */

bool_t Event_LeftArrow = ZERO;								
bool_t Event_RightArrow = ZERO;
bool_t Event_GearUp = ZERO;
bool_t Event_GearDown = ZERO;
bool_t Event_PartialKm_Reset = ZERO;
bool_t Event_Brights = ZERO;

/*
 * SysTick ISR2
 */
ISR2(systick_handler)
{
	/* count the interrupts, waking up expired alarms */
	CounterTick(myCounter);
}

/*!
 *  \brief Task that draws all the Graphic
 *  \return void
 */
TASK(TaskGuiDashboardSlow)
{
	UpdateFuel();
	UpdateSpeedoMeter();
	DrawIcons(MyDashBoardScr);
	UpdateTime();
} 
TASK(TaskGuiDashboardFast)
{
	UpdateMotorRPM();
	UpdateSpeedValue();
} 


/*!
 *  \brief Task that calculates and updates all the Variables used by TaskGuiDashboardSlow. 
 *   Also reads all the events from the buttons
 *  \return void
 */
TASK(TaskCheckEvents)
{
UpdateEngineResponse();
Update_Accell();
checkEvents();
}


/**/
TASK(TaskReadSensors){
ReadSensors();
}

/*
 * MAIN TASK
 */
int main(void)
{
	SystemInit();
  /*Initializes Erika related stuffs*/
	EE_system_init();

	/*Initialize systick */
	EE_systick_set_period(MILLISECONDS_TO_TICKS(1, SystemCoreClock));
	EE_systick_enable_int();
	EE_systick_start();
	/* Initializes LCD */
	IOE_Config();
	/* Initialize the LCD */
	STM32f4_Discovery_LCD_Init();
	/* Draw the background*/
	DrawInit(MyDashBoardScr);
	LCD_SetTextColor(White);
	//Initialize gpio
	io_init();
	/* Program cyclic alarms which will fire after an initial offset,
	 * and after that periodically
	 * */
	SetRelAlarm(AlarmTaskCheckEvents, TASKOFFSET, TaskCheckEvents_FREQ);
	SetRelAlarm(AlarmTaskReadSensors, TASKOFFSET, TASKGUI_ReadSensors_FREQ);
	SetRelAlarm(AlarmTaskGuiDashboardSlow, TASKOFFSET, TASKGUI_FREQ_SLOW);
	SetRelAlarm(AlarmTaskGuiDashboardFast, TASKOFFSET, TASKGUI_FREQ_FAST);

  /* Forever loop: background activities (if any) should go here */
	for (;;) {
	}

}


