#include "stdbool.h"
#include "error_check.h"
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

void assert_callback(uint16_t line_num, const uint8_t * file_name, uint32_t err_code)
{
    error_info_t error_info =
    {
        .line_num    = line_num,
        .p_file_name = file_name,
				.err_code    = err_code,
    };
    error_fault_handler((uint32_t)(&error_info));
}

__WEAK void error_fault_handler(uint32_t info)
{
    critical_state_enter();
		
#ifndef DEBUG_ENABLED
		// LOG --> "System reset"
    NVIC_SystemReset();
#else
    // The following variable helps Keil keep the call stack visible, in addition, it can be set to
    // 0 in the debugger to continue executing code after the error check.
    volatile bool loop = true;
    ((void)(loop));

    // If printing is disrupted, remove the irq calls, or set the loop variable to 0 in the debugger.
    critical_state_enter();
    while (loop);

    critical_state_exit();
#endif // DEBUG
}
/*lint -restore */
