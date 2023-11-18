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
char data;

uint8_t data_rec[6];

void read_mpu(uint8_t reg){
	I2C1_byteRead(MPU_ADDR_DEFAULT, reg, &data);

}

void write_mpu(uint8_t reg, char value){
	char data[1];
	data[0] = value;
	I2C1_burstWrite(MPU_ADDR_DEFAULT, reg, 1 , data );
}

void read_mpu_multiple(uint8_t reg){

	I2C1_burstRead(MPU_ADDR_DEFAULT , reg, 6, (char*) data_rec);
}

void mpu_init(void){

	I2C1_init();
	// configure the acceleration to 4g mode
	write_mpu(0x1C, 0b00001000);

	// configuring the power register to exit sleep mode
			//and disabling the temperature sensor
	write_mpu(0x6B, 0b00000000);


}


//void mpu_init(void){
//	// read the device, ID
//    read_mpu(MPU_WHO_AM_I_REG);
//
//    if (data == MPU6050_WHO_AM_I) {
//        // MPU6050 is responding correctly
//
//        // Configure gyroscope and accelerometer range
//
//        // Perform sensor calibration if needed
//        calibrate_mpu();
//
//        // Optional: Configure additional settings or features
//
//        // Sensor is now initialized
//    } else {
//        // MPU6050 is not responding correctly, handle the error
//    }
//
//}


void calibrate_mpu(void){
    // Perform calibration by reading and averaging sensor values over a number of samples
    int16_t accel_x = 0, accel_y = 0, accel_z = 0;

    for (int i = 0; i < NUM_CALIBRATIONS; ++i) {
        read_mpu_multiple(MPU_ACCEL_X_REG);

        // Accumulate sensor values
        accel_x += (int16_t)((data_rec[0] << 8) | data_rec[1]);
        accel_y += (int16_t)((data_rec[2] << 8) | data_rec[3]);
        accel_z += (int16_t)((data_rec[4] << 8) | data_rec[5]);

        // Optional: Add delay between readings if necessary
    }

    // Calculate the average values
    accel_x /= NUM_CALIBRATIONS;
    accel_y /= NUM_CALIBRATIONS;
    accel_z /= NUM_CALIBRATIONS;

    // Use these averages to apply calibration offsets if needed
    // Write calibration offsets to corresponding registers
    // Example:
    // write_mpu(MPU_ACCEL_X_OFFSET_H, (char)(accel_x >> 8));
    // write_mpu(MPU_ACCEL_X_OFFSET_L, (char)(accel_x & 0xFF));
    // Repeat for accel_y, accel_z, gyro_x, gyro_y, gyro_z
}




