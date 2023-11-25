#include "stm32f4xx.h"
#include "I2C.h"
#include "MPU6050.h"

/*
PB9 -  I2C1_SDA
PB8 -  I2C1_SCL
*/
void delayMs(int n);

int16_t Ax, Ay, Az, Gx, Gy, Gz, temperature;



int main(void){


    mpu_init(); // initialize MPU6050
    while(1){
        readMPU6050Data(&Ax, &Ay, &Az, &Gx, &Gy, &Gz, &temperature); // read values
        delayMs(50); // delay
    }

}

void delayMs(int n) {
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++) ; // Delay loop
}
