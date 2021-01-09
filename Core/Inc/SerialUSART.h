//
//#include <string.h>
//#include <stdio.h>
//extern uint8_t u_buf[1024];
//#define printf(...)  HAL_UART_Transmit((UART_HandleTypeDef *)&huart1, (uint8_t *)u_buf, sprintf((char*)u_buf,__VA_ARGS__), 0xFFFF);

#include <stdio.h>
#include "main.h"

//#ifdef __GNUC__
//#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#else
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//#endif
//
//PUTCHAR_PROTOTYPE {
//	HAL_UART_Transmit((UART_HandleTypeDef *)&huart1, (uint8_t*)&ch, 1, 0xFFFF);
//	return ch;
//}
