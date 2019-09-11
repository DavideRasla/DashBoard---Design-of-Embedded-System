
#ifndef IO_H
#define IO_H

#include "types.h"
#include "Event.h"

void io_init();

bool_t Button_LeftArrow_Read();

void get_buttons_events();

bool_t Clutch_Read(uint32_T valueClutch);
int Throttle_Read(uint32_T valueThrottle);
int UpdateFuel(int Fuel_Value);
#endif
