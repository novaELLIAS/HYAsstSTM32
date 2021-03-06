
#include "main.h"
#include <stdio.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

struct tok {
	char name[32];
	char num;
	char sendstr[20][256];
	char ret[256];
};

struct at_cmd_hanld_t {
	char *atcmd;
	int (*send_hanld)   (char *atcmd, struct tok *tok);
	int (*return_hanld) (char *str, int flag);
};

#define Timeout            10
#define Retime              2

#define LNW_INIT            0
#define SET_LNW_PARAMETER   1
#define CONNECT_OK          2

int AT_CMD_Dispose(struct tok *tok);

void CMD_Send(char *buff, char *atcmd, struct tok *tok);

int AT_Send(char *atcmd, struct tok *tok);

int AT_Return(char *str, int flag);

void Buff_clear(struct tok *tok);
