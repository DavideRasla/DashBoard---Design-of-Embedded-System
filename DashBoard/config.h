#ifndef CONFIG_H
#define CONFIG_H

//DASHBOARD CONFIGURATION 

//Fuel Conf
#define FULL_FUEL					   100
#define RED_FUEL						30
#define YELLOW_FUEL						70



//Motorbike Parameters
#define SPEED_MAX 					   250
#define CONSUMPTION_LITRE				 2 		//2km for litre
#define Oil_MustBe_Refilled			  	 1		//Should be like 500
#define Km_Before_Crash					 2		//Should be 50

//RPM Parameters
#define RPM_INITIAL					  3000      //Initial RPM when the engine is ON
#define RPM_MAX						 14000      //Max value of RPM
#define RPM_LOW						  2000      //low RPM (Green)
#define RPM_MEDIUM					  8000      //Medium RPM (yellow)
#define RPM_MIN						  500       //Min RPM value


//Dashboard Parameters
#define SPEEDOMETER_INITIAL			123000      //Initial total amount of km. User can't reset this
#define	ZERO							 0
#define	ONE 							 1
#define	ONE_KM						  1000
#define	Neutral_Gear					 0
#define	First_Gear						 1
#define	Second_Gear						 2
#define	Third_Gear					 	 3
#define	Fourth_Gear						 4
#define	Fifth_Gear						 5
#define	Sixth_Gear						 6

//GPIO CONFIGURATIONS

#define BTN_PORT 				(GPIOE)
#define BTN_PORT_C 				(GPIOC)

#define BTN_GEAR_UP_PIN			(GPIO_PIN_0)
#define BTN_GEAR_DOWN_PIN		(GPIO_PIN_5)
#define BTN_TURNLIGHT_DX_PIN	(GPIO_PIN_4)
#define BTN_TURNLIGHT_SX_PIN	(GPIO_PIN_6)
#define BTN_RESET_PARTIALKM		(GPIO_PIN_3) //C
#define BTN_Brights_DOWN_PIN    (GPIO_PIN_1)



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
#define TASKGUI_ReadSensors_FREQ	40

//EVENTS DASHBOARD
#define OIL 		0x01
#define FUEL 		0x02
#define AR			0x04
#define AL			0x08
#define BRIGHTS     0x10


//EVENTS BUTTONS
#define TURNR 			0x20
#define TURNL 			0x40
#define TIME_DEBOUNCING 40 //50ms*40 =2000ms = 2sec

#define Initial_Seconds 0
#define Initial_Minutes 42
#define Initial_Hours 13
#endif
