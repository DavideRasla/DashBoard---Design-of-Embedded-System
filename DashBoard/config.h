#ifndef CONFIG_H
#define CONFIG_H

//DASHBOARD CONFIGURATION 

//Fuel Conf
#define FULL_FUEL					   100
#define RED_FUEL						30
#define YELLOW_FUEL						70

//Engine Status
#define ENGINE_ON				  	  2500

//Motorbike Parameters
#define SPEED_MAX 					   250
#define CONSUMPTION_LITRE				 2 		//2km for litre
#define Oil_MustBe_Refilled			  	 1		//Should be 500
#define Km_Before_Crash					 1		//Should be 50

//RPM Parameters
#define RPM_INITIAL					  3000
#define RPM_MAX						 14000
#define RPM_LOW						  2000
#define RPM_MEDIUM					  8000
#define RPM_MIN						  500


//Dashboard Parameters
#define SPEEDOMETER_INITIAL			123000
#define	ZERO							 0
#define	ONE 							 1
#define	ONE_KM						  1000
#define	Watch_Pos						 6
#define	Neutral_Gear					 0
#define	First_Gear						 1
#define	Second_Gear						 2
#define	Third_Gear					 	 3
#define	Fourth_Gear						 4
#define	Fifth_Gear						 5
#define	Sixth_Gear						 6

//GPIO CONFIGURATIONS
//#define BUTTON_PORT     (GPIOE)
//#define BUTTON_PIN      (GPIO_PIN_5)
#define BTN_PORT 				(GPIOE)

#define BTN_GEAR_UP_PIN			(GPIO_PIN_0)
#define BTN_GEAR_DOWN_PIN		(GPIO_PIN_5)
#define BTN_TURNLIGHT_DX_PIN	(GPIO_PIN_4)
#define BTN_TURNLIGHT_SX_PIN	(GPIO_PIN_6)
#define BTN_HEADLIGHT_PIN		(GPIO_PIN_2)
#define BTN_RESET_PARTIALKM		(GPIO_PIN_1)
#define OUT_PORT				(GPIOB)

#define LED_ARROW_DX			(GPIO_PIN_1)
#define LED_ARROW_SX			(GPIO_PIN_3)
#define LED_LIGHT				(GPIO_PIN_5)

/* Initialize ADC1 on channel 4 for the throttle, this is pin PA4 */
#define ADC_THROTTLE_DEV		(ADC1)
#define ADC_THROTTLE_CHANNEL 	(ADC_Channel_4)

/* Initialize ADC2 on channel 5 for the Clutch, this is pin PA5 */
#define ADC_CLUTCH_DEV			(ADC2)
#define ADC_CLUTCH_CHANNEL 		(ADC_Channel_5)

//TASK CONFIGURATIONS
#define TASKOFFSET 					10
#define TaskCheckEvents_FREQ		50
#define TASKGUI_FREQ_FAST			200
#define TASKGUI_FREQ_SLOW			1000
#define TASKGUI_ReadSensors_FREQ	20

//EVENTS DASHBOARD
#define OIL 		0x02
#define FUEL 		0x04
#define AR			0x08
#define AL			0x010

//EVENTS BUTTONS
#define TURNR 			0x012
#define TURNL 			0x014
#define GEARUP 			0x016
#define GEARDOWN 		0x018
#define LIGHT			0x020
#define TIME_DEBOUNCING 40 //50ms*40 =2000ms = 2sec

#define Initial_Seconds 0
#define Initial_Minutes 42
#define Initial_Hours 13
#endif
