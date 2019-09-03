
#include <stdlib.h>
#include "io.h"
#include "../config.h"
#include "../libs_custom/types.h"
#include "../libs/Event.h"


#include "../libs/tm_stm32f4_gpio.h"
#include "../libs/tm_stm32f4_adc.h"

#define is_pin_high(GPIOx, GPIO_Pin)	(((GPIOx) & (GPIO_Pin)) == 0 ? 0 : 1)


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
/*

double Throttle_Read(){
	uint32_T ActualThrottleValue= TM_ADC_Read(ADC1, ADC_Channel_3);
	if( ActualThrottleValue<500 ){
		return 0.03;
	}
	if( ActualThrottleValue<1000 ){
		return 0.04;
	}
	if( ActualThrottleValue<1300 ){
		return 0.05;
	}
	if( ActualThrottleValue<1600 ){
		return 0.06;
	}
	if( ActualThrottleValue<1800 ){
		return 0.07;
	}
	if( ActualThrottleValue<2000 ){
		return 0.1;
	}
	if( ActualThrottleValue<2500 ){
		return 0.11;
	}
	if( ActualThrottleValue<2700 ){
		return 0.12;
	}
	if( ActualThrottleValue<2900 ){
		return 0.13;
	}
	if( ActualThrottleValue<3100 ){
		return 0.14;
	}
	if( ActualThrottleValue<3200 ){
		return 0.15;
	}
	if( ActualThrottleValue<3500 ){
		return 0.16;
	}
	if( ActualThrottleValue<3800 ){
		return 0.17;
	}
	if( ActualThrottleValue<4000 ){
		return 0.18;
	}
return 0;
}*/

void get_buttons_events(){
	uint16_T val = TM_GPIO_GetPortInputValue(BTN_PORT);
	if(is_pin_high(val,BTN_GEAR_UP_PIN)){
		SetEvent(GEARUP);
		debug(2);
	}
	if(is_pin_high(val,BTN_GEAR_DOWN_PIN)){
		SetEvent(GEARDOWN);
		debug(3);
	}
	if(is_pin_high(val,BTN_TURNLIGHT_DX_PIN)){
		SetEvent(TURNR);
		debug(4);
	}
	if(is_pin_high(val,BTN_TURNLIGHT_SX_PIN)){
		SetEvent(TURNL);
		debug(5);
	}
	if(is_pin_high(val,	BTN_HEADLIGHT_PIN)){
		SetEvent(LIGHT);
		debug(6);
	}
}

