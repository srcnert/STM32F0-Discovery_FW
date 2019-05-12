/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

#include "error_check.h"
#include "rtt_log.h"

GPIO_InitTypeDef GPIO_InitStructure;

error_info_t *p_error_info;

int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */

  loge("LOG_INIT.");
	
  /* Initialize Leds mounted on STM32F0-discovery */
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  /* Turn on LED3 and LED4 */
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED4);
  for(volatile int i=0; i<5000000; i++){};

	/* Turn off LED3 and LED4 */
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);
  for(volatile int i=0; i<5000000; i++){};
	
  //DEBUG
  // *(uint32_t *)~0 = 4;  	// Hard Fault Example
  // ERROR_CHECK(1);     		// App Error Fault Handler Example
  // ASSERT(0);					// App Error Fault Handler Example
	
  while (1)
  {
		/* Turn on LED3 and LED4 */
		STM_EVAL_LEDOn(LED3);
		STM_EVAL_LEDOn(LED4);
  }
}

void error_fault_handler(uint32_t info)
{
		p_error_info = (error_info_t *) info;
		loge("ERROR FAULT.");
		loge("File Name: %s, Line: %d, Err Code: %d", p_error_info->p_file_name, p_error_info->line_num, p_error_info->err_code);
		while(1);	
	
//		NVIC_SystemReset();
}

void HardFault_Handler(void)  
{
		while(1);
		//NVIC_SystemReset();
}
