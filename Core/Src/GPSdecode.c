
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "GPSdecode.h"

/**
 * NMEA_Comma_Pos
 * function: get the position of the number count x "," in gps_buff.
 */

int NMEA_Comma_Pos (int *buf,uint8_t cx) {
    int *p = buf;
    while(cx) {
        if(*buf=='*' || *buf<' ' || *buf>'z') return 0xFF;
        if(*buf == ',') {-- cx;} ++ buf;
    } return buf - p;
}

/**
 * NMEA_Pow
 * function: get "pow(m, n);"
 */

int NMEA_Pow (int m, int n) {
    int res = 1;
    while (-- n) res *= m;
    return res;
}

/**
 * NMEA_STR2num
 * function: turn str to numbers, end with "," or "*"
 * input: "dx" is the position of the dot of the number
 */

int NMEA_Str2num (int *buf, int *dx)
{
    int *p = buf;
    int ires = 0,fres = 0;
    int ilen = 0,flen = 0,i;
    int mask = 0, res;
    while (1) {
    	if (*p=='-') {mask |= 0x02; ++ p;}
    	if (*p==','||*p=='*') break;
    	if (*p=='.') {mask |= 0x01; ++ p;}
    	else if(*p>'9' || (*p<'0')) {ilen = flen = 0; break;}
        if (mask & 0x01) ++ flen;
        else {++ ilen;} ++ p;
    }
    if (mask & 0x02) ++ buf;
    for (i=0; i<ilen; ++ i) {
        ires += NMEA_Pow(10, ilen-1-i) * (buf[i]^'0');
    } if (flen > 5) flen = 5; *dx = flen;
    for (i=0; i<flen; ++ i) {
        fres += NMEA_Pow(10, flen-1-i) * (buf[ilen+1+i]^'0');
    }
    res = ires * NMEA_Pow(10, flen) + fres;
    if(mask & 0x02) res = -res;
    return res;
}

/**
 * NMEA_BDS_GPRMC_Analysis
 * function: decode NMEA message
 */

void NMEA_BDS_GPRMC_Analysis (GPSmessage *gpsmsg, int *buf) {
    int *p4,dx;
    int posx, temp;
    float rs;
    p4 = (int*)strstr((const char *)buf,"$GPRMC");
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


