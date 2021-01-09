
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "GPSdecode.h"

/**
 * NMEA_Comma_Pos
 * function: get the position of the number count x "," in gps_buff.
 */

uint8_t NMEA_Comma_Pos (uint8_t *buf, uint8_t cx) {
	uint8_t *p = buf;
    while(cx) {
        if(*buf=='*' || *buf<' ' || *buf>'z') return 0xFF;
        if(*buf == ',') {-- cx;} ++ buf;
    } return buf - p;
}

/**
 * NMEA_Pow
 * function: get "pow(m, n);"
 */

uint32_t NMEA_Pow (uint8_t m, uint8_t n) {
	uint32_t res = 1;
	while (n) {
		if (n&1) res *= m;
		m *= m; n >>= 1;
	} return res;
}

/**
 * NMEA_STR2num
 * function: turn str to numbers, end with "," or "*"
 * input: "dx" is the position of the dot of the number
 */

int NMEA_Str2num (uint8_t *buf, uint8_t *dx) {
	uint8_t *p = buf;
	uint32_t ires = 0, fres = 0;
    uint8_t  ilen = 0, flen = 0, i, mask = 0;
    int res;
    while (1) {
    	if (*p=='-') {mask |= 0x02; ++ p;}
    	if (*p==','||*p=='*') break;
    	if (*p=='.') {mask |= 0x01; ++ p;}
    	else if((*p>'9') || (*p<'0')) {ilen = flen = 0; break;}
        if (mask & 0x01) ++ flen;
        else {++ ilen;} ++ p;
    }
    if (mask & 0x02) ++ buf;
    for (i=0; i<ilen; ++ i) {
        ires += NMEA_Pow(10, ilen-i-1) * (buf[i]^'0');
    } if (flen > 5) {flen = 5;} *dx = flen;
    for (i=0; i<flen; ++ i) {
        fres += NMEA_Pow(10, flen-i-1) * (buf[ilen+i+1]^'0');
    } res = ires * NMEA_Pow(10, flen) + fres;
    return (mask&0x02)? -res:res;
}

/**
 * NMEA_BDS_GPRMC_Analysis
 * function: decode NMEA message
 */

void NMEA_BDS_GPRMC_Analysis (GPSmessage *gpsmsg, uint8_t *buf) {
	uint8_t *p4, dx;
	uint8_t posx;
	uint32_t temp;
    float rs;
    p4 = (uint8_t*)strstr((const char *)buf, "$GPRMC");
    posx = NMEA_Comma_Pos (p4, 3);
    if (posx != 0XFF) {
        temp = NMEA_Str2num (p4+posx, &dx);
        gpsmsg->latitude_bd = temp/NMEA_Pow(10, dx+2);
        rs = temp % NMEA_Pow(10, dx+2);
        gpsmsg->latitude_bd = gpsmsg->latitude_bd * NMEA_Pow(10, 5) + (rs * NMEA_Pow(10, 5-dx)) / 60;
    } posx = NMEA_Comma_Pos(p4, 4);
    if (posx != 0XFF) gpsmsg->nshemi_bd = *(p4+posx);
     posx = NMEA_Comma_Pos(p4, 5);
    if (posx != 0XFF) {
        temp = NMEA_Str2num(p4+posx, &dx);
        gpsmsg->longitude_bd = temp / NMEA_Pow(10, dx+2);
        rs = temp % NMEA_Pow(10, dx+2);
        gpsmsg->longitude_bd = gpsmsg->longitude_bd * NMEA_Pow(10, 5) + (rs * NMEA_Pow(10, 5-dx)) / 60;
    } posx = NMEA_Comma_Pos(p4, 6);
    if (posx != 0XFF) gpsmsg->ewhemi_bd = *(p4+posx);
}


