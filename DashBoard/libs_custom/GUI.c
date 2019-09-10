#include "GUI.h"
#include "config.h"
#include "types.h"
#include "../libs/stm32f4_discovery_lcd.h"
#include "../libs/Widget.h"
#include "../libs/WidgetConfig.h"
#include "io.h"

#include "../libs/STMPE811QTR.h"
#include "../assets/pictures.h"
#include "../libs/Event.h"
#include "../libs/debug.h"

/*Global Variables*/


extern uint8_T   Fuel_Value;										// Fuel value, inizializated to full
extern uint32_T  Speedometer;								        // Total KM counter
extern uint32_T  Partial_Speedometer;					    		// Total partial KM counter. This can be resetted
extern uint16_T  RPM_Value;									    	// RPM of the engine
extern uint8_T   Actual_Gear;										// Actual gear of the motorbike. Is shown on the screen
extern bool_t 	  StopEngine; 										// If equals to zero the engine can work, otherwise the engine has a problem or the motorbike is without fuel

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

