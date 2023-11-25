#include "stm32f4xx.h"



/*
 * MPU6050.h
 *
 *  Created on: Nov 5, 2023
 *      Author: Hairu Mossa
 */

#ifndef MPU6050_H_
#define MPU6050_H_

// MPU6050 definitions
#define MPU_ADDR_DEFAULT                    0x68
#define MPU_ADDR_SCND                       0x69
#define MPU6050_WHO_AM_I                    0x68
#define MPU_ACCEL_X_REG 		            0x3B
#define MPU_ACCEL_Y_REG 		            0x3D
#define MPU_ACCEL_Z_REG 		            0x3F
#define MPU_GYRO_X_REG 			            0x43
#define MPU_GYRO_Y_REG 			            0x45
#define MPU_GYRO_Z_REG 			            0x47
#define MPU_TEMP_REG 			            0x41
#define MPU_GYRO_CONFIG_REG		            0x1B
#define MPU_ACCEL_CONFIG_REG	            0x1C
#define MPU_WHO_AM_I_REG 		            0x75
#define NUM_CALIBRATIONS                    1000
#define MPU_PWR_MGMT_1_REG					0x6B

void mpu_init(void);
void readMPU6050Data(int16_t* accelX, int16_t* accelY,
					 int16_t* accelZ, int16_t* gyroX,
					 int16_t* gyroY, int16_t* gyroZ, int16_t* temp);

#endif /* MPU6050_H_ */
