
#include <stdlib.h>
#include "io.h"
#include "../config.h"

#include "../libs/tm_stm32f4_gpio.h"

//aggiungere libreria adc!!

void io_init(){
	TM_GPIO_Init(BUTTON_PORT,
				BUTTON_PIN,
			  TM_GPIO_Mode_IN,
			  TM_GPIO_OType_PP,
			  TM_GPIO_PuPd_DOWN,
			  TM_GPIO_Speed_High);
}

bool_t button_read(){
	return BOOL(TM_GPIO_GetInputPinValue((BUTTON_PORT), (BUTTON_PIN)));
}
