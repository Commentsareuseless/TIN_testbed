#include "send.h"
#include "gpio.h"
#include "usart.h"
#include "stm32f4xx_hal.h"



void send(char* reading)
{

	HAL_UART_Transmit(&huart2, reading, 24, 100);
	memset(reading, '0', 24);
};
