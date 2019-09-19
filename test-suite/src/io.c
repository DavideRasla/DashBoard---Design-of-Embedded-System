
#include <stdlib.h>
#include <math.h>
#include "io.h"
#include "config.h"
#include "types.h"
#include "Event.h"

bool_t StopEngine = 0;


/*!
 *  \brief Reads the value of the throttle [0-4000]. If <2000 returns a negative acceleration, otherwise a positive accelleration.
 *  \return int
 */
int Throttle_Read(uint32_T valueThrottle){
	uint32_T ActualThrottleValue=valueThrottle ;
if(ActualThrottleValue > 1){
	if( ActualThrottleValue<2000 ){
		return -150;
	}else{
		return 130;
	}
}
return 0;
}
/*!
 *  \brief Reads the value of the throttle [0-4000]. If <2000 returns 0, otherwise the clutch is ON
 *  \return bool_t
 */
bool_t Clutch_Read(uint32_T valueClutch){
uint32_T ActualClutchValue= valueClutch;
	if(ActualClutchValue > 1){
		if( ActualClutchValue<2000 ){
			return 0;//unactive
		}else{
			return 1;//active
		}
	}
	return 0;
}


int UpdateFuel(int Fuel_Value){
	int ret;
	if( Fuel_Value>=ZERO ){
			if( Fuel_Value < RED_FUEL){
				ret = 1;
			}else if(Fuel_Value >=RED_FUEL && Fuel_Value<= YELLOW_FUEL){			
				ret = 2;
			}else{			
				ret = 3;
				}
		}else{
				ret = -1;
		}
	return ret;
}