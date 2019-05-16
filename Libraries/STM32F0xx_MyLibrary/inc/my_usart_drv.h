#ifndef MY_USART_DRV
#define MY_USART_DRV

#include <stdbool.h>
#include <stdint.h>

#include "stm32f0xx.h"
#include "my_gpio_drv.h"

/*
 * USART INTERRUPTS	
 *																																				Event flag				Enable Control bit
 * Transmit data register empty 																					TXE 							TXEIE
 * CTS interrupt 																													CTSIF 						CTSIE
 * Transmission Complete 																									TC 								TCIE
 * Receive data register not empty (data ready to be read) 								RXNE							RXNEIE
 * Overrun error detected 																								ORE								RXNEIE
 * Idle line detected 																										IDLE 							IDLEIE
 * Parity error 																													PE 								PEIE
 * LIN break 																															LBDF 							LBDIE
 * Noise Flag, Overrun err and Framing Err in multibuffer communication. 	NF or ORE or FE 	EIE
 * Character match 																												CMF 							CMIE
 * Receiver timeout 																											RTOF 							RTOIE
 * End of Block 																													EOBF 							EOBIE										Smartcard
 * Wakeup from Stop mode 																									WUF 							WUFIE
*/

typedef enum USART_Wordlength_e 
{
	USART_Wordlength_7bit = 2, // If parity is enabled, 6-bit data 1-bit parity
	USART_Wordlength_8bit = 0, // If parity is enabled, 7-bit data 1-bit parity
	USART_Wordlength_9bit = 1  // If parity is enabled, 8-bit data 1-bit parity
} USART_Wordlengh_t;

typedef enum USART_oversampling_e 
{
	USART_oversampling_8 = 1,
	USART_oversampling_16 = 0
} USART_oversampling_t;

typedef enum USART_baudrate_e 
{
	USART_baudrate_9600 	= 9600,
	USART_baudrate_19200 	= 19200,
	USART_baudrate_38400 	= 38400,
	USART_baudrate_57600 	= 57600,
	USART_baudrate_115200 = 115200,
	USART_baudrate_230400 = 230400,
	USART_baudrate_460800 = 460800,
	USART_baudrate_921600 = 921600,
	USART_baudrate_2MBps 	= 2000000,
	USART_baudrate_3MBps 	= 3000000,	
} USART_baudrate_t;

typedef enum USART_partity_e 
{
	USART_parity_disabled = 0,
	USART_parity_enabled = 1,
} USART_parity_t; 

typedef enum USART_partity_mode_e 
{
	USART_parity_even = 0,
	USART_parity_odd = 1,
} USART_parity_mode_t; 

typedef enum USART_stopbit_e 
{
	USART_stopbit_1 = 0,
	USART_stopbit_0_5 = 1,
	USART_stopbit_2 = 2,
	USART_stopbit_1_5 = 3,
} USART_stopbit_t; 

typedef struct USART_conf_s
{
	GPIO_pin_number_t 		TX_PIN;
	GPIO_pin_number_t			RX_PIN;
	GPIO_TypeDef					*GPIOx;
	GPIO_pin_init_t 			*USART_pin_init;
	USART_Wordlengh_t			USART_wordlength;
	USART_oversampling_t 	USART_oversampling;
	USART_parity_t 				USART_parity;
	USART_parity_mode_t 	USART_parity_mode;
	USART_stopbit_t 			USART_stopbit;
	USART_baudrate_t 			USART_baudrate;
} USART_conf_t;

extern volatile bool rx_ready;

void my_drv_usart_init(USART_conf_t *USART_conf);
void my_drv_usart_send_char(char x);

#endif /* MY_USART_DRV */
