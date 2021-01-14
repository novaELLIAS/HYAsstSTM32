
#include <MQTT/utility.h>
#include <stdio.h>
#include <string.h>

void intToString (u16 n, u8 *str) {
	u8 len = 0; u16 tmp = n;
	while (tmp/=10) {++ len;}
	do {str[len --] = n%10+'0';}
	while (n/=10); return;
}

int stringToInt (char *str) {
	int ret = 0;
	while (*str ^ '\0') {
		ret = (ret<<1) + (ret<<3);
		ret += *str ^ '0';
		str ++;
	} return ret;
}

int getIntLen (int a) {
	int ret = 1;
	while (a /= 10) ++ ret;
	return ret;
}

void stringCapitalize (char *dest, char *str) {
	int pos = 0, len = strlen (str);
	for (; pos<len; ++ pos) {
		if (str[pos]<='z' && str[pos]>='a') dest[pos]=str[pos]-32;
		else dest[pos] = str[pos];
	} dest[pos] = '\0';
}

int num[2048];
const char hex[16]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void strToHex (char *dst, char *src) {
	int len = strlen(src), i, pos = 0, top = 0;
	char tmp;
	for (i=0; i<len; ++ i) {
		tmp = src[i], top = 0;
		while (tmp) {
			num[top ++] = tmp % 16;
			tmp >>= 4;
		} for (top=top-1; top>=0; -- top) dst[pos ++] = hex[num[top]];
	} dst[pos] = '\0';
}
