
#include <stdlib.h>
#include "io.h"
#include "../config.h"
#include "../libs_custom/types.h"
#include "../libs/Event.h"


#include "../libs/tm_stm32f4_gpio.h"
#include "../libs/tm_stm32f4_adc.h"




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

	TM_GPIO_Init(BTN_PORT,
			BTN_HEADLIGHT_PIN,
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
	
	/*
	//OUTPUTS
	TM_GPIO_Init(OUT_PORT,
				LED_ARROW_DX,
				TM_GPIO_Mode_OUT,
				TM_GPIO_OType_PP,
				TM_GPIO_PuPd_DOWN,
				TM_GPIO_Speed_High);

	TM_GPIO_Init(OUT_PORT,
					LED_ARROW_SX,
					TM_GPIO_Mode_OUT,
					TM_GPIO_OType_PP,
					TM_GPIO_PuPd_DOWN,
					TM_GPIO_Speed_High);

	TM_GPIO_Init(OUT_PORT,
					LED_LIGHT,
					TM_GPIO_Mode_OUT,
					TM_GPIO_OType_PP,
					TM_GPIO_PuPd_DOWN,
					TM_GPIO_Speed_High);


*/

	
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
	return BOOL(TM_GPIO_GetInputPinValue((BTN_PORT), (BTN_RESET_PARTIALKM)));
}

/*
	Retuns between [100 - 4000]
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



