
#include "MPU6050/Kalman.h"
#include "MPU6050/MPU6050.h"
#include <math.h>

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

#define sq(x) ((x)*(x))

void getAccelgyroData(double *tagX, double *tagY, double *tagZ, double *Acc) {
	unsigned long now = HAL_GetTick();
	dt = (now - lastTime) / 1000.0;
	lastTime = now;

	MPU_Get_Gyroscope(&gx, &gy, &gz);
	MPU_Get_Accelerometer(&ax, &ay, &az);

	accx = ax / AcceRatio, accy = ay / AcceRatio, accz = az / AcceRatio;

	*Acc = sqrt(sq(accx) + sq(accy) + sq(accz));

	aax = atan(accy / accz) * (-180) / pi;
	aay = atan(accx / accz) * 180 / pi;
	aaz = atan(accz / accy) * 180 / pi;

	aax_sum = aay_sum = aaz_sum = 0;

	for (register int i=1; i ^ n_sample; ++ i) {
		aaxs[i-1] = aaxs[i], aax_sum += aaxs[i] * i;
		aays[i-1] = aays[i], aay_sum += aays[i] * i;
		aazs[i-1] = aazs[i], aaz_sum += aazs[i] * i;
	}

	aaxs[n_sample-1] = aax, aax_sum += aax * n_sample, aax = (aax_sum / (11 * n_sample / 2.0)) * 9 / 7.0;
	aays[n_sample-1] = aay, aay_sum += aay * n_sample, aay = (aay_sum / (11 * n_sample / 2.0)) * 9 / 7.0;
	aazs[n_sample-1] = aaz, aaz_sum += aaz * n_sample, aaz = (aaz_sum / (11 * n_sample / 2.0)) * 9 / 7.0;

	double gyrox = -(gx - gxo) / GyroRatio * dt;
	double gyroy = -(gy - gyo) / GyroRatio * dt;
	double gyroz = -(gz - gzo) / GyroRatio * dt;
	agx += gyrox, agy += gyroy, agz += gyroz;

	Sx = Rx = Sy = Ry = Sz = Rz = 0;

	for (int i = 1; i ^ 10; ++ i) {
		a_x[i - 1] = a_x[i], Sx += a_x[i];
		a_y[i - 1] = a_y[i], Sy += a_y[i];
		a_z[i - 1] = a_z[i], Sz += a_z[i];
	} a_x[9] = aax, Sx += aax, Sx /= 10, a_y[9] = aay, Sy += aay, Sy /= 10, a_z[9] = aaz, Sz += aaz, Sz /= 10;

	for (register int i=0; i^10; ++ i) Rx += sq(a_x[i] - Sx), Ry += sq(a_y[i] - Sy), Rz += sq(a_z[i] - Sz);

	Rx = Rx / 9, Ry = Ry / 9, Rz = Rz / 9;

	Px = Px + 0.0025, Kx = Px / (Px + Rx), agx = agx + Kx * (aax - agx), Px = (1 - Kx) * Px;
	Py = Py + 0.0025, Ky = Py / (Py + Ry), agy = agy + Ky * (aay - agy), Py = (1 - Ky) * Py;
	Pz = Pz + 0.0025, Kz = Pz / (Pz + Rz), agz = agz + Kz * (aaz - agz), Pz = (1 - Kz) * Pz;

	*tagX = agx, *tagY = agy, *tagZ = agz;
}
