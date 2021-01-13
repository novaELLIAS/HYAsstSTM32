
#include "SIM7020Commander/AT.h"
#include <stdio.h>
#include <string.h>
#include "SIM7020Commander/utility_basic.h"

struct tok tok;

struct at_cmd_hanld_t at_cmd_hanld[] = {
	 {"AT+CFUN", AT_Send ,AT_Return},

	 {"AT+CPIN", AT_Send ,AT_Return},

	 {"AT+CSQ", AT_Send ,AT_Return},

	 {"AT+CGATT", AT_Send ,AT_Return},
	 {"AT+CEREG", AT_Send ,AT_Return},

	 {"AT+MIPLCREATEEXT", AT_Send ,AT_Return},
	 {"AT+MIPLADDOBJ", AT_Send ,AT_Return},
	 {"AT+MIPLOPEN", AT_Send ,AT_Return},
	 {"AT+MIPLOBSERVERSP", AT_Send ,AT_Return},
	 {"AT+MIPLDISCOVERRSP", AT_Send ,AT_Return},
	 {"AT+MIPLNOTIFY", AT_Send ,AT_Return},
	 {"AT+MIPLREADRSP", AT_Send ,AT_Return},
	 {"AT+MIPLCLOSE", AT_Send ,AT_Return},
	 {"AT+MIPLDELETE", AT_Send ,AT_Return},

	 {NULL, NULL , NULL}
};

char Buff[2048];
int SIM7020_state = LNW_INIT;
uint8_t ReceiveFinish;
uint8_t ReceiveFlag;
uint8_t ConnectFlag = 0;
uint16_t Buffsiez;
uint16_t Timout;

void CMD_Send(char *buff, char *atcmd, struct tok *tok) {
	int i = 0; char temp[64];
	sprintf (buff, "%s", atcmd);
	if (tok->num != 0) {
		for (i=0; i<tok->num; i++) {
			if(i == 0 && tok->sendstr[i][0] == '?') {
				sprintf(temp,"%s",tok->sendstr[i]);
				strcat(buff,temp);
			} else if(i == 0 && tok->sendstr[i][0] != '?') {
				sprintf(temp,"=%s",tok->sendstr[i]);
				strcat(buff,temp);
			} else {
				sprintf(temp,",%s",tok->sendstr[i]);
				strcat(buff,temp);
			}
		}
	} strcat(buff,"\r\n");
}

int AT_Send(char *atcmd, struct tok *tok) {
	int i; char buff[256];
	for(i=0; i<Retime; ++ i) {
		CMD_Send(buff, atcmd, tok);
		HAL_UART_Transmit_IT(&huart6, (uint8_t*)buff, strlen(buff));
		if(!AT_Return(tok->ret)) {return 0;}
	} return 1;
}

int AT_Return(char *str) {
	uint32_t Time_count = 0;
	Time_count = Timeout;
	memset(Buff, 0, sizeof Buff);
	while(Time_count --) {
		HAL_UART_Receive(&huart6, (uint8_t *)Buff, sizeof Buff, 100);
		if(strstr((const char *)Buff,str)!=NULL) {return 0;}
		HAL_Delay(1);
	} return 1;
}

int AT_CMD_Dispose(struct tok *tok) {
	struct at_cmd_hanld_t *atcmd, *match = NULL;
	char name[32];
	atcmd = at_cmd_hanld;
	stringCapitalize(name, tok->name);
	while(atcmd->atcmd) {
		if(strcmp(atcmd->atcmd,name) == 0) {
			match = atcmd; break;
		} atcmd ++;
  } if(match) return match->send_hanld(match->atcmd,tok);
  else {return 1;}
}

void Buff_clear(struct tok *tok) {
	tok->num = 0;
	memset(tok->sendstr, 0, sizeof(tok->sendstr));
	memset(tok->ret, 0, sizeof(tok->ret));
	memset(Buff, 0, sizeof(Buff));
}
