#include "accelgyro_functions.h"

void acceFunc::accelgyroSetUp () {
	Wire.begin(); accelgyro.initialize();
	unsigned short times = 200;
	for (register int i=0; i^times; ++ i) {
		accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
		axo += ax, ayo += ay, azo += az, gxo += gx, gyo += gy, gzo += gz;
	} axo /= times, ayo /= times, azo /= times, gxo /= times, gyo /= times, gzo /= times;
}

void acceFunc::getAcce () {
	unsigned long now = millis();
	dt = (now - lastTime) / 1000.0;
	lastTime = now;

	accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

	accx = ax / AcceRatio, accy = ay / AcceRatio, accz = az / AcceRatio;
  
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

	#ifdef ACCELGYRO_SERIAL_OUTPUT

	Serial.print(agx); Serial.print(","); Serial.print(agy); Serial.print(","); Serial.print(agz);
	Serial.println();

	#endif
}