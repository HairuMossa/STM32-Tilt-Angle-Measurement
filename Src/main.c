#include "stm32f4xx.h"
#include "I2C.h"
#include "MPU6050.h"

uint8_t data_record[2];

int16_t x_acc=0;

void mpu6050_read(void){
	uint8_t data[2];

	I2C1_burstRead(MPU_ADDR_DEFAULT , 59, 2, (char*) data);
	x_acc = (int16_t)(data[0]<< 8 | data[1]);

}

int plz=0;
int main(void)
{

	mpu_init();
	while(1){
		plz++;
		mpu6050_read();
		for(int i=0; i<1000; i++){

		}


	}
}
