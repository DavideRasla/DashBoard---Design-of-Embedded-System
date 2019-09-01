
#include <stdlib.h>
#include "io.h"
#include "../config.h"

#include "../libs/tm_stm32f4_gpio.h"
#include "../libs/tm_stm32f4_adc.h"


#define BUTTON_PORT    			 			(GPIOE)
#define BUTTON_PIN_LeftArrow      			(GPIO_PIN_7)
#define BUTTON_PIN_RightArrow				(GPIO_PIN_8)
#define BUTTON_PIN_GearUp					(GPIO_PIN_5)
#define BUTTON_PIN_GearDown					(GPIO_PIN_6)

void io_init(){
	TM_GPIO_Init(BUTTON_PORT,
				BUTTON_PIN,
			  TM_GPIO_Mode_IN,
			  TM_GPIO_OType_PP,
			  TM_GPIO_PuPd_DOWN,
			  TM_GPIO_Speed_High);



/* Initialize ADC1 on channel 4 for the throttle, this is pin PA4 */
    TM_ADC_Init(ADC1, ADC_Channel_4);
/* Initialize ADC2 on channel 3 for the Clutch, this is pin PA3 */
    TM_ADC_Init(ADC2, ADC_Channel_3);	
	/* Enable vbat channel */
	TM_ADC_EnableVbat();
	


	
}

bool_t Button_LeftArrow_Read(){
	return BOOL(TM_GPIO_GetInputPinValue((BUTTON_PORT), (BUTTON_PIN_LeftArrow)));
}
bool_t Button_RightArrow_Read(){
	return BOOL(TM_GPIO_GetInputPinValue((BUTTON_PORT), (BUTTON_PIN_RightArrow)));
}
bool_t Button_GearUp_Read(){
	return BOOL(TM_GPIO_GetInputPinValue((BUTTON_PORT), (BUTTON_PIN_GearUp)));
}
bool_t Button_GearDown_Read(){
	return BOOL(TM_GPIO_GetInputPinValue((BUTTON_PORT), (BUTTON_PIN_GearDown)));
}
/*
	Retuns between [100 - 4000]
*/
int Throttle_Read(){
	uint32_T ActualThrottleValue= TM_ADC_Read(ADC1, ADC_Channel_4);
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
uint32_T ActualClutchValue= TM_ADC_Read(ADC1, ADC_Channel_3);
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