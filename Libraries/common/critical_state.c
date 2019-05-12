#include "stdbool.h"
#include "critical_state.h"

#ifdef NRF52832_XXAA
#include "nrf52.h"
#elif STM32F10X_CL
#include "stm32f10x.h"
#elif STM32F0XX
#include "stm32f0xx.h"
#else
#error Processor is not defined inside err_check module. 
#endif

bool state = true;
void critical_state_enter(void)
{
	if(state == true)
	{
		__disable_irq();
		state = false;
	}	
}

void critical_state_exit(void)
{
	if (state == false)
	{
			__enable_irq();
			state = true;
	}
}
