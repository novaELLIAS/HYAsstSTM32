#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <MPU6050.h>
#include "I2Cdev.h"

/*
 *  Code by Ellias Kiri Stuart
 *  2020/01/06
*/

#define ACCIDENT_ACCE 2 //minnimal acceleration to trigger accident report
#define ACCIDENT_ANGLE 90 //minnimal dip angle to trigger accident report
#define ACCIDENT_ALERT_SPEED 25

class acceFunc {
#define durVal 200

private:

	MPU6050 accelgyro;

	unsigned long now, lastTime = 0;
	double dt;

	int16_t ax, ay, az, gx, gy, gz;
	double aax=0, aay=0,aaz=0, agx=0, agy=0, agz=0;
	double accx, accy, accz;
	long axo = 0, ayo = 0, azo = 0, gxo = 0, gyo = 0, gzo = 0;

	double pi = 3.1415926, AcceRatio = 16384.0, GyroRatio = 131.0;

	long long n_sample = 8;
	double aaxs[8]={0}, aays[8]={0}, aazs[8]={0};
	long long aax_sum, aay_sum,aaz_sum;

	double a_x[10]={0}, a_y[10]={0}, a_z[10]={0}, g_x[10]={0}, g_y[10]={0}, g_z[10]={0};
	double Px=1, Rx, Kx, Sx, Vx, Qx, Py=1, Ry, Ky, Sy, Vy, Qy, Pz=1, Rz, Kz, Sz, Vz, Qz;

	double tmpAgx[durVal], tmpAgy[durVal], tmpAgz[durVal];
	int pos, totx, toty, totz;
	double avgx, avgy, avgz;

	template <typename T>
	inline T Abs(T x) {return (x)<0? (-x):(x);}

public:

	void accelgyroSetUp ();
	void getAcce ();

	inline bool isRotate (register int Acce) {
  		getAcce(); register bool flag = false;
  
  		flag = Acce>=ACCIDENT_ACCE;
  		if (Abs(agx-avgx)>ACCIDENT_ANGLE || Abs(agy-avgy)>ACCIDENT_ANGLE || Abs(agz-avgz)>ACCIDENT_ANGLE) flag = true;

  		totx -= tmpAgx[pos], toty -= tmpAgy[pos], totz -= tmpAgz[pos];
  		tmpAgx[pos] = agx, tmpAgy[pos]= agy, tmpAgz[pos] = agz;
  		totx += agx, toty += agy, totz += agz;
  		avgx = 1.0*totx/(1.0*durVal), avgy = 1.0*toty/(1.0*durVal), avgz = 1.0*totz/(1.0*durVal);
  		pos = (pos+1)%(durVal);
  		return flag;
	}

#undef durVal
};