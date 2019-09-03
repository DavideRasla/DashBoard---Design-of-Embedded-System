

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
#include "libs/Touch.h"
#include "libs/Event.h"
#include "libs/debug.h"
#include "libs_custom/io.h"
#include "libs_custom/types.h"



/*Global Variables*/

uint8_T hours=13, minutes=44, seconds=ZERO;

uint8_T   Fuel_Value   		= FULL_FUEL;
uint8_T   IstantSpeed  		= 10;
uint32_T  Speedometer  		= SPEEDOMETER_INITIAL;
uint16_T  RPM_Value    		= RPM_INITIAL;
uint8_T   Actual_Gear 		= Neutral_Gear;
double    Actual_Accel 		= ZERO;
uint32_T  MetersTraveled 	= ZERO;
uint8_T	  KmTraveled 		= ZERO;
bool_t 	  StopEngine 		= ZERO; //0 = ON; 1 = OFF

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
 *  Called: NONE
 *  \return void
 */
void Calculate_MetersTraveled(){//Davide:: Da sistemare, non è precisa
static uint8_T i = ONE;
static uint8_T Speed1 = ZERO;
static uint8_T Speed2 = ZERO;
static uint8_T Average = ZERO;

	if( i!=2 ){ //First call: 
	Speed1 = IstantSpeed;
	}else{ 		//Second call:
	i = ZERO;
	Speed2 = IstantSpeed;
	Average = (Speed1 + Speed2)/2;
	MetersTraveled = MetersTraveled +  2*Average/3.6;//From km/h to m/s. Calculate for 2 seconds
	if(MetersTraveled > ONE_KM){//Davide: This should be 1000
		Speedometer++;
		KmTraveled++;
		MetersTraveled = MetersTraveled % ONE_KM; //Save just the meters

		if( KmTraveled % CONSUMPTION_LITRE==ZERO ){
			Fuel_Value= Fuel_Value - 20;
			//debug(Fuel_Value);
			if(Fuel_Value == ZERO){
				Fuel_Value= ONE;
				StopEngine = ONE;
				RPM_Value = RPM_INITIAL;
				}
			}
		}
	}
	i++;
}

/*!
 *  \brief Updates the clock on the top of the DashBoard. Also calls Calculate_MetersTraveld 
 *  Called: Calculate_MetersTraveled()
 *  \return void
 */
void UpdateTime()
{
static uint8_T i = 0;
unsigned char watchstr[20];

	if(i == 5){
	i = ZERO;
	seconds++;
	if(seconds == 60){
		seconds = ZERO;
		minutes++;
		if (minutes == 60){
			minutes = ZERO;
			hours++;
		}
		if (hours == 99){
			hours = ZERO;
		}
		}
	Calculate_MetersTraveled(); 	
	}
i++;
	sprintf(watchstr, "%2d:%2d:%2d", hours, minutes, seconds);
	LCD_SetTextColor(Black);
	LCD_SetBackColor(Black);
	LCD_DrawFullRect(95, 20, 135, 30);
	LCD_SetTextColor(White);
    LCD_DisplayStringXY(90, 20, watchstr);   
}
/*!
 *  \brief Reads the variable 'Fuel_Value' and draws the relative bar.
 *  Called: None
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
 *  Called: None
 *  \return void
 */
void checkEvents(){
//void checkButtons(){
	if(Button_LeftArrow_Read()==0){
			ClearEvent(iconinfo(&MyDashBoardScr[2])->onevent);
		}else{
			SetEvent(iconinfo(&MyDashBoardScr[2])->onevent);
		}

	
	if(Button_RightArrow_Read()==0){
			ClearEvent(iconinfo(&MyDashBoardScr[1])->onevent);
			}else{
			SetEvent(iconinfo(&MyDashBoardScr[1])->onevent);
			}
	
	//if(IsEvent(LIGHT)){

	//}
	if(Button_GearUp_Read() && Actual_Gear <= 5 && Clutch_Read()==1 ){
		Actual_Gear++;
		ChangeGear(&MyDashBoardScr[5], Actual_Gear);
		IstantSpeed = IstantSpeed / 2;
		RPM_Value = RPM_Value / 2;

	}
	if(Button_GearDown_Read() && Actual_Gear > 0 && Clutch_Read()==1 ){
		Actual_Gear--;
		ChangeGear(&MyDashBoardScr[5], Actual_Gear);
		IstantSpeed = IstantSpeed / 2;
		RPM_Value = RPM_Value / 2;
	}
	
}
/*!
 *  \brief Reads the variable 'RPM_VALUE' and draws the relative bar.
 *  Called: None
 *  \return void
 */
void UpdateMotorRPM(){
	//if(Clutch_Read()==0 && Actual_Gear != Neutral_Gear){ //DAVIDE: Credo sia concettualmente sbagliato che il motore non salga di giri se la frizione è tirata
		if( RPM_Value>=ZERO && RPM_Value<RPM_LOW ){
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
	/*}else{/*Draw a low bar in case the clutch is on 
			LCD_SetTextColor(Black);
			LCD_SetBackColor(Black);
			LCD_DrawFullRect(241, 50, 44,180);
			LCD_SetTextColor(Green);
			LCD_SetBackColor(Black);
			LCD_DrawFullRect(241, 190 - (RPM_LOW/100) , 44, (RPM_LOW/100) );
	}
	*/
}

/*!
 *  \brief Read from the sensor the amount of Throttle and calculates the actual acceleration of the motorbike.
 *  Called: None
 *  \return void
 */
void Update_Accell(){
	Actual_Accel = (double)(Throttle_Read())/RPM_Value;
}
/*!
 *  \brief Reads the variable 'Actual_Accel' and calculate the new speed
 *  Called: None
 *  \return void
 */
void UpdateSpeedValue(){
char text[20];
	LCD_SetTextColor(Black);
	LCD_SetBackColor(Black);
	LCD_DrawFullRect(90, 100, 130, 30);
	LCD_SetTextColor(White);

if( StopEngine == ZERO ){//if engine can work
	if(Clutch_Read()==0 && Actual_Gear != Neutral_Gear){//if the clutch is not on
					//debug(Actual_Accel);
						if(Actual_Accel>ZERO) {//Positive accelleration, need to increase the speed
							if(IstantSpeed == ZERO){
									IstantSpeed = 5;
								}
								if(IstantSpeed < SPEED_MAX/10){ //low speed
										IstantSpeed = 5 + IstantSpeed + ((int)(IstantSpeed  *1.4*Actual_Accel));
								}
								if(IstantSpeed >= SPEED_MAX/10 && IstantSpeed < SPEED_MAX/3){ //low speed
										IstantSpeed = IstantSpeed + ((int)(IstantSpeed *1.1*Actual_Accel));

								}
								if(IstantSpeed >= SPEED_MAX/3 && IstantSpeed < SPEED_MAX/2){ //medium speed
										IstantSpeed = IstantSpeed +  ((int)(IstantSpeed * 0.6*Actual_Accel));

								}
								if(IstantSpeed >= SPEED_MAX/2 && IstantSpeed < SPEED_MAX){ //high speed
									if( (uint8_T)(IstantSpeed + IstantSpeed * Actual_Accel) < SPEED_MAX)
										IstantSpeed = IstantSpeed +  ((int)(IstantSpeed * 0.25*Actual_Accel));
								}
								}else{//Negative accelleration, need to reduce the speed
									if(IstantSpeed < SPEED_MAX/10){ //very low speed
										if( (uint8_T)(IstantSpeed - 0.6*IstantSpeed)>0 ){
											IstantSpeed =  IstantSpeed - 0.6*IstantSpeed;
											if(IstantSpeed < 7){
											IstantSpeed = 0; //Stop
											}
										}
									}
								if(IstantSpeed >= SPEED_MAX/10 && IstantSpeed < SPEED_MAX/5){ //low speed
									if( (uint8_T)(IstantSpeed + IstantSpeed * Actual_Accel) < SPEED_MAX && (uint8_T)(IstantSpeed + IstantSpeed * Actual_Accel)>0 )
										IstantSpeed = IstantSpeed + ((int)(IstantSpeed *0.3*Actual_Accel));

								}
								if(IstantSpeed >= SPEED_MAX/5 && IstantSpeed < SPEED_MAX/2){ //medium speed
									if( (uint8_T)(IstantSpeed + IstantSpeed * Actual_Accel) < SPEED_MAX && (uint8_T)(IstantSpeed + IstantSpeed * Actual_Accel)>0 )
										IstantSpeed = IstantSpeed +  ((int)(IstantSpeed * 0.7*Actual_Accel));

								}
								if(IstantSpeed >= SPEED_MAX/3 && IstantSpeed <= SPEED_MAX){ //high speed
									if( (uint8_T)(IstantSpeed + IstantSpeed * Actual_Accel) < SPEED_MAX && (uint8_T)(IstantSpeed + IstantSpeed * Actual_Accel)>0 )
										IstantSpeed = IstantSpeed +  ((int)(IstantSpeed * 0.9*Actual_Accel));
								}
							}
						}else{//Clutch is ON, slow 10%
								IstantSpeed = IstantSpeed -  ((int)(IstantSpeed * 0.07)); //slow 10%
						}
	}else{
		IstantSpeed = 0;//No Fuel
	}
	sprintf((char*)text,"%d Km/h", IstantSpeed);
    LCD_DisplayStringXY(90, 100, text);
}

/*!
 *  \brief Reads the variable 'Speedometer' and print it on the screen
 *  Called: None
 *  \return void
 */
void UpdateSpeedoMeter(){
char text[20];
	LCD_SetTextColor(Black);
	LCD_SetBackColor(Black);
	LCD_DrawFullRect(100, 200, 100, 30);
	LCD_SetTextColor(White);
   	sprintf((char*)text,"%d Km", Speedometer);
    LCD_DisplayStringXY(100, 210, text);
}

/*!
 *  \brief Used to print a variable on the screen
 *  Called: None
 *  \return void
 */
void debug(int a){
char text[20];
	LCD_SetTextColor(Black);
	LCD_SetBackColor(Black);
	LCD_DrawFullRect(50, 130, 100, 40);
	LCD_SetTextColor(White);
   	sprintf((char*)text,"%d", a);
    LCD_DisplayStringXY(50, 130, text);
}

/*!
 *  \brief Simulate the engine response reading the throttle and calculate the new RPM of the engine
 *  Called: None
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
			if(RPM_Value < RPM_MAX/2 &&  RPM_Value + 1.2*NewRPM > RPM_MIN ){ 
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
TASK(TaskGuiDashboard)
{
	UpdateTime();
	UpdateFuel();
	UpdateMotorRPM();
	UpdateSpeedValue();
	UpdateSpeedoMeter();
	DrawIcons(MyDashBoardScr);
	
} 


/*!
 *  \brief Task that calculates and updates all the Variables used by TaskGuiDashboard. 
 *   Also reads all the events from the buttons
 *  \return void
 */
TASK(TaskUpdate)
{

UpdateEngineResponse();
Update_Accell();
//get_buttons_events();
checkEvents();


//to remove this below. Touch is not used anymore
	unsigned int px, py;
	TPoint p;
	if (GetTouch_SC_Async(&px, &py)) {
		p.x = px;
		p.y = py;
		OnTouch(MyDashBoardScr, &p);
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
	/* Initializes LCD and touchscreen */
	IOE_Config();
	/* Initialize the LCD */
	STM32f4_Discovery_LCD_Init();

	InitTouch(-0.102, 0.0656, -310, 16);//DAVIDE::to remove this

	/* Draw the background*/
	DrawInit(MyDashBoardScr);
	

	LCD_SetTextColor(White);

	//Initialize gpio
	io_init();

	/* Program cyclic alarms which will fire after an initial offset,
	 * and after that periodically
	 * */
	SetRelAlarm(AlarmTaskUpdate, TASKOFFSET, TaskUpdate_FREQ);
	SetRelAlarm(AlarmTaskGuiDashboard, TASKOFFSET, TASKGUI_FREQ);

  /* Forever loop: background activities (if any) should go here */
	for (;;) {
	}

}


