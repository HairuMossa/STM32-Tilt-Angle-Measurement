#include "MPU6050.h"
#include "I2C.h"


void readMPU6050Data(int16_t* accelX, int16_t* accelY,
					 int16_t* accelZ, int16_t* gyroX,
					 int16_t* gyroY, int16_t* gyroZ){


	I2C1_init();

    // Read accelerometer values
    char accelData[6];
    I2C1_burstRead(MPU_ADDR_DEFAULT, MPU_ACCEL_X_REG, 6, accelData);

    // Combine high and low bytes to get accelerometer values
    *accelX = (int16_t)((accelData[0] << 8) | accelData[1]);
    *accelY = (int16_t)((accelData[2] << 8) | accelData[3]);
    *accelZ = (int16_t)((accelData[4] << 8) | accelData[5]);

    // Read gyroscope values
    char gyroData[6];
    I2C1_burstRead(MPU_ADDR_DEFAULT, MPU_GYRO_X_REG, 6, gyroData);

    // Combine high and low bytes to get gyroscope values
    *gyroX = (int16_t)((gyroData[0] << 8) | gyroData[1]);
    *gyroY = (int16_t)((gyroData[2] << 8) | gyroData[3]);
    *gyroZ = (int16_t)((gyroData[4] << 8) | gyroData[5]);





}



