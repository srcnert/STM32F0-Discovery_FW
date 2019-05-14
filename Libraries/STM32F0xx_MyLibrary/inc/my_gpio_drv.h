#ifndef MY_GPIO_DRV
#define MY_GPIO_DRV

#include <stdbool.h>
#include "stm32f0xx.h"

typedef enum gpio_mode_e /* GPIO_MODE */
{
	INPUT_MODE = 0U, // Input mode (reset state)
	OUTPUT_MODE = 1U, // General purpose output mode
	ALTERNATE_FUNCTION_MODE = 2U, //Alternate function mode
	ANALOG_MODE = 3U //Analog mode
} GPIO_mode_t;

typedef enum gpio_output_type_e /* OUTPUT_TYPE */
{
	PUSH_PULL = 0U,  // Output push-pull (reset state)
	OPEN_DRAIN = 1U  // Output open-drain
} GPIO_output_type_t;

typedef enum gpio_speed_e /* SPEED */
{
	LOW_SPEED = 0U,		// Low speed
	MEDIUM_SPEED = 1U,	// Medium speed
	HIGH_SPEED = 3U,	// High speed
} GPIO_speed_t;

typedef enum gpio_pupd_e/* PUPD */
{
	NO_PULL_UP_DOWN = 0U,	// No pull-up, pull-down
	PULL_UP = 1U,	// Pull-up
	PULL_DOWN = 2U	// Pull-down
} GPIO_pupd_t;

typedef enum gpio_alternate_function_e
{
	ALTERNATE_FUNCTION_0 = 0U, // AF0
	ALTERNATE_FUNCTION_1 = 1U, // AF1
	ALTERNATE_FUNCTION_2 = 2U, // AF2
	ALTERNATE_FUNCTION_3 = 3U, // AF3
	ALTERNATE_FUNCTION_4 = 4U, // AF4
	ALTERNATE_FUNCTION_5 = 5U, // AF5
	ALTERNATE_FUNCTION_6 = 6U, // AF6
	ALTERNATE_FUNCTION_7 = 7U, // AF7
} GPIO_alternate_function_t;

typedef enum gpio_pin_state_e
{
	PIN_RESET = 0U,	
	PIN_SET = 1U
} GPIO_pin_state_t;

typedef enum gpio_pin_number_e
{
	PIN_0,
	PIN_1,
	PIN_2,
	PIN_3,
	PIN_4,
	PIN_5,
	PIN_6,
	PIN_7,
	PIN_8,
	PIN_9,
	PIN_10,
	PIN_11,
	PIN_12,
	PIN_13,
	PIN_14,
	PIN_15
} GPIO_pin_number_t;

typedef struct gpio_pin_init_s{
    GPIO_mode_t 				gpio_mode;
    GPIO_output_type_t 			output_type;
    GPIO_speed_t 				speed;
    GPIO_pupd_t				  	pupd;
    GPIO_alternate_function_t 	af;
} GPIO_pin_init_t;

bool my_drv_gpio_read_pin(GPIO_TypeDef *GPIOx, GPIO_pin_number_t pin_number);
void my_drv_gpio_write_pin(GPIO_TypeDef *GPIOx, GPIO_pin_number_t pin_number, GPIO_pin_state_t pin_state);
void my_drv_gpio_toggle_pin(GPIO_TypeDef *GPIOx, GPIO_pin_number_t pin_number);

void my_drv_gpio_clock_init(GPIO_TypeDef *GPIOx);
void my_drv_gpio_init(GPIO_TypeDef *GPIOx, GPIO_pin_init_t *GPIO_pin_init, GPIO_pin_number_t pin_number);
void my_drv_gpio_pin_output(GPIO_TypeDef *GPIOx, GPIO_pin_number_t pin_number);

#endif /* MY_GPIO_DRV */
