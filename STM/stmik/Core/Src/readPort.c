#include "readPort.h"
#include "gpio.h"
#include "usart.h"
#include "stm32f4xx_hal.h"
#include <string.h>


char  reading[24];

char readPort(char * option)
{

	if (strcmp(option, "RDPB") == 0) {

			reading[0] = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) ? '1' : '0');
			reading[1] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) ? '1' : '0');
			reading[2] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) ? '1' : '0');
			reading[3] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) ? '1' : '0');
			reading[4] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) ? '1' : '0');
			reading[5] = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) ? '1' : '0');
			HAL_UART_Transmit(&huart2, reading, 6, 100);
			memset(reading, '0', 24);
		}
		else if(strcmp(option, "RDPC") == 0) {
			reading[0] = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) ? '1' : '0');
			reading[1] = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) ? '1' : '0');
			reading[2] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) ? '1' : '0');
			reading[3] = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) ? '1' : '0');
			reading[4] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) ? '1' : '0');
			reading[5] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) ? '1' : '0');
			HAL_UART_Transmit(&huart2, reading, 6, 100);
			memset(reading, '0', 24);
		}
		else if (strcmp(option, "RDPD") == 0) {
			reading[0] = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) ? '1' : '0');
			reading[1] = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) ? '1' : '0');
			reading[2] = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) ? '1' : '0');
			reading[3] = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) ? '1' : '0');
			reading[4] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) ? '1' : '0');
			reading[5] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) ? '1' : '0');
			reading[6] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) ? '1' : '0');
			reading[7] = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4) ? '1' : '0');
			HAL_UART_Transmit(&huart2, reading, 8, 100);
			memset(reading, '0', 24);
		}
		else if (strcmp(option, "RDPE") == 0) {
			reading[0] = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) ? '1' : '0');
			reading[1] = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) ? '1' : '0');
			reading[2] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) ? '1' : '0');
			reading[3] = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) ? '1' : '0');
			HAL_UART_Transmit(&huart2, reading, 4, 100);
			memset(reading, '0', 24);
		}
	else if (strcmp(option, "RDAL") == 0) {
		reading[0] = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) ? '1' : '0');
		reading[1] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) ? '1' : '0');
		reading[2] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) ? '1' : '0');
		reading[3] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) ? '1' : '0');
		reading[4] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) ? '1' : '0');
		reading[5] = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) ? '1' : '0');
		reading[6] = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) ? '1' : '0');
		reading[7] = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) ? '1' : '0');
		reading[8] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) ? '1' : '0');
		reading[9] = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) ? '1' : '0');
		reading[10] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) ? '1' : '0');
		reading[11] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) ? '1' : '0');
		reading[12] = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) ? '1' : '0');
		reading[13] = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) ? '1' : '0');
		reading[14] = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) ? '1' : '0');
		reading[15] = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) ? '1' : '0');
		reading[16] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) ? '1' : '0');
		reading[17] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) ? '1' : '0');
		reading[18] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) ? '1' : '0');
		reading[19] = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4) ? '1' : '0');
		reading[20] = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) ? '1' : '0');
		reading[21] = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) ? '1' : '0');
		reading[22] = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) ? '1' : '0');
		reading[23] = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) ? '1' : '0');
		HAL_UART_Transmit(&huart2, reading, 24, 100);
		memset(reading, '0', 24);
	}
	memset(option, '0', 4);
	return reading;
};
