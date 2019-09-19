
#include <stdlib.h>
#include "io.h"
#include "../config.h"
#include "../libs_custom/types.h"
#include "../libs/Event.h"
#include "Sensors.h"
#include "GUI.h"
#include "../libs/tm_stm32f4_gpio.h"
#include "../libs/tm_stm32f4_adc.h"
#include "../libs/stm32f4xx_gpio.h"
#include "../libs/stm32f4xx_adc.h"




void io_init(){
	//BUTTON INPUTS
	TM_GPIO_Init(BTN_PORT,
				BTN_GEAR_UP_PIN,
				TM_GPIO_Mode_IN,
				TM_GPIO_OType_PP,
				TM_GPIO_PuPd_DOWN,
				TM_GPIO_Speed_High);

	TM_GPIO_Init(BTN_PORT,
					BTN_GEAR_DOWN_PIN,
					TM_GPIO_Mode_IN,
					TM_GPIO_OType_PP,
					TM_GPIO_PuPd_DOWN,
					TM_GPIO_Speed_High);

	TM_GPIO_Init(BTN_PORT,
					BTN_TURNLIGHT_DX_PIN,
						TM_GPIO_Mode_IN,
						TM_GPIO_OType_PP,
						TM_GPIO_PuPd_DOWN,
						TM_GPIO_Speed_High);

	TM_GPIO_Init(BTN_PORT,
				BTN_TURNLIGHT_SX_PIN,
						TM_GPIO_Mode_IN,
						TM_GPIO_OType_PP,
						TM_GPIO_PuPd_DOWN,
						TM_GPIO_Speed_High);


	TM_GPIO_Init(BTN_PORT_C,
			BTN_RESET_PARTIALKM,
						TM_GPIO_Mode_IN,
						TM_GPIO_OType_PP,
						TM_GPIO_PuPd_DOWN,
						TM_GPIO_Speed_High);
	TM_GPIO_Init(BTN_PORT,
			BTN_Brights_DOWN_PIN,
						TM_GPIO_Mode_IN,
						TM_GPIO_OType_PP,
						TM_GPIO_PuPd_DOWN,
						TM_GPIO_Speed_High);
						

	//ADC INPUTS
/* Initialize ADC1 on channel 4 for the throttle, this is pin PA4 */
    TM_ADC_Init(ADC_THROTTLE_DEV, ADC_THROTTLE_CHANNEL);
/* Initialize ADC2 on channel 3 for the Clutch, this is pin PA3 */
    TM_ADC_Init(ADC_CLUTCH_DEV, ADC_CLUTCH_CHANNEL);
	/* Enable vbat channel */
	TM_ADC_EnableVbat();


	
}

bool_t Button_LeftArrow_Read(){
	return BOOL(TM_GPIO_GetInputPinValue((BTN_PORT), (BTN_TURNLIGHT_SX_PIN)));
}
bool_t Button_RightArrow_Read(){
	return BOOL(TM_GPIO_GetInputPinValue((BTN_PORT), (BTN_TURNLIGHT_DX_PIN)));
}
bool_t Button_GearUp_Read(){
	return BOOL(TM_GPIO_GetInputPinValue((BTN_PORT), (BTN_GEAR_UP_PIN)));
}
bool_t Button_GearDown_Read(){
	return BOOL(TM_GPIO_GetInputPinValue((BTN_PORT), (BTN_GEAR_DOWN_PIN)));
}
bool_t Button_ResetKm_Read(){
	return BOOL(TM_GPIO_GetInputPinValue((BTN_PORT_C), (BTN_RESET_PARTIALKM)));
}
bool_t Button_Brights_Read(){
	return BOOL(TM_GPIO_GetInputPinValue((BTN_PORT), (BTN_Brights_DOWN_PIN)));
}

/*!
 *  \brief Reads the value of the throttle [0-4000]. If <2000 returns a negative acceleration, otherwise a positive accelleration.
 *  \return int
 */
int Throttle_Read(){
	uint32_T ActualThrottleValue= TM_ADC_Read(ADC_THROTTLE_DEV, ADC_THROTTLE_CHANNEL);
if(ActualThrottleValue > 1){
	if( ActualThrottleValue<2000 ){
		return -150*log(ActualThrottleValue);
	}else{
		return 130*log(ActualThrottleValue);
	}
}
return 0;
}
/*!
 *  \brief Reads the value of the throttle [0-4000]. If <2000 returns 0, otherwise the clutch is ON
 *  \return bool_t
 */
bool_t Clutch_Read(){
uint32_T ActualClutchValue= TM_ADC_Read(ADC_CLUTCH_DEV, ADC_CLUTCH_CHANNEL);
	if(ActualClutchValue > 1){
		if( ActualClutchValue<2000 ){
			return 0;//unactive
		}else{
			return 1;//active
		}
	}
	return 0;
}



