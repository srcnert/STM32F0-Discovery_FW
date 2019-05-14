#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "my_gpio_drv.h"

bool my_drv_gpio_read_pin(GPIO_TypeDef* GPIOx, GPIO_pin_number_t pin_number)
{
	return ((GPIOx->IDR >> pin_number) & 1UL);
}

void my_drv_gpio_write_pin(GPIO_TypeDef* GPIOx, GPIO_pin_number_t pin_number, GPIO_pin_state_t pin_state)
{
	if(pin_state == PIN_SET)
		GPIOx->ODR |= (1 << pin_number);
	else if(pin_state == PIN_RESET)
		GPIOx->ODR &= ~(1 << pin_number);
}

void my_drv_gpio_toggle_pin(GPIO_TypeDef* GPIOx, GPIO_pin_number_t pin_number)
{	
	GPIOx->ODR ^= (1 << pin_number);
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

void my_drv_gpio_init(GPIO_TypeDef* GPIOx, GPIO_pin_init_t *GPIO_pin_init, GPIO_pin_number_t pin_number)
{	
	my_drv_gpio_clock_init(GPIOx);

	GPIOx->MODER 	|= (GPIO_pin_init->gpio_mode) << (pin_number*2U);
	GPIOx->OTYPER 	|= (GPIO_pin_init->output_type) << pin_number;
	GPIOx->OSPEEDR 	|= (GPIO_pin_init->speed) << (pin_number*2U);
	GPIOx->PUPDR 	|= (GPIO_pin_init->pupd) << (pin_number*2U);

	if(pin_number <= PIN_7)
		GPIOx->AFR[0] |= ((GPIO_pin_init->af) << (pin_number*4U));
	else
		GPIOx->AFR[1] |= ((GPIO_pin_init->af) << ((pin_number-8)*4U));		
}

void my_drv_gpio_pin_output(GPIO_TypeDef* GPIOx, GPIO_pin_number_t pin_number)
{	
	GPIO_pin_init_t GPIO_pin_init;
	
	GPIO_pin_init.gpio_mode = OUTPUT_MODE;
	GPIO_pin_init.output_type = PUSH_PULL;
	GPIO_pin_init.speed = HIGH_SPEED;
	GPIO_pin_init.pupd = NO_PULL_UP_DOWN;
	
	my_drv_gpio_init(GPIOx, &GPIO_pin_init, pin_number);
}

