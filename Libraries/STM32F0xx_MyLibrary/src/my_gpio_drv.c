#include "stm32f0xx.h"
#include "my_gpio_drv.h"

void my_drv_gpio_write_pin(GPIO_TypeDef* GPIOx, GPIO_pin_number_t PIN_x, GPIO_pin_state_t pin_state)
{
	if(pin_state == PIN_SET)
		GPIOx->ODR |= (1 << PIN_x);
	else if(pin_state == PIN_RESET)
		GPIOx->ODR &= ~(1 << PIN_x);
}

void my_drv_gpio_toggle_pin(GPIO_TypeDef* GPIOx, GPIO_pin_number_t PIN_x)
{	
	GPIOx->ODR ^= (1 << PIN_x);
}

void my_drv_gpio_clock_init(GPIO_TypeDef* GPIOx)
{
	if(GPIOx == GPIOA)
		RCC->AHBENR |= (1U << 17U);
	else if(GPIOx == GPIOB)
		RCC->AHBENR |= (1U << 18U);
	else if(GPIOx == GPIOC)
		RCC->AHBENR |= (1U << 19U);
	else if(GPIOx == GPIOD)
		RCC->AHBENR |= (1U << 20U);
	else if(GPIOx == GPIOE)
		RCC->AHBENR |= (1U << 21U);
	else if(GPIOx == GPIOF)
		RCC->AHBENR |= (1U << 22U);
}

void my_drv_gpio_init(GPIO_TypeDef* GPIOx, GPIO_pin_init_t *GPIO_pin_init, GPIO_pin_number_t PIN_x)
{	
	my_drv_gpio_clock_init(GPIOx);

	GPIOx->MODER 	|= (GPIO_pin_init->gpio_mode) << (PIN_x*2U);
	GPIOx->OTYPER 	|= (GPIO_pin_init->output_type) << PIN_x;
	GPIOx->OSPEEDR 	|= (GPIO_pin_init->speed) << (PIN_x*2U);
	GPIOx->PUPDR 	|= (GPIO_pin_init->pupd) << (PIN_x*2U);

	if(PIN_x <= PIN_7)
		GPIOx->AFR[0] |= ((GPIO_pin_init->af) << (PIN_x*4U));
	else
		GPIOx->AFR[1] |= ((GPIO_pin_init->af) << ((PIN_x-8)*4U));		
}

void my_drv_gpio_pin_af(GPIO_TypeDef* GPIOx, GPIO_alternate_function_t AF_x, GPIO_output_type_t GPIO_output_type, GPIO_pupd_t GPIO_pupd, GPIO_pin_number_t PIN_x)
{	
	GPIO_pin_init_t GPIO_pin_init;
	
	GPIO_pin_init.gpio_mode = ALTERNATE_FUNCTION_MODE;
	GPIO_pin_init.output_type =GPIO_output_type;
	GPIO_pin_init.speed = HIGH_SPEED;
	GPIO_pin_init.pupd = GPIO_pupd;
	GPIO_pin_init.af = AF_x;
	
	my_drv_gpio_init(GPIOx, &GPIO_pin_init, PIN_x);
}

void my_drv_gpio_pin_output(GPIO_TypeDef* GPIOx, GPIO_pin_number_t PIN_x)
{	
	GPIO_pin_init_t GPIO_pin_init;
	
	GPIO_pin_init.gpio_mode = OUTPUT_MODE;
	GPIO_pin_init.output_type = PUSH_PULL;
	GPIO_pin_init.speed = HIGH_SPEED;
	GPIO_pin_init.pupd = NO_PULL_UP_DOWN;
	
	my_drv_gpio_init(GPIOx, &GPIO_pin_init, PIN_x);
}

void my_drv_gpio_pin_input(GPIO_TypeDef* GPIOx, GPIO_pupd_t GPIO_pupd, GPIO_pin_number_t PIN_x)
{	
	GPIO_pin_init_t GPIO_pin_init;
	
	GPIO_pin_init.gpio_mode = INPUT_MODE;
	GPIO_pin_init.speed = HIGH_SPEED;
	GPIO_pin_init.pupd = GPIO_pupd;
	
	my_drv_gpio_init(GPIOx, &GPIO_pin_init, PIN_x);
}

bool my_drv_gpio_read_pin(GPIO_TypeDef* GPIOx, GPIO_pin_number_t PIN_x)
{
	return ((GPIOx->IDR >> PIN_x) & 1UL);
}

void my_drv_gpio_interrupt_pin_enabled(GPIO_port_t PORTx, GPIO_pin_number_t PIN_x, GPIO_int_polarity_t POLARITY_x)
{
	RCC->APB2ENR |= (1 << 0); // Enable SYSCFG clock
	
	/*
	 * Select port and pin for interrupt
	 * PIN -> 00 01 02 03  -- EXTICR[0] -- 
	 * PIN -> 04 05 06 07  -- EXTICR[1]
	 * PIN -> 08 09 10 11  -- EXTICR[2]
	 * PIN -> 12 13 14 15  -- EXTICR[3]
	 * EXTI_x --> PIN_x 
	 */
	if(PIN_x <= PIN_3)
	{
		SYSCFG->EXTICR[0] |= (PORTx << ((PIN_x % 4) * 4U));
	}
	else if(PIN_x <= PIN_7)
	{
		SYSCFG->EXTICR[1] |= (PORTx << ((PIN_x % 4) * 4U));
	}
	else if(PIN_x <= PIN_11)
	{
		SYSCFG->EXTICR[2] |= (PORTx << ((PIN_x % 4) * 4U));
	}	
	else if(PIN_x <= PIN_15)
	{
		SYSCFG->EXTICR[3] |= (PORTx << ((PIN_x % 4) * 4U));
	}
	
	/*
	 * Interrupt line status
	 * EXTI_LINE_5 --> EXTI_0 and EXTI_1 (PIN_0 and PIN_1) 			--> SYSCFG->IT_LINE_SR[5] 
	 * EXTI_LINE_6 --> EXTI_2 and EXTI_3 (PIN_2 and PIN_3)			--> SYSCFG->IT_LINE_SR[5] 
	 * EXTI_LINE_7 --> EXTI_4 to EXTI15  (from PIN_4 to PIN_15) --> SYSCFG->IT_LINE_SR[5] 
	 */
		if(POLARITY_x == POLARITY_LoToHi)
			EXTI->RTSR |= (1 << PIN_x); // Rising interrupt trigger 
		else if(POLARITY_x == POLARITY_LoToHi)
			EXTI->FTSR |= (1 << PIN_x); // Falling interrupt trigger 
		else if(POLARITY_x == POLARITY_Toggle)
		{	
			EXTI->RTSR |= (1 << PIN_x); // Rising interrupt trigger 
			EXTI->FTSR |= (1 << PIN_x); // Falling interrupt trigger 
		}	
			
		EXTI->IMR |= (1 << PIN_x); // Interrupt request from Line x is not mask
	
		if(PIN_x <= PIN_1)
		{
			// The priority for that interrupt source which can be set from 0(highest) to 3(lowest). 	
			uint8_t EXTI_0_1_PRIORITY = 0;
			NVIC_SetPriority(EXTI0_1_IRQn, EXTI_0_1_PRIORITY);
			NVIC_ClearPendingIRQ(EXTI0_1_IRQn);
			NVIC_EnableIRQ(EXTI0_1_IRQn);
		}
		else if(PIN_x <= PIN_3)
		{
			// The priority for that interrupt source which can be set from 0(highest) to 3(lowest). 	
			uint8_t EXTI_2_3_PRIORITY = 0;
			NVIC_SetPriority(EXTI2_3_IRQn, EXTI_2_3_PRIORITY);
			NVIC_ClearPendingIRQ(EXTI2_3_IRQn);
			NVIC_EnableIRQ(EXTI2_3_IRQn);
		}
		else if(PIN_x <= PIN_15)
		{
			// The priority for that interrupt source which can be set from 0(highest) to 3(lowest). 	
			uint8_t EXTI_4_15_PRIORITY = 0;
			NVIC_SetPriority(EXTI4_15_IRQn, EXTI_4_15_PRIORITY);
			NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
			NVIC_EnableIRQ(EXTI4_15_IRQn);
		}
}

void my_drv_gpio_interrupt_pin_disabled(GPIO_pin_number_t PIN_x)
{	
		if(PIN_x <= PIN_1)
		{
			// The priority for that interrupt source which can be set from 0(highest) to 3(lowest). 	
			NVIC_DisableIRQ(EXTI0_1_IRQn);
		}
		else if(PIN_x <= PIN_3)
		{
			// The priority for that interrupt source which can be set from 0(highest) to 3(lowest). 	

			NVIC_DisableIRQ(EXTI2_3_IRQn);
		}
		else if(PIN_x <= PIN_15)
		{
			// The priority for that interrupt source which can be set from 0(highest) to 3(lowest). 	
			NVIC_DisableIRQ(EXTI4_15_IRQn);
		}
		
		EXTI->IMR &= ~(1 << PIN_x); // Interrupt request from Line x is not mask
}
