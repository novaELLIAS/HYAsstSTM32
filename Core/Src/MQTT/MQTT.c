
#include "MQTT/MQTT.h"
#include "MQTT/utility.h"
#include "MQTT/general_command.h"
#include "MQTT/at.h"
#include <stdio.h>
#include <string.h>

extern struct tok tok;
extern char Buff[256];
extern int SIM7020_state;

char ClientID[20] = "\"653000696\"";
char UserNAME[20] = "387253";
char Password[20] = "1096";
char TOPIC[20]    = "\"test\"";

char RemoteIP[20]    = "\"183.230.40.39\"";
char RemotePort[6]   = "6002";

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

int PUB_Messag(char *Messag)
{
  int ret;
	char Messag_len[5];
	intToString(strlen(Messag)-2,(uint8_t*)Messag_len);
	strcpy(tok.name,"AT+CMQPUB");
	tok.num = 7;
	strcpy(tok.sendstr[0],"0");
	strcpy(tok.sendstr[1],TOPIC);
	strcpy(tok.sendstr[2],"0");
	strcpy(tok.sendstr[3],"0");
	strcpy(tok.sendstr[4],"0");
	strcpy(tok.sendstr[5],Messag_len);
	strcpy(tok.sendstr[6],Messag);
	strcpy(tok.ret,"OK");
	ret = AT_CMD_Dispose(&tok);
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
