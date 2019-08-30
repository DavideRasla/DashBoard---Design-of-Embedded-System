
#include <stdlib.h>
#include "io.h"
#include "../config.h"

#include "../libs/tm_stm32f4_gpio.h"
#include "../libs/tm_stm32f4_adc.h"


#define BUTTON_PORT     (GPIOE)
#define BUTTON_PIN      (GPIO_PIN_5)
#define Speed_PIN		(GPIO_PIN_6)


void io_init(){
	TM_GPIO_Init(BUTTON_PORT,
				BUTTON_PIN,
			  TM_GPIO_Mode_IN,
			  TM_GPIO_OType_PP,
			  TM_GPIO_PuPd_DOWN,
			  TM_GPIO_Speed_High);



/* Initialize ADC1 on channel 3, this is pin PA3 */
    TM_ADC_Init(ADC1, ADC_Channel_3);
	
	/* Enable vbat channel */
	TM_ADC_EnableVbat();
	


	
}

bool_t button_read(){
	return BOOL(TM_GPIO_GetInputPinValue((BUTTON_PORT), (BUTTON_PIN)));
}


uint32_T Speed_Read(){

return U32T(TM_ADC_Read(ADC1, ADC_Channel_3));
}
