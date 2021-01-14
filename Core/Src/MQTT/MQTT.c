
#include "MQTT/MQTT.h"
#include "MQTT/utility.h"
#include "MQTT/general_command.h"
#include "MQTT/at.h"
#include "LED_Functions/LED_OUTPUT.h"
#include <stdio.h>
#include <string.h>

extern struct tok tok;
extern char Buff[256];
extern int SIM7020_state;

char ClientID[20] = "\"653000696\"";
char UserNAME[20] = "387253";
char Password[20] = "10961096";
char TOPIC[20]    = "\"$dp\"";

char RemoteIP[20]    = "\"183.230.40.39\"";
char RemotePort[6]   = "6002";

char hexToSend[256], strToSend[256];

int CONNECT_Server(void) {
	int ret;
	strcpy(tok.name, "AT+CMQNEW");
	tok.num = 4;
	strcpy(tok.sendstr[0],RemoteIP);
	strcpy(tok.sendstr[1],RemotePort);
	strcpy(tok.sendstr[2],"12000");
	strcpy(tok.sendstr[3],"100");
	strcpy(tok.ret,"OK");
	ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("OneNET Server Connection Fail.\r\n");
	else printf("OneNET Server Connection Success.\r\n");
	return ret;
}

int Registered_Plant(void) {
	int ret;
	strcpy(tok.name,"AT+CMQCON");
	tok.num = 8;
	strcpy(tok.sendstr[0],"0");
	strcpy(tok.sendstr[1],"4");
	strcpy(tok.sendstr[2],ClientID);
	strcpy(tok.sendstr[3],"180");
	strcpy(tok.sendstr[4],"0");
	strcpy(tok.sendstr[5],"0");
	strcpy(tok.sendstr[6],UserNAME);
	strcpy(tok.sendstr[7],Password);
	strcpy(tok.ret,"OK");
	ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("Device Register Fail.\r\n");
	else printf("Device Register Success.\r\n");
	return ret;
}

int SUB_Topic(void) {
	int ret;
	strcpy(tok.name,"AT+CMQSUB");
	tok.num = 3;
	strcpy(tok.sendstr[0],"0");
	strcpy(tok.sendstr[1],TOPIC);
	strcpy(tok.sendstr[2],"1");
	strcpy(tok.ret,"OK");
	ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("Subscribe Fail.\r\n");
	else printf("Subscribe Success.\r\n");
	return ret;
}

int PUB_Messag(char *Messag) {
	int ret, len = strlen(Messag) - 2;
	char Messag_len[5];
	memset(Messag_len, 0, sizeof Messag_len);
	intToString(len,(uint8_t*)Messag_len);
	strcpy(tok.name,"AT+CMQPUB");
	tok.num = 7;
	strcpy(tok.sendstr[0],"0");
	strcpy(tok.sendstr[1],TOPIC);
	strcpy(tok.sendstr[2],"0");
	strcpy(tok.sendstr[3],"0");
	strcpy(tok.sendstr[4],"0");
	strcpy(tok.sendstr[5],Messag_len);
	//printf("Message_len: %send\r\n",Messag_len);
	strcpy(tok.sendstr[6],strToSend);
	//printf("Message_TEST1\r\n");
	strcpy(tok.ret,"OK");
	ret = AT_CMD_Dispose(&tok);
	//printf("Message_TEST2\r\n");
	Buff_clear(&tok);
	if(ret) printf("Message PUB Fail.\r\n");
	else printf("Message PUB Success.\r\n");
	return ret;
}

int UNSUB_Topic(void) {
	int ret;
	strcpy(tok.name,"AT+CMQUNSUB");
	tok.num = 2;
	strcpy(tok.sendstr[0],"0");
	strcpy(tok.sendstr[1],TOPIC);
	strcpy(tok.ret,"OK");
	ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("UNSUB Topic Fail.\r\n");
	else printf("UNSUB Topic Success.\r\n");
	return ret;
}

int Close_Server(void) {
	int ret;
	strcpy(tok.name,"AT+CMQDISCON");
	tok.num = 1;
	strcpy(tok.sendstr[0],"0");
	strcpy(tok.ret,"OK");
	ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("ONENET Disconnect Fail.\r\n");
	else printf("ONENET Disconnect Success.\r\n");
	return ret;
}

int HEX_Mode_Enable(void) {
	int ret;
	strcpy(tok.name,"AT+CREVHEX");
	tok.num = 1;
	strcpy(tok.sendstr[0],"1");
	strcpy(tok.ret,"OK");
	ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("HEX mode Fail.\r\n");
	else printf("HEX mode Success.\r\n");
	return ret;
}

void Messag_Analysis(char *buff) {
	char *str1;
	char *str2;
	char top[64];
	char Messag[64];
	str1 = strstr(buff,"\"") + 1;
	str2 = strstr(str1,"\"");
	strncpy(top,str1,str2-str1);
	top[str2-str1] = '\0';
	str1 = strstr(str2+1,"\"") + 1;
	str2 = strstr(str1,"\"");
	strncpy(Messag,str1,str2-str1);
	Messag[str2-str1] = '\0';
	printf("TOP: %s\r\n",top);
	printf("Message: %s\r\n",Messag);
}

int Messag_Bispose(void) {
	if(!AT_Return("+CMQPUB:", 1)) {
		Messag_Analysis(Buff);
		Buff_clear(&tok); return 0;
	} return 1;
}

void Messag_Builder (dataPoints *DP) {
	sprintf(strToSend, "{\"La\":\"%.6f\",\"Lo\":\"%.6f\",\"S\":\"%.2f\",\"A\":\"%.1f\",\"F\":\"%d\"}", DP->latitude, DP->longitude, DP->speed, DP->pdop, DP->flag);
	int len = strlen(strToSend);
	//printf("Messag_Builder 1: %s\r\n", strToSend);
	strToHex(hexToSend, strToSend);
	sprintf(strToSend, "\"03%04X", len);
	strcat(strToSend, hexToSend);
	strcat(strToSend, "\"");
	//printf("Messag_Builder 2: %s\r\n", strToSend);
}

void ONENET_MQTT(dataPoints *DP) {
	switch(SIM7020_state) {
		case LNW_INIT:
			if(!lte_init()) SIM7020_state = SET_LNW_PARAMETER;
			break;
		case SET_LNW_PARAMETER:
			Close_Server();
			if(CONNECT_Server()) return;
			if(Registered_Plant()) return;
			if(SUB_Topic()) return;
			SIM7020_state = CONNECT_OK;
			break;
		case CONNECT_OK:
			LED_DATUPD_ON();
			HEX_Mode_Enable();
			Messag_Builder(DP);
			PUB_Messag(strToSend);
			LED_DATUPD_OFF();
	}
}
