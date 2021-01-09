#include <string.h>
#include <stdio.h>
extern u8 u_buf[64];
#define printf(...)  HAL_UART_Transmit((UART_HandleTypeDef *)&huart1, (uint8_t *)u_buf,\
	                        sprintf((char*)u_buf,__VA_ARGS__), 0xFFFF);
