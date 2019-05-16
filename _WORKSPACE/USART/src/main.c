/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

#include "my_gpio_drv.h"
#include "my_usart_drv.h"

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
  my_drv_gpio_pin_output(GPIOC, PIN_8);
  my_drv_gpio_pin_output(GPIOC, PIN_9);;

  /* Turn on LED3 and LED4 */
  my_drv_gpio_write_pin(GPIOC, PIN_8, PIN_SET);
  my_drv_gpio_write_pin(GPIOC, PIN_9, PIN_SET);
  for(volatile int i=0; i<5000000; i++){};

  /* Turn off LED3 and LED4 */
  my_drv_gpio_write_pin(GPIOC, PIN_8, PIN_RESET);
  my_drv_gpio_write_pin(GPIOC, PIN_9, PIN_RESET);
  for(volatile int i=0; i<5000000; i++){};
		
	USART_conf_t USART_conf;
	USART_conf.GPIOx = GPIOA;
	USART_conf.RX_PIN = PIN_10;
	USART_conf.TX_PIN = PIN_9;
		
	GPIO_pin_init_t USART_pin_conf;
	USART_pin_conf.gpio_mode = ALTERNATE_FUNCTION_MODE;
	USART_pin_conf.output_type = PUSH_PULL;
	USART_pin_conf.speed = HIGH_SPEED;
	USART_pin_conf.pupd = NO_PULL_UP_DOWN;
	USART_pin_conf.af = ALTERNATE_FUNCTION_1;	
	USART_conf.USART_pin_init = &USART_pin_conf; // Asign pointer to pointee

	USART_conf.USART_wordlength = USART_Wordlength_8bit;
	USART_conf.USART_stopbit = USART_stopbit_1;
	USART_conf.USART_parity = USART_parity_disabled;
	USART_conf.USART_parity_mode = USART_parity_even;
	USART_conf.USART_oversampling = USART_oversampling_8;
	USART_conf.USART_baudrate = USART_baudrate_115200;
	my_drv_usart_init(&USART_conf);
		
	my_drv_usart_send_char('d');
	my_drv_usart_send_char('e');
	my_drv_usart_send_char('n');
	my_drv_usart_send_char('e');
	my_drv_usart_send_char('m');
	my_drv_usart_send_char('e');
	my_drv_usart_send_char('\r');
	my_drv_usart_send_char('\n');

	//DEBUG
  // *(uint32_t *)~0 = 4;  	// Hard Fault Example
  // ERROR_CHECK(1);     		// App Error Fault Handler Example
  // ASSERT(0);					// App Error Fault Handler Example
	
  while (1)
  {
			/* Turn on LED3 and LED4 */
			my_drv_gpio_write_pin(GPIOC, PIN_8, PIN_SET);
			my_drv_gpio_write_pin(GPIOC, PIN_9, PIN_SET);
		
			if(rx_ready == true)
			{
				rx_ready = false;
				my_drv_usart_send_char('?');
				my_drv_usart_send_char('\r');
				my_drv_usart_send_char('\n');				
			}				
			
//			for(volatile int i=0; i<5000000; i++){};
//			logi("LOG");
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
		loge("HARDFAULT.");
		while(1);
		//NVIC_SystemReset();
}
