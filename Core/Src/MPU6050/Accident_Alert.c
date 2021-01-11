
#include "main.h"
#include "MPU6050/Kalman.h"
#include "MPU6050/MPU6050.h"
#include "GPS_Decoder/GPSdecode.h"

#define ACCIDENT_ACCE            2          //Minimal acceleration to trigger accident report
#define ACCIDENT_ANGLE          90       	//Minimal dip angle to trigger accident report
#define ACCIDENT_ALERT_SPEED	25

void accidentMonitorSetup (void) {MPU_Init();}

double Abs (double a) {return a<0? -a:a;}

double agx, agy, agz, acc;

#define durVal 500
double tmpAgx[durVal], tmpAgy[durVal], tmpAgz[durVal];
int pos, totx, toty, totz;
long long timer = 0;
double avgx, avgy, avgz, prespd;

int accidentJudge (gps_data *GPSdata) {
	//timer = HAL_GetTick();
	getAccelgyroData(&agx, &agy, &agz, &acc);
	register int flag = (acc>ACCIDENT_ACCE) && ((GPSdata->speed - prespd)/(double)HAL_GetTick()>ACCIDENT_ACCE);
	timer = HAL_GetTick(); prespd = GPSdata->speed;
	if (Abs(agx-avgx)>ACCIDENT_ANGLE || Abs(agy-avgy)>ACCIDENT_ANGLE || Abs(agz-avgz)>ACCIDENT_ANGLE) flag = 1;
	totx -= tmpAgx[pos], toty -= tmpAgy[pos], totz -= tmpAgz[pos];
	tmpAgx[pos] = agx, tmpAgy[pos]= agy, tmpAgz[pos] = agz;
	totx += agx, toty += agy, totz += agz;
	avgx = (double)totx/(1.0*durVal), avgy = (double)toty/(1.0*durVal), avgz = (double)totz/(1.0*durVal);
	pos = (pos+1)%(durVal);
	return flag;
}

#undef durVal
