
#include "SIM7020Commander/AT.h"
#include <stdio.h>
#include <string.h>
#include "SIM7020Commander/utility_basic.h"

struct tok tok;

struct at_cmd_handld_t at_cmd_hanld[] = {
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

int AT_Send(char *atcmd, struct tok *tok) {
	return 1;
}

int AT_Return(char *str) {
	return 1;
}
