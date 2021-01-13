
#include "SIM7020Commander/General_Command.h"
#include "SIM7020Commander/utility_basic.h"
#include "SIM7020Commander/AT.h"
#include <string.h>

extern struct tok tok;
extern char Buff[2048];

void Restart_Module(void) {
	strcpy(tok.name,"AT+CFUN");
	tok.num = 2;
	strcpy(tok.sendstr[0],"1");
	strcpy(tok.sendstr[1],"1");
	strcpy(tok.ret,"OK\r\n");
	AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	HAL_Delay(10000);
}

int Query_Signal_Quality(void) {
	int ret, i=0;
	char signal[5];
	char *temp;
	strcpy(tok.name,"AT+CSQ");
	tok.num = 0;
	strcpy(tok.ret,"+CSQ");
	ret = AT_CMD_Dispose(&tok);
	if(!ret) {
		HAL_UART_Receive(&huart6, (uint8_t *)Buff, sizeof Buff, 100);
		//printf("Query_Signal_Quality::Buff: \r\n%s\r\n", Buff);
		temp = strstr((const char *)Buff,"+CSQ:"); temp += 6;
		while(*temp!=0x2C) {signal[i] = *temp; temp++; i++; signal[i] = '\0';}
		printf("Query_Signal_Quality::Signal: \r\n%s\r\n", signal);
		if(stringToInt(signal) < 10 || stringToInt(signal) == 99) {ret = 1;}
	} Buff_clear(&tok);
	return ret;
}

int Read_SIM_Card(void) {
	int ret;
	strcpy(tok.name,"AT+CPIN");
	tok.num = 1;
	strcpy(tok.sendstr[0],"?");
	strcpy(tok.ret,"+CPIN: READY");
	ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	return ret;
}

int GET_LNW_State(void) {
	int ret;
	strcpy(tok.name,"AT+CEREG");
	tok.num = 1;
	strcpy(tok.sendstr[0],"?");
	strcpy(tok.ret,"+CEREG: 0,1");
	ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	return ret;
}

int GET_LNW_Adhere(void) {
	strcpy(tok.name,"AT+CGATT");
	tok.num = 1;
	strcpy(tok.sendstr[0],"?");
	strcpy(tok.ret,"+CGATT: 1\r\n");
	if(AT_CMD_Dispose(&tok)) {
		Buff_clear(&tok);
		return 1;
	} Buff_clear(&tok);
	return 0;
}

int lte_init(void) {
	if(Read_SIM_Card()) return 1;
	if(GET_LNW_State()) return 2;
	if(GET_LNW_Adhere()) return 3;
	return 0;
}
