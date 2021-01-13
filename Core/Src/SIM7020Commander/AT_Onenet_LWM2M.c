
#include "SIM7020Commander/AT_Onenet_LWM2M.h"
#include "SIM7020Commander/General_Command.h"
#include "SIM7020Commander/utility_basic.h"
#include "SIM7020Commander/AT.h"
#include <string.h>
#include <stdlib.h>

extern struct tok tok;
extern char Buff[2048];
extern int SIM7020_state;

char RemoteIP[20] = "\"183.230.40.39\"";
char Object[20]   = "3303";
char MSGID[10];

int CONNECT_Server(void) {
	int ret;
	strcpy(tok.name,"AT+MIPLCREATEEXT");
	tok.num = 2;
	strcpy(tok.sendstr[0],RemoteIP);
	strcpy(tok.sendstr[1],"1");
	strcpy(tok.ret,"+MIPLCREATEEXT: 0");
	ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	return ret;
}

void ADD_Object(void) {
	strcpy(tok.name,"AT+MIPLADDOBJ");
	tok.num = 6;
	strcpy(tok.sendstr[0],"0");
	strcpy(tok.sendstr[1],Object);
	strcpy(tok.sendstr[2],"2");
	strcpy(tok.sendstr[3],"11");
	strcpy(tok.sendstr[4],"7");
	strcpy(tok.sendstr[5],"3");
	strcpy(tok.ret,"OK");
	AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
}

int Registered_Plant(void) {
	int ret;
	strcpy(tok.name,"AT+MIPLOPEN");
	tok.num = 2;
	strcpy(tok.sendstr[0],"0");
	strcpy(tok.sendstr[1],"180");
	strcpy(tok.ret,"+MIPLEVENT: 0,6");
	ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(!ret) {
		HAL_Delay(50000);
		Reply_Observe_Command();
		Reply_Discover_Command();
	} return ret;
}

int Reply_Observe_Command(void) {
	int ret = AT_Return("+MIPLOBSERVE:");
	if(!ret) {
		HAL_UART_Receive(&huart6, (uint8_t *)Buff, sizeof Buff, 100);
		Parse_Observe_Command(Buff,MSGID);
		Buff_clear(&tok);
		strcpy(tok.name,"AT+MIPLOBSERVERSP");
		tok.num = 3;
		strcpy(tok.sendstr[0],"0");
		strcpy(tok.sendstr[1],MSGID);
		strcpy(tok.sendstr[2],"1");
		strcpy(tok.ret,"OK");
		ret = AT_CMD_Dispose(&tok);
		Buff_clear(&tok);

		ret = AT_Return("+MIPLOBSERVE");
		HAL_UART_Receive(&huart6, (uint8_t *)Buff, sizeof Buff, 100);
		Parse_Observe_Command(Buff,MSGID);
		strcpy(tok.name,"AT+MIPLOBSERVERSP");
		tok.num = 3;
		strcpy(tok.sendstr[0],"0");
		strcpy(tok.sendstr[1],MSGID);
		strcpy(tok.sendstr[2],"1");
		strcpy(tok.ret,"OK");
		ret = AT_CMD_Dispose(&tok);
		Buff_clear(&tok);
	} return ret;
}

void Parse_Observe_Command(char *buff,char *ret) {
	int i = 0; buff += 16;
	while(*(buff+i) != ',') {
		*(ret+i) = *(buff+i); i ++;
	} ret[i] = '\0';
}

int Reply_Discover_Command(void) {
	int ret = AT_Return("+MIPLDISCOVER");
	char msgid[10];
	if(!ret) {
		HAL_UART_Receive(&huart6, (uint8_t *)Buff, sizeof Buff, 100);
		Parse_Discover_Command(Buff,msgid);
		Buff_clear(&tok);
		strcpy(tok.name,"AT+MIPLDISCOVERRSP");
		tok.num = 5;
		strcpy(tok.sendstr[0],"0");
		strcpy(tok.sendstr[1],msgid);
		strcpy(tok.sendstr[2],"1");
		strcpy(tok.sendstr[3],"4");
		strcpy(tok.sendstr[4],"\"5701\"");
		strcpy(tok.ret,"OK");
		ret = AT_CMD_Dispose(&tok);
		Buff_clear(&tok);
	} return ret;
}

void Parse_Discover_Command(char *buff,char *ret) {
	int i = 0;
	buff += 17;
	while(*(buff+i) != ',') {
		*(ret+i) = *(buff+i); i ++;
	} ret[i] = '\0';
}

int SEND_Messag(char *messag) {
	int ret;
	char *messag_len = NULL;

	messag_len = malloc(5);
	intToString(strlen(messag),(uint8_t*)messag_len);
	strcpy(tok.name,"AT+MIPLNOTIFY");
	tok.num = 10;
	strcpy(tok.sendstr[0],"0");
	strcpy(tok.sendstr[1],MSGID);
	strcpy(tok.sendstr[2],"3303");
	strcpy(tok.sendstr[3],"1");
	strcpy(tok.sendstr[4],"5701");
	strcpy(tok.sendstr[5],"1");
	strcpy(tok.sendstr[6],messag_len);
	strcpy(tok.sendstr[7],messag);
	strcpy(tok.sendstr[8],"0");
	strcpy(tok.sendstr[9],"0");
	strcpy(tok.ret,"OK");
	ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	return ret;
}

int Reply_READ_Command(char *messag) {
	int ret;
	char msgid[10];
	char *messag_len = NULL;
	ret = AT_Return("+MIPLREAD");
	if(!ret) {
		HAL_UART_Receive(&huart6, (uint8_t *)Buff, sizeof Buff, 100);
		Parse_READ_Command(Buff,msgid);
		Buff_clear(&tok);
		messag_len = malloc(5);
		intToString(strlen(messag)-2,(uint8_t*)messag_len);
		strcpy(tok.name,"AT+MIPLREADRSP");
		tok.num = 11;
		strcpy(tok.sendstr[0],"0");
		strcpy(tok.sendstr[1],msgid);
		strcpy(tok.sendstr[2],"1");
		strcpy(tok.sendstr[3],"3303");
		strcpy(tok.sendstr[4],"1");
		strcpy(tok.sendstr[5],"5701");
		strcpy(tok.sendstr[6],"4");
		strcpy(tok.sendstr[7],messag_len);
		strcpy(tok.sendstr[8],messag);
		strcpy(tok.sendstr[9],"0");
		strcpy(tok.sendstr[10],"0");
		strcpy(tok.ret,"OK");
		ret = AT_CMD_Dispose(&tok);
		Buff_clear(&tok);
	} return ret;
}

void Parse_READ_Command(char *buff,char *ret) {
	int i = 0; buff += 13;
	while(*(buff+i) != ',') {
		*(ret+i) = *(buff+i); i ++;
	} ret[i] = '\0';
}

int LOGOUT_Device(void) {
	strcpy(tok.name,"AT+MIPLCLOSE");
	tok.num = 1;
	strcpy(tok.sendstr[0],"0");
	strcpy(tok.ret,"+MIPLEVENT: 0,15");
	int ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	return ret;
}

int CLOSE_Server(void) {
  strcpy(tok.name,"AT+MIPLDELETE");
	tok.num = 1;
	strcpy(tok.sendstr[0],"0");
	strcpy(tok.ret,"OK");
	int ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	return ret;
}

void ONENET_LWM2M(void) {
	printf("ONENET_LWM2M triggered. SIM7020_state: %d\r\n", SIM7020_state);
	switch(SIM7020_state) {
		case LNW_INIT:
			if(!lte_init()) {SIM7020_state = SET_LNW_PARAMETER;}
			break;
		case SET_LNW_PARAMETER:
			LOGOUT_Device();
			CLOSE_Server();
			if(CONNECT_Server()) {return;}
			ADD_Object();
			if(Registered_Plant()) {return;}
			SIM7020_state = CONNECT_OK;
			break;
		case CONNECT_OK:
			SEND_Messag("123");
			Reply_READ_Command("\"123\"");
			break;
	}
}
