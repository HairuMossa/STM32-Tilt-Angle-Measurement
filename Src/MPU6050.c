#include "MPU6050.h"
#include "I2C.h"

void mpu_init(void){

	// initialize I2C
	I2C1_init();
	// configure the acceleration to 4g mode
	char dataToSend = 0b00001000;
	I2C1_burstWrite(MPU_ADDR_DEFAULT, MPU_ACCEL_CONFIG_REG, 1, &dataToSend );
	// write_mpu(MPU_ACCEL_CONFIG_REG, 0b00001000);

	// configure the GYRO to ± 500 °/s mode

	I2C1_burstWrite(MPU_ADDR_DEFAULT, MPU_GYRO_CONFIG_REG, 1, &dataToSend );
	// configuring the power register to exit sleep mode
			//and enable the temperature sensor
	dataToSend = 0x00; // Clear SLEEP bit to wake up the MPU6050
	I2C1_burstWrite(MPU_ADDR_DEFAULT, MPU_PWR_MGMT_1_REG, 1, &dataToSend);

}


void readMPU6050Data(int16_t* accelX, int16_t* accelY,
					 int16_t* accelZ, int16_t* gyroX,
					 int16_t* gyroY, int16_t* gyroZ, int16_t* temp){
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

    // measure temperature
    char temperature[2];
    I2C1_burstRead(MPU_ADDR_DEFAULT, MPU_TEMP_REG, 2, temperature);

    // combine the high and low bytes
    int16_t raw_Temp = (int16_t)((temperature[0] << 8) | temperature[1]);

    // convert to degree C by using the formula in MPU6050 data sheet
    *temp = raw_Temp / 340.0 + 36.53;
}
