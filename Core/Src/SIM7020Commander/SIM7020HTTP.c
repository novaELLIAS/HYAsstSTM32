
#include "SIM7020Commander/SIM7020HTTP.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

#define HOST_NAME	api.heclouds.com
#define HOST_PORT	80
#define DEVICE_ID	644250210
#define APIKey      fhAS54e5X8HL5wcaB6ZW74oA3vo=

uint8_t *ATmap[] = {
		(uint8_t *) "AT\r\n",
		(uint8_t *) "AT+CSOC=1,1,1\r\n",
		(uint8_t *) "AT+CSOCON=0,80,\"183.230.40.33\"\r\n",
		(uint8_t *) "AT+CSOSEND=0,0,",
		(uint8_t *) "AT+CSOCL=0\r\n"
};

char buf[2048], snd[2048], tmp[2048];

void intToString(int n, char *str) {
	uint8_t  i = 0;
	int temp = n;
	while((temp/=10)>0) {++ i;}
	str[i] = '\0';
	do {str[-- i] = n%10 + '0';}
	while ((n/=10) > 0); return;
}

int num[2048];
const char hex[16]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void strToHex (char *src, char *dst) {
	int len = strlen(src), i, pos = 0, top = 0;
	char tmp;
	for (i=0; i<len; ++ i) {
		tmp = src[i], top = 0;
		while (tmp) {
			num[top ++] =tmp % 16;
			tmp /= 16;
		} for (top=top-1; ~top; -- top) dst[++ pos] = hex[num[top]];
	} dst[pos] = '\0';
}

void HTTP_Send_Data (float lat, float log, float spd, int isAccident) {
	HAL_UART_Transmit_IT(&huart6, (uint8_t *) ATmap[0], sizeof(ATmap[0])); HAL_Delay(100);
	HAL_UART_Transmit_IT(&huart6, (uint8_t *) ATmap[1], sizeof(ATmap[1])); HAL_Delay(100);
	HAL_UART_Transmit_IT(&huart6, (uint8_t *) ATmap[2], sizeof(ATmap[2])); HAL_Delay(100);

	sprintf(buf, "{\"La\":\"%.6f\",\"Lo\":\"%.6f\",\"S\":\"%.2f\",\"F\":\"%d\"}\r\n", lat, log, spd, isAccident);
	printf ("%s\r\n", buf);

	strcpy(snd, "POST /devices/644250210/datapoints?type=3 HTTP/1.1\r\napi-key:fhAS54e5X8HL5wcaB6ZW74oA3vo=\r\nHost:api.heclouds.com\r\nConnection:close\r\nContent-Length:");

	intToString(strlen(buf)-2, tmp);
	strcat(snd, tmp);
	strcat(snd, "\r\n\r\n");
	strcat(snd, buf);

	strcpy(buf, "AT+CSOSEND=0,0,");
	strToHex(snd, tmp);
	strcat(buf, tmp);
	strcat(buf, "\r\n");

	HAL_UART_Transmit_IT(&huart6, (uint8_t *) buf, sizeof(buf)); HAL_Delay(1000);

	HAL_UART_Transmit_IT(&huart6, (uint8_t *) ATmap[4], sizeof(ATmap[4]));
}
