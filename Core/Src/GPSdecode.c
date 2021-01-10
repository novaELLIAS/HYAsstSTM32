
#include <stdio.h>
#include <string.h>
#include "GPSdecode.h"

u8  NMEA_Comma_Pos (u8 *buf, u8 cx) {
	u8 *p = buf;
	while (cx) {
		if (*buf=='*' || *buf<' ' || *buf>'z') return 0xFF;
		if (*buf==',') {-- cx;} buf ++;
	} return buf - p;
}

u32 NMEA_Pow (u32 a, u8 n) {
	u32 ret = 1;
	while (n) {
		if(n&1) ret *= a;
		a *= a, n >>= 1;
	} return ret;
}

int NMEA_StrToNum (u8 *buf, u8*dx) {
	u8  *p = buf;
	u32 ires = 0, fres = 0;
	u8  ilen = 0, flen = 0, flag = 0, i;
	int ret;

	while (1) {
		if (*p=='-') {flag |= 0x02; p ++;}
		if (*p==',' || *p=='*') {break;}
		if (*p=='.') {flag |= 0x01; p ++;}
		else if (*p<'0' || *p>'9') {
			ilen = 0; flen = 0; break;
		} if (flag&0x01) {++ flen;}
		else {++ ilen;} p ++;
	} if (flag&0x02) {buf ++;}

	for (i=0; i<ilen; ++ i) {
		ires += NMEA_Pow(10, ilen-i-1)*(buf[i]^'0');
	} if (flen>5) {flen = 5;} *dx = flen;

	for (i=0; i<flen; ++ i) {
		fres += NMEA_Pow(10, flen-i-1)*(buf[ilen+i+1]^'0');
	} ret = ires * NMEA_Pow(10, flen) + fres;
	return (flag&0x02)? -ret:ret;
}

void NMEA_GPGSV_Analysis (nmea_msg *gpsx, u8 *buf) {
	u8 *p, *p1, dx;
	u8 len, i, j, slx = 0;
	u8 posx; p = buf;

	p1 = (u8*)strstr((const char *)p, "$GPGSV");
	len = p1[7] ^ '0';
	posx = NMEA_Comma_Pos(p1, 3);

	if (posx^0XFF) gpsx->svnum = NMEA_StrToNum(p1+posx, &dx);
	for (i=0; i<len; ++ i) {
		p1 = (u8*)strstr((const char *)p, "$GPGSV");
		for (j=0; j<4; ++ j) {
			posx = NMEA_Comma_Pos(p1, 4+j*4);

			if (posx^0XFF) gpsx->slmsg[slx].num = NMEA_StrToNum(p1+posx, &dx);
			else break;

			posx = NMEA_Comma_Pos(p1, 5+j*4);

			if (posx^0XFF) gpsx->slmsg[slx].eledeg = NMEA_StrToNum(p1+posx, &dx);
			else break;

			posx = NMEA_Comma_Pos(p1, 6+j*4);

			if (posx^0XFF) gpsx->slmsg[slx].azideg = NMEA_StrToNum(p1+posx, &dx);
			else break;

			posx = NMEA_Comma_Pos(p1, 7+j*4);

			if (posx^0XFF) gpsx->slmsg[slx].sn = NMEA_StrToNum(p1+posx, &dx);
			else break;

			++ slx;
		} p = p1+1;
	}
}

void NMEA_GPVTG_Analysis(nmea_msg *gpsx,u8 *buf) {
	u8 *p1, dx; u8 posx;
	p1 = (u8*)strstr((const char *)buf, "$GPVTG");
	posx = NMEA_Comma_Pos(p1, 7);
	if(posx^0XFF) {
		gpsx->speed = NMEA_StrToNum(p1+posx, &dx);
		if (dx < 3) gpsx->speed *= NMEA_Pow(10, 3-dx);
	}
}

void NMEA_GPGGA_Analysis (nmea_msg *gpsx,u8 *buf) {
	u8 *p1, dx;
	u8 posx;
	p1 = (u8*)strstr((const char *)buf, "$GPGGA");
	posx = NMEA_Comma_Pos(p1, 6);
	if (posx^0XFF) gpsx->gpssta = NMEA_StrToNum(p1+posx, &dx);
//	posx = NMEA_Comma_Pos(p1, 7);
//	if (posx^0XFF) gpsx->posslnum = NMEA_StrToNum(p1+posx, &dx);
//	posx = NMEA_Comma_Pos(p1, 9);
//	if (posx^0XFF) gpsx->altitude = NMEA_StrToNum(p1+posx, &dx);
}

void NMEA_GPGSA_Analysis (nmea_msg *gpsx, u8 *buf) {
	u8 *p1, dx;
	u8 posx;
	p1 = (u8*)strstr((const char *)buf, "$GPGSA");
//	posx = NMEA_Comma_Pos(p1, 2); u8 i;
//	if (posx^0XFF) gpsx->fixmode = NMEA_StrToNum(p1+posx, &dx);
//	for (i=0; i<12; ++ i) {
//		posx = NMEA_Comma_Pos(p1, 3+i);
//		if (posx^0XFF) gpsx->possl[i] = NMEA_StrToNum(p1+posx, &dx);
//		else break;
//	}
	posx = NMEA_Comma_Pos(p1, 15);
	if (posx^0XFF) gpsx->pdop = NMEA_StrToNum(p1+posx, &dx);
//	posx = NMEA_Comma_Pos(p1, 16);
//	if (posx^0XFF) gpsx->hdop = NMEA_StrToNum(p1+posx, &dx);
//	posx = NMEA_Comma_Pos(p1, 17);
//	if (posx^0XFF) gpsx->vdop = NMEA_StrToNum(p1+posx, &dx);
}

void NMEA_GPRMC_Analysis (nmea_msg *gpsx, u8 *buf) {
	u8 *p1, dx;
	u8 posx;
	u32 temp;
	float rs;
	p1 = (u8*)strstr((const char *)buf, "$GPRMC");
//	posx = NMEA_Comma_Pos(p1, 1);
//	if (posx^0XFF) {
//		temp = NMEA_StrToNum(p1+posx, &dx) / NMEA_Pow(10, dx);
//		gpsx->utc.hour = temp/10000;
//		gpsx->utc.min  = (temp/100)%100;
//		gpsx->utc.sec  = temp%100;
//	}
	posx = NMEA_Comma_Pos(p1, 3);
	if (posx^0XFF) {
		temp = NMEA_StrToNum(p1+posx, &dx);
		gpsx->latitude = temp/NMEA_Pow(10, dx+2);
		rs = temp%NMEA_Pow(10, dx+2);
		gpsx->latitude = gpsx->latitude*100000+(rs*NMEA_Pow(10, 5-dx))/60;
	}
	posx = NMEA_Comma_Pos(p1, 4);
	if (posx^0XFF) gpsx->nshemi = *(p1+posx);
	if (gpsx->nshemi ^ 'N') gpsx->latitude *= -1;
	posx = NMEA_Comma_Pos(p1,5);
	if (posx^0XFF) {
		temp = NMEA_StrToNum(p1+posx, &dx);
		gpsx->longitude = temp/NMEA_Pow(10, dx+2);
		rs = temp%NMEA_Pow(10, dx+2);
		gpsx->longitude = gpsx->longitude*100000+(rs*NMEA_Pow(10, 5-dx))/60;
	}
	posx = NMEA_Comma_Pos(p1, 6);
	if (posx^0XFF) gpsx->ewhemi = *(p1+posx);
	if (gpsx->ewhemi ^ 'E') gpsx->longitude *= -1;
//	posx = NMEA_Comma_Pos(p1, 9);
//	if (posx^0XFF) {
//		temp = NMEA_StrToNum(p1+posx, &dx);
//		gpsx->utc.date  = temp/10000;
//		gpsx->utc.month = (temp/100)%100;
//		gpsx->utc.year  = 2000+temp%100;
//	}
}

