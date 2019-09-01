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

//RPM Parameters
#define RPM_INITIAL					  3000
#define RPM_MAX						 12000
#define RPM_LOW						  5000
#define RPM_MEDIUM					  8000
#define RPM_MIN						  2000


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
#define BUTTON_PORT     (GPIOE)
#define BUTTON_PIN      (GPIO_PIN_5)

//GUI CONFIGURATION


//TASK CONFIGURATIONS
#define TASKOFFSET 		10
#define TaskUpdate_FREQ	50
#define TASKGUI_FREQ	200



#endif
