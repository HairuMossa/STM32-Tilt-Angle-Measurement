#include "stm32f4xx.h"
/*
 * MPU6050.h
 *
 *  Created on: Nov 5, 2023
 *      Author: USER
 */

#ifndef MPU6050_H_
#define MPU6050_H_


#define MPU_ADDR_DEFAULT 0x68
#define MPU_ADDR_SCND 0x69

#define MPU_ACCEL_X_REG 		0x3B
#define MPU_ACCEL_Y_REG 		0x3D
#define MPU_ACCEL_Z_REG 		0x3F
#define MPU_GYRO_X_REG 			0x43
#define MPU_GYRO_Y_REG 			0x45
#define MPU_GYRO_Z_REG 			0x47
#define MPU_TEMP_REG 			0x41
#define MPU_GYRO_CONFIG_REG		0x1B
#define MPU_ACCEL_CONFIG_REG	0x1C
#define MPU_TEMP_CONFIG_REG

#define NUM_CALIBRATIONS 1000


#endif /* MPU6050_H_ */
