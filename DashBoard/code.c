

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
 *  \brief Calculates the amount of Meters Traveled in 2 seconds 
 *  \return void
 */
void Calculate_MetersTraveled(){
static uint8_T i 		= ONE;
static uint8_T Speed1   = ZERO;
static uint8_T Speed2   = ZERO;
static uint8_T Average  = ZERO;

	if( i!=2 ){ //First call: simulate an instant t0 
	Speed1 = InstantSpeed;
	}else{ 		//Second call: simulate an istant t1
	i = ZERO;
	Speed2 = InstantSpeed;
	Average = (Speed1 + Speed2)/2;
	MetersTraveled = MetersTraveled +  2*Average/3.6;//From km/h to m/s. Calculate for 2 seconds
	if(MetersTraveled > ONE_KM){
		Speedometer++;
		Partial_Speedometer++;
		KmTraveled++;
		MetersTraveled = MetersTraveled % ONE_KM; //Save just the meters

		if( KmTraveled % CONSUMPTION_LITRE==ZERO ){	// if kmTraveled is a multiple of the km/litre
			Fuel_Value= Fuel_Value - 20;
			if(Fuel_Value == ZERO){
				Fuel_Value= ONE;
				StopEngine = ONE;
				RPM_Value = RPM_INITIAL;
				}
			}
		}
	}
	i++;

	if( Partial_Speedometer>Oil_MustBe_Refilled ){//if there is not a refill before X km, the engine can work but the oil icon is on
		SetEvent(iconinfo(&MyDashBoardScr[3])->onevent);
	}else{
		ClearEvent(iconinfo(&MyDashBoardScr[3])->onevent);
	}
	if(Partial_Speedometer> Oil_MustBe_Refilled + Km_Before_Crash ){ //if there is not a a refill at all the engine melts 
		StopEngine = ONE;
	}
}

/*!
 *  \brief Updates the clock on the top of the DashBoard. Also calls Calculate_MetersTraveld 
 *  \return void
 */
void UpdateTime()
{
static uint8_T i = ZERO;
unsigned char Seconds_Str[5];
unsigned char Minutes_Str[5];
unsigned char Hours_Str[5];
bool_t inc_m = ZERO;
bool_t inc_h = ZERO;

	if(i == 1){
	i = ZERO;
	seconds++;
	if(seconds == 60){
		seconds = ZERO;
		minutes++;
		inc_m = ONE;

		if (minutes == 60){
			minutes = ZERO;
			hours++;
		inc_h = ONE;
		}
		if (hours == 99){
			hours = ZERO;
		}
		}
	Calculate_MetersTraveled(); 	//Called here in order to simulate the evolution in the time
	}
i++;

	sprintf(Seconds_Str, ":%2d", seconds);
	sprintf(Minutes_Str, "%2d", minutes);
	sprintf(Hours_Str, "%2d:", hours);
	LCD_SetTextColor(Black);
	LCD_SetBackColor(Black);
	LCD_DrawFullRect(190, 20, 40, 30);
	LCD_SetTextColor(White);
    LCD_DisplayStringXY(180, 20, Seconds_Str);  

    if(inc_m==ONE){				//in order to avoid useless refresh, draws the minutes only if needed
	LCD_SetTextColor(Black);
	LCD_SetBackColor(Black);
	LCD_DrawFullRect(150, 20, 30, 30);
	LCD_SetTextColor(White);
   		LCD_DisplayStringXY(150, 20, Minutes_Str);
   	}
   	if(inc_h==ONE){				//in order to avoid useless refresh, draws the hours only if needed
   		LCD_SetTextColor(Black);
		LCD_SetBackColor(Black);
		LCD_DrawFullRect(90, 20, 50, 30);
		LCD_SetTextColor(White);
   		LCD_DisplayStringXY(100, 20, Hours_Str); 
   		}  
   		
}
/*!
 *  \brief: Reads the variable 'Fuel_Value' and draws the relative bar.
 *  \return void
 */
void UpdateFuel(){

	if( Fuel_Value>=ZERO ){
			if( Fuel_Value < RED_FUEL){
				LCD_SetTextColor(Red);
				LCD_SetBackColor(Black);
				LCD_DrawFullRect(110, 180, Fuel_Value, 10);
				LCD_SetBackColor(Black);
				LCD_SetTextColor(Black);
				LCD_DrawFullRect(110 + Fuel_Value, 180, 100 - Fuel_Value, 10);
				SetEvent(FUEL);
			}else if(Fuel_Value >=RED_FUEL && Fuel_Value<= YELLOW_FUEL){			
				LCD_SetTextColor(Yellow);
				LCD_SetBackColor(Black);
				LCD_DrawFullRect(110, 180, Fuel_Value, 10);
				LCD_SetBackColor(Black);
				LCD_SetTextColor(Black);
				LCD_DrawFullRect(110 + Fuel_Value, 180, 100 - Fuel_Value, 10);			
			}else{			
				LCD_SetTextColor(Green);
				LCD_SetBackColor(Black);
				LCD_DrawFullRect(110, 180, Fuel_Value, 10);
				LCD_SetBackColor(Black);
				LCD_SetTextColor(Black);
				LCD_DrawFullRect(110 + Fuel_Value, 180, 100 - Fuel_Value, 10);
			}
		}
}
/*!
 *  \brief Checks all the events from the buttons.
 *  \return void
 */
void checkEvents(){
	if(Event_LeftArrow){ //if the task readSensors reads a valid input, then the left arrow must blink
		Event_LeftArrow = ZERO;		//To avoid multiple commands
		Blink_Left = !Blink_Left; 	//if was ON then reset, otherwise set
		Blink_Right = ZERO;			//The arrows cannot be on at the same time
		ClearEvent(iconinfo(&MyDashBoardScr[1])->onevent);
		ClearEvent(iconinfo(&MyDashBoardScr[2])->onevent);
		time_Arrow = ZERO;
		}

	if(Event_RightArrow ){//if the task readSensors reads a valid input, then the right arrow must blink
		Event_RightArrow = ZERO; 	//To avoid multiple commands
		Blink_Right = !Blink_Right; //if was ON then reset, otherwise set
		Blink_Left = ZERO;			//The arrows cannot be on at the same time
		ClearEvent(iconinfo(&MyDashBoardScr[1])->onevent);
		ClearEvent(iconinfo(&MyDashBoardScr[2])->onevent);
		time_Arrow = ZERO;
		}	

	if(Blink_Left == 1){
			if( time_Arrow==10 ){//10 * 50ms (Task_Frequency) = 0,5 second
				SetEvent(iconinfo(&MyDashBoardScr[2])->onevent);
				}
			else if( time_Arrow==20 ){//20 * 50ms (Task_Frequency) = 1 second
				ClearEvent(iconinfo(&MyDashBoardScr[2])->onevent);
				time_Arrow = ZERO;	//restart the counter
				}
			time_Arrow++;
	}		
	if(Blink_Right == 1){
			if( time_Arrow==10 ){//10 * 50ms (Task_Frequency) = 0,5 second
				SetEvent(iconinfo(&MyDashBoardScr[1])->onevent);
				}
			else if (time_Arrow==20 ){//20 * 50ms (Task_Frequency) = 1 second
				ClearEvent(iconinfo(&MyDashBoardScr[1])->onevent);
				time_Arrow = ZERO;
				}
			time_Arrow++;	
	}


	

	if( Event_Brights ){//if the task readSensors reads a valid input, then the right arrow must blink
		Event_Brights = ZERO;
		Brights_Status = !Brights_Status; 
		if(Brights_Status)//if was OFF
			SetEvent(iconinfo(&MyDashBoardScr[7])->onevent);
		else //if was ON
			ClearEvent(iconinfo(&MyDashBoardScr[2])->onevent);
		}
		
	if(Event_GearUp && Actual_Gear <= Sixth_Gear && Clutch_Read()==1 ){
		Event_GearUp = ZERO;
		Actual_Gear++;
		ChangeGear(&MyDashBoardScr[5], Actual_Gear);
		InstantSpeed = InstantSpeed / 2;
		if((RPM_Value / 2)>RPM_MIN)
			RPM_Value = RPM_Value / 2;

	}
	if(Event_GearDown && Actual_Gear > Neutral_Gear && Clutch_Read()==1 ){
		Event_GearDown = ZERO;
		Actual_Gear--;
		ChangeGear(&MyDashBoardScr[5], Actual_Gear);
		InstantSpeed = InstantSpeed / 2;
		if( (RPM_Value/2)>RPM_MIN )
			RPM_Value = RPM_Value / 2;
	}
	if(Event_PartialKm_Reset && StopEngine == ZERO){//Reset the partial km
		Event_PartialKm_Reset = ZERO;
		Partial_Speedometer = ZERO;
	}
}


/*!
 *  \brief Reads the variable 'RPM_VALUE' and draws the relative bar.
 *  \return void
 */
void UpdateMotorRPM(){
	if(StopEngine == 0 ){ /*If the Engine is in a valid state*/
		if( RPM_Value>RPM_MIN && RPM_Value<RPM_LOW ){
			LCD_SetTextColor(Black);
			LCD_SetBackColor(Black);
			LCD_DrawFullRect(241, 50, 44,180);
			LCD_SetTextColor(Green);
			LCD_SetBackColor(Black);
			LCD_DrawFullRect(241, 190 - (RPM_Value/100) , 44, (RPM_Value/100) );
		}else if( RPM_Value>=RPM_LOW && RPM_Value<RPM_MEDIUM ){
			LCD_SetTextColor(Black);
			LCD_SetBackColor(Black);
			LCD_DrawFullRect(241, 50, 44,190);
			LCD_SetTextColor(Yellow);
			LCD_SetBackColor(Black);
			LCD_DrawFullRect(241, 190 - (RPM_Value/100), 44,(RPM_Value/100));
			LCD_SetTextColor(Green);
			LCD_SetBackColor(Black);
			LCD_DrawFullRect(241, 140, 44, 50);	
		}else if( RPM_Value<RPM_MAX ){
			LCD_SetTextColor(Black);
			LCD_SetBackColor(Black);
			LCD_DrawFullRect(241, 50, 44,190);
			LCD_SetTextColor(Red);
			LCD_SetBackColor(Black);
			LCD_DrawFullRect(241, 190 - (RPM_Value/100), 44,(RPM_Value/100));
			LCD_SetTextColor(Yellow);
			LCD_SetBackColor(Black);
			LCD_DrawFullRect(241, 110, 44, 30);
			LCD_SetTextColor(Green);
			LCD_SetBackColor(Black);
			LCD_DrawFullRect(241, 140, 44, 50);	
		}	
	}else{ /*No rpm is possible*/
			LCD_SetTextColor(Black);
			LCD_SetBackColor(Black);
			LCD_DrawFullRect(241, 50, 44,180);
			LCD_SetTextColor(Black);
			LCD_SetBackColor(Black);
			LCD_DrawFullRect(241, 190 - (RPM_LOW/100) , 44, (RPM_LOW/100) );
	}
	
}

/*!
 *  \brief Read from the sensor the amount of Throttle and calculates the actual acceleration of the motorbike.
 *  \return void
 */
void Update_Accell(){
	Actual_Accel = (double)(Throttle_Read())/RPM_Value;
}
/*!
 *  \brief Reads the variable 'Actual_Accel' and calculate the new speed
 *  \return void
 */
void UpdateSpeedValue(){
char text[20];
	LCD_SetTextColor(Black);
	LCD_SetBackColor(Black);
	LCD_DrawFullRect(90, 100, 50, 30);
	LCD_SetTextColor(White);

if( StopEngine == ZERO ){//if engine can work
	if(Clutch_Read()==0 && Actual_Gear != Neutral_Gear){//if the clutch is not on AND if the gear is not in neutral
					//debug(Actual_Accel);
						if(Actual_Accel>ZERO) {//Positive accelleration, need to increase the speed
								if(InstantSpeed == ZERO){
									InstantSpeed = 5;
								}
								if(InstantSpeed < SPEED_MAX/10){ //low speed
										InstantSpeed = 5 + InstantSpeed + ((int)(InstantSpeed  *1.4*Actual_Accel));
								}
								if(InstantSpeed >= SPEED_MAX/10 && InstantSpeed < SPEED_MAX/3){ //low speed
										InstantSpeed = InstantSpeed + ((int)(InstantSpeed *1.1*Actual_Accel));
								}
								if(InstantSpeed >= SPEED_MAX/3 && InstantSpeed < SPEED_MAX/2){ //medium speed
										InstantSpeed = InstantSpeed +  ((int)(InstantSpeed * 0.6*Actual_Accel));
								}
								if(InstantSpeed >= SPEED_MAX/2 && InstantSpeed < SPEED_MAX){ //high speed
									if( (uint8_T)(InstantSpeed + InstantSpeed * Actual_Accel) < SPEED_MAX)
										InstantSpeed = InstantSpeed +  ((int)(InstantSpeed * 0.25*Actual_Accel));
								}
						}else{//Negative accelleration, need to reduce the speed
									if(InstantSpeed < SPEED_MAX/10){ //very low speed
										if( (uint8_T)(InstantSpeed - 0.6*InstantSpeed)>0 ){
											InstantSpeed =  InstantSpeed - 0.6*InstantSpeed;
											if(InstantSpeed < 7){
											InstantSpeed = 0; //Stop
											}
										}
									}
									if(InstantSpeed >= SPEED_MAX/10 && InstantSpeed < SPEED_MAX/5){ //low speed
										if( (uint8_T)(InstantSpeed + InstantSpeed * Actual_Accel) < SPEED_MAX && (uint8_T)(InstantSpeed + InstantSpeed * Actual_Accel)>0 )
											InstantSpeed = InstantSpeed + ((int)(InstantSpeed *0.3*Actual_Accel));
									}
									if(InstantSpeed >= SPEED_MAX/5 && InstantSpeed < SPEED_MAX/2){ //medium speed
										if( (uint8_T)(InstantSpeed + InstantSpeed * Actual_Accel) < SPEED_MAX && (uint8_T)(InstantSpeed + InstantSpeed * Actual_Accel)>0 )
											InstantSpeed = InstantSpeed +  ((int)(InstantSpeed * 0.7*Actual_Accel));
									}
									if(InstantSpeed >= SPEED_MAX/3 && InstantSpeed <= SPEED_MAX){ //high speed
										if( (uint8_T)(InstantSpeed + InstantSpeed * Actual_Accel) < SPEED_MAX && (uint8_T)(InstantSpeed + InstantSpeed * Actual_Accel)>0 )
											InstantSpeed = InstantSpeed +  ((int)(InstantSpeed * 0.9*Actual_Accel));
									}
							}
						}else{//Clutch is ON, slow 10%
								InstantSpeed = InstantSpeed -  ((int)(InstantSpeed * 0.07)); //slow 10%
						}
}else{/*No Fuel || No Oil*/
		InstantSpeed = 0;
	}
	sprintf((char*)text,"%d", InstantSpeed);
    LCD_DisplayStringXY(90, 100, text);
}

/*!
 *  \brief Reads the variable 'Speedometer' and print it on the screen
 *  \return void
 */
void UpdateSpeedoMeter(){
char text[20];
	LCD_SetTextColor(Black);
	LCD_SetBackColor(Black);
	LCD_DrawFullRect(110, 200, 100, 30);
	LCD_DrawFullRect(20, 200, 15, 30);
	LCD_SetTextColor(White);
   	sprintf((char*)text,"%d", Speedometer);
    LCD_DisplayStringXY(110, 210, text);
    sprintf((char*)text,"%d", Partial_Speedometer);
    LCD_DisplayStringXY(20, 210, text);
}

/*!
 *  \brief Used to print a variable on the screen
 *  \return void
 */


/*!
 *  \brief Simulate the engine response reading the throttle and calculate the new RPM of the engine
 *  \return void
 */
void UpdateEngineResponse(){
int16_T NewRPM =  Throttle_Read();
	if( StopEngine == ZERO ){
		if(NewRPM>ZERO){//More Accel
			if( RPM_Value < RPM_MAX/2){ //low RPM
					RPM_Value = RPM_Value + NewRPM;
			}
			if(RPM_Value < RPM_MAX-2000){ 
					RPM_Value = RPM_Value + 0.8*NewRPM;
			}else if(RPM_Value < RPM_MAX){
					RPM_Value = RPM_Value + 0.4*NewRPM;
			}
		}else {//Less Decel
			if( RPM_Value < RPM_MAX/2 && RPM_Value+ NewRPM > RPM_MIN ){ //low speed
					RPM_Value = RPM_Value + NewRPM;
			}
			if(RPM_Value < RPM_MAX/2 && (RPM_Value + 1.2*NewRPM) > RPM_MIN ){ 
					RPM_Value = RPM_Value + 1.2*NewRPM;
			}else if(RPM_Value + 1.5*NewRPM > RPM_MIN){
					RPM_Value = RPM_Value + 1.5*NewRPM;
			}
		}
	}
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

static uint8_t RepeatLeftArrow = ZERO;
static uint8_t RepeatRightArrow = ZERO;
static uint8_t RepeatGearUp = ZERO;
static uint8_t RepeatGearDown = ZERO;
static uint8_t RepeatResetButton = ZERO;
static uint8_t RepeatBrightsButton = ZERO;

bool_t ReadLeftArrow = Button_LeftArrow_Read();
bool_t ReadRightArrow = Button_RightArrow_Read();
bool_t ReadGearUp = Button_GearUp_Read();
bool_t ReadGearDown = Button_GearDown_Read();
bool_t ReadResetKm = Button_ResetKm_Read();
bool_t ReadBrights = Button_Brights_Read();

debug(RepeatLeftArrow);//Davide: Remove this


/*debouncing for left arrow button*/
	if(ReadLeftArrow && RepeatLeftArrow <= 3){
		RepeatLeftArrow++;
	}
	if(ReadLeftArrow && RepeatLeftArrow == 3){
		Event_LeftArrow = ONE;
	}
	if( ReadLeftArrow==ZERO ){						//if and only if the user release the button the counter restarts from zero
		RepeatLeftArrow = ZERO;
		Event_LeftArrow = ZERO;
	}

/*debouncing for right arrow button*/
	if(ReadRightArrow && RepeatRightArrow <= 3){
		RepeatRightArrow++;
	}
	if(ReadRightArrow && RepeatRightArrow == 3){
		Event_RightArrow = ONE;
	}
	if( ReadRightArrow==ZERO ){					//if and only if the user release the button the counter restarts from zero
		RepeatRightArrow = ZERO;
		Event_RightArrow = ZERO;
	}

/*debouncing for GearUpbutton*/
	if(ReadGearUp && RepeatGearUp <= 3){
		RepeatGearUp++;
	}
	if(ReadGearUp && RepeatGearUp == 3){
		Event_GearUp = ONE;
	}
	if( ReadGearUp==ZERO ){						//if and only if the user release the button the counter restarts from zero
		RepeatGearUp = ZERO;
		Event_GearUp = ZERO;
	}

/*debouncing for GearDownbutton*/
	if(ReadGearDown && RepeatGearDown <= 3){
		RepeatGearDown++;
	}
	if(ReadGearDown && RepeatGearDown == 3){
		Event_GearDown = ONE;
	}
	if( ReadGearDown==ZERO ){						//if and only if the user release the button the counter restarts from zero
		RepeatGearDown = ZERO;
		Event_GearDown = ZERO;
	}

/*debouncing for Reset partial km button*/
	if(ReadResetKm && RepeatResetButton <= 3){
		RepeatResetButton++;
	}
	if(ReadResetKm && RepeatResetButton == 3){//if for 3 times the value was HIGH
		Event_PartialKm_Reset = ONE;
	}
	if( ReadResetKm==ZERO ){						//if and only if the user release the button the counter restarts from zero
		RepeatResetButton = ZERO;
		Event_PartialKm_Reset = ZERO;
		}	

/*debouncing for Brights button*/
	if( ReadBrights && RepeatBrightsButton <= 3){
		RepeatBrightsButton++;
	}
	if(ReadBrights && RepeatBrightsButton == 3){
		Event_Brights = ONE;
	}
	if( ReadBrights==ZERO ){						//if and only if the user release the button the counter restarts from zero
		RepeatBrightsButton = ZERO;
		Event_Brights = ZERO;
	}

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


