#include "stm32f4xx.h"
#include "I2C.h"
#include "MPU6050.h"


int main(void)
{

	while(1){
	    int16_t accelX, accelY, accelZ, gyroX, gyroY, gyroZ;
	    readMPU6050Data(&accelX, &accelY, &accelZ, &gyroX, &gyroY, &gyroZ);



	}
}
