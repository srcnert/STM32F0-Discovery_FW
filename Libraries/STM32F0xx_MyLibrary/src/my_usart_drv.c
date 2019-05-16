#include "stm32f0xx.h"
#include "my_gpio_drv.h"
#include "my_usart_drv.h"
#include "rtt_log.h"

volatile bool tx_ready = false;
volatile bool rx_ready = false;

uint8_t rx_data[256];

void USART1_IRQHandler()
{
//	logi("USART1_IRQ");

	if(((USART1->ISR >> 6) & 1UL) == 1)
	{
		logi("TC");
		USART1->ICR |= (1 << 6); // Clear TC flag
		tx_ready = true;
	}

	if(((USART1->ISR >> 5) & 1UL) == 1)
	{
		logi("RXNE");
		static uint8_t i = 0;
		uint8_t data = USART1->RDR;
		rx_data[i] = data;
		i++;
		if(data == '\n')
		{
			i = 0;
			rx_ready = true;
		}	
	}

}

void my_drv_usart_init(USART_conf_t *USART_conf)
{
	my_drv_gpio_init(USART_conf->GPIOx, USART_conf->USART_pin_init, USART_conf->TX_PIN);
	my_drv_gpio_init(USART_conf->GPIOx, USART_conf->USART_pin_init, USART_conf->RX_PIN);
	
	RCC->APB2ENR |= (1 << 14); // USART1 clock enabled 
	
	USART1->CR1 &= ~(1 << 0); // USART disabled
	
	USART1->CR1 = (((USART_conf->USART_wordlength>>1) & 1UL) << 28) | // M1: 1 start bit - 8 data bits - n stop bit
								//(0 << 28) |
								(0 << 27) | // End of block interrupt disabled
								(0 << 26) | // Receiver timeout interrupt disabled
								(0 << 21) | // Driver Enable assertion time
								(0 << 16) | // Driver Enable de-assertion time
								((USART_conf->USART_oversampling) << 15) | // Oversampling by 8
								//(1 << 15) |
								(0 << 14) | // Character match interrupt disabled
								(0 << 13) | // Receiver in active mode permanently
								(((USART_conf->USART_wordlength) & 1UL) << 12) | // M0: 8 data bit
								//(0 << 12) |
								(0 << 11) | // Receiver wake-up method: idle 
								((USART_conf->USART_parity) << 10) | // Parity control disabled
								((USART_conf->USART_parity_mode) << 9)  | // Partity selection: Even parity
								//(0 << 10) |
								//(0 << 9)  |

								(0 << 8)  | // PE interrupt disabled
								(0 << 7)  | // TXE interrupt disabled
								(1 << 6)  | // TC interrupt disable
								(1 << 5)  | // RXNE interrupt disabled
								(0 << 4)  | // IDLE interrupt disabled
								
								(1 << 3)  | // Transmitter enabled
								(1 << 2)  | // Receiver disabled
								(0 << 1)  | // USART not able to wake up the MCU from stop mode
								(0 << 0);		// USART disabled

	USART1->CR2 = (0 << 23) | // Receiver timeout feature disabled. 
								(0 << 20) | // Auto baud rate detection is disabled. 
								(0 << 19) | // Data is transmitted/received with data bit-0 first, following the start bit. 
								((USART_conf->USART_stopbit) << 12) | // 1 stop bit
								//(0 << 12) |
								(0 << 11);  // Clock disabled
	
	USART1->CR3 = (0 << 22) | // wake-up from stop mode interrupt disabled
								(0 << 11) | // Three sample bit error
								(0 << 10) | // CTS interrupt disabled
								(0 << 9)  | // CTS disabled
								(0 << 8)  | // RTS disabled
								(0 << 7)  | // DMA transmitter disabled
								(0 << 6)  |	// DMA receiver disabled
								(0 << 0);	  // Error interrupt disabled
	/*
		Bits 15:4 BRR[15:4]
		BRR[15:4] = USARTDIV[15:4]
		
		Bits 3:0 BRR[3:0]
		When OVER8 = 0, BRR[3:0] = USARTDIV[3:0].
		When OVER8 = 1:
		
		BRR[2:0] = USARTDIV[3:0] shifted 1 bit to the right.
		BRR[3] must be kept cleared.
	*/
	if(USART_conf->USART_oversampling == USART_oversampling_16)
	{
			USART1->BRR = (48000000) / (USART_conf->USART_baudrate);
	}
	else if(USART_conf->USART_oversampling == USART_oversampling_8)
	{
			uint32_t usartdiv = (2 * 48000000) / (USART_conf->USART_baudrate);
			uint32_t usartdiv_lsb = (usartdiv & 0xF) >> 1;
			usartdiv = (usartdiv & 0xFFFFFFF0) | usartdiv_lsb;
			USART1->BRR =  usartdiv;
	}
	
	USART1->CR1 |= (1 << 0); // USART enabled
	
	uint8_t USART1_IRQ_PRIORITY = 0;
	NVIC_SetPriority(USART1_IRQn, USART1_IRQ_PRIORITY);
	NVIC_ClearPendingIRQ(USART1_IRQn);
	NVIC_EnableIRQ(USART1_IRQn);
}
	
void my_drv_usart_send_char(char x)
{
	 USART1->TDR = x;
	 while(((USART1->ISR >> 7) & 1UL) != 1);
//   while (!tx_ready); 
//	 tx_ready = false;	// wait for TX (transmission complete)
}
