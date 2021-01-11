
#include "MPU6050/MPU6050.h"

u8 MPU_Init(void) {
	u8 res;
	extern I2C_HandleTypeDef hi2c1;
	HAL_I2C_Init(&hi2c1);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X80);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X00);
	MPU_Set_Gyro_Fsr(3);
	MPU_Set_Accel_Fsr(0);
	MPU_Set_Rate(50);
	MPU_Write_Byte(MPU_INT_EN_REG, 0X00);
	MPU_Write_Byte(MPU_USER_CTRL_REG, 0X00);
	MPU_Write_Byte(MPU_FIFO_EN_REG, 0X00);
	MPU_Write_Byte(MPU_INTBP_CFG_REG, 0X80);
	res = MPU_Read_Byte(MPU_DEVICE_ID_REG);
	if (res == MPU_ADDR) {
		MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X01);
		MPU_Write_Byte(MPU_PWR_MGMT2_REG, 0X00);
		MPU_Set_Rate(50);
	} else return 1;
	return 0;
}

u8 MPU_Set_Gyro_Fsr(u8 fsr) {
	return MPU_Write_Byte(MPU_GYRO_CFG_REG, fsr<<3);
}

u8 MPU_Set_Accel_Fsr(u8 fsr) {
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG, fsr<<3);
}

u8 MPU_Set_LPF(u16 lpf) {
	u8 data = 0;
	if (lpf >= 188)  data = 1;
	else if(lpf>=98) data = 2;
	else if(lpf>=42) data = 3;
	else if(lpf>=20) data = 4;
	else if(lpf>=10) data = 5;
	else data = 6;
	return MPU_Write_Byte(MPU_CFG_REG, data);
}

u8 MPU_Set_Rate(u16 rate) {
	u8 data;
	if (rate > 1000) rate = 1000;
	if (rate < 4)    rate = 4;
	data = 1000/rate - 1;
	data = MPU_Write_Byte(MPU_SAMPLE_RATE_REG, data);
 	return MPU_Set_LPF(rate/2);
}

float MPU_Get_Temperature(void) {
	unsigned char buf[2];
	short raw;
	float temp;

	MPU_Read_Len(MPU_TEMP_OUTH_REG, 2, buf);
	raw  = (buf[0]<<8) | buf[1];
	temp = (36.53+((double)raw)/340)*100;
	return temp / 100.0f;
}

u8 MPU_Get_Gyroscope(short *gx, short *gy, short *gz) {
	u8 buf[6], res;
	res = MPU_Read_Len(MPU_GYRO_XOUTH_REG, 6, buf);
	if(res==0) {
		*gx = ((u16)buf[0]<<8) | buf[1];
		*gy = ((u16)buf[2]<<8) | buf[3];
		*gz = ((u16)buf[4]<<8) | buf[5];
	} return res;
}

u8 MPU_Get_Accelerometer(short *ax, short *ay, short *az) {
	u8 buf[6], res;
	res = MPU_Read_Len(MPU_ACCEL_XOUTH_REG, 6, buf);
	if(res == 0) {
		*ax = ((u16)buf[0]<<8) | buf[1];
		*ay = ((u16)buf[2]<<8) | buf[3];
		*az = ((u16)buf[4]<<8) | buf[5];
	} return res;
}

u8 MPU_Write_Len(u8 reg, u8 len, u8 *buf) {
	extern I2C_HandleTypeDef hi2c1;
	HAL_I2C_Mem_Write(&hi2c1, MPU_WRITE, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
	HAL_Delay(100); return 0;
}

u8 MPU_Read_Len(u8 reg, u8 len, u8 *buf) {
	extern I2C_HandleTypeDef hi2c1;
	HAL_I2C_Mem_Read  (&hi2c1, MPU_READ, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
	HAL_Delay(100); return 0;
}

u8 MPU_Write_Byte(u8 reg, u8 data) {
	extern I2C_HandleTypeDef hi2c1;
	unsigned char W_Data = data;
	HAL_I2C_Mem_Write(&hi2c1, MPU_WRITE, reg, I2C_MEMADD_SIZE_8BIT, &W_Data, 1, 0xfff);
	HAL_Delay(100); return 0;
}

u8 MPU_Read_Byte(u8 reg) {
	extern I2C_HandleTypeDef hi2c1;
	unsigned char R_Data = 0;
	HAL_I2C_Mem_Read(&hi2c1, MPU_READ, reg, I2C_MEMADD_SIZE_8BIT, &R_Data, 1, 0xfff);
	HAL_Delay(100); return R_Data;
}
