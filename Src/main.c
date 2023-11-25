#include "stm32f4xx.h"
/*
PB9 -  I2C1_SDA
PB8 -  I2C1_SCL

// the alternate function mapping is AF04

*/

#define GPIOBEN                             (1U<<1)
#define I2C1EN                              (1U<<21)
#define I2C_100KHZ                          80
#define SD_MODE_MAX_RISE_TIME               17
#define CR1_PE					            (1U<<0)
#define SR2_BUSY 		                    (1U<<1)
#define CR1_START 		                    (1U<<8)
#define SR1_SB 			                    (1U<<0)
#define SR1_ADDR 		                    (1U<<1)
#define SR1_TXE 		                    (1U<<7)
#define CR1_ACK 		                    (1U<<10)
#define CR1_STOP 		                    (1U<<9)
#define SR1_RXNE 		                    (1U<<6)
#define SR1_BTF			                    (1U<<2)



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




void I2C1_init(void){

	// enable clock access to GPIOB
	RCC->AHB1ENR |= (GPIOBEN);

	// configure the pins to Alternate function mode ,  AF04
    GPIOB->MODER &= ~(0x3 << (2 * 8)); // Clear current setting for PB8
    GPIOB->MODER |= (0b10<<16);  // Set PB8 to alternate function mode

    GPIOB->MODER &= ~(0x3 << (2 * 9)); // Clear current setting for PB9
    GPIOB->MODER |= (0b10<<18); // Set PB9 to alternate function mode

    GPIOB->AFR[1] &= ~(0xF << (4 * 0)); // Clear current setting for PB8
    GPIOB->AFR[1] |= (0b0100<<0); // Set PB8 to AF4

    GPIOB->AFR[1] &= ~(0xF << (4 * 1)); // Clear current setting for PB9
    GPIOB->AFR[1] |= (0b0100<<4); // Set PB9 to AF4


	// set pins to open drain
	GPIOB->OTYPER |= (1U<<8);
	GPIOB->OTYPER |= (1U<<9);

	// enable pull-up
	GPIOB->PUPDR |=(1U<<16);
	GPIOB->PUPDR &=~(1U<<17);

	GPIOB->PUPDR |=(1U<<18);
	GPIOB->PUPDR &=~(1U<<19);

	// enable clock access to the I2C1
	RCC->APB1ENR |= (I2C1EN);

	// enter reset mode
	I2C1->CR1 |= (1U<<15);

	// exit out of reset mode
	I2C1->CR1 &=~(1U<<15);


	// set peripheral clock frequency
	// I2C1->CR2 = (1U<<4);  // 16Mhz
    I2C1->CR2 = 0x0010; /* peripheral clock is 16 MHz*/

	/// SET I2C TO STANDARD MODE , 100KHZ CLOCK
	I2C1->CCR = I2C_100KHZ;


	// SET RISE TIME
	I2C1->TRISE = SD_MODE_MAX_RISE_TIME;

	// ENABLE I2C1 MODULE
	I2C1->CR1 |= 0x0001; /* enable I2C1 */

}

void I2C1_burstWrite(char saddr, char maddr, int n, char* data ){


	volatile int tmp;


	while(I2C1->SR2 & (SR2_BUSY)); // check if the bus is free

	I2C1->CR1 |= CR1_START; // generate a start

	while(!(I2C1->SR1 & (SR1_SB))); // wait until start flag is set

	I2C1->DR = saddr << 1; // transmit slave address

	while(!(I2C1->SR1 & (SR1_ADDR))); // wait until address flag is set

	tmp = I2C1->SR2; // clear address flag

	while(!(I2C1->SR1 & (SR1_TXE))); // wait until data register is empty

	I2C1->DR = maddr; // send memory address

	for (int i=0; i<n; i++){


		// wait until data register is empty
		while(!(I2C1->SR1 & (SR1_TXE)));

		// transmit data to memory address
		I2C1->DR = *data++;
	}
	// wait until transfer is complete
	while(!(I2C1->SR1 & (SR1_BTF)));

	I2C1->CR1 |= CR1_STOP; // generate a stop
}



void I2C1_burstRead(char saddr , char maddr, int n, char* data){

	volatile int tmp;

    // wait until bus is not busy
	while(I2C1->SR2 & (SR2_BUSY));

    // generate a start
	I2C1->CR1 |= CR1_START;

    // wait until start flag is set
	while(!(I2C1->SR1 & (SR1_SB)));

    // transmit slave address+ write
	I2C1->DR = saddr << 1;

    // wait until addr flag is set
	while(!(I2C1->SR1 & (SR1_ADDR)));

    // clear flag
	tmp = I2C1->SR2;

    // wait until transmitter empty
	while(!(I2C1->SR1 & (SR1_TXE)));

    // send memory address
	I2C1->CR1 = maddr;

    // wait until transmitter empty
	while(!(I2C1->SR1 & (SR1_TXE)));

    // generate restrart
	I2C1->CR1 |= CR1_START;

    // wait start flag is set
	while(!(I2C1->SR1 & (SR1_SB)));

    // transmit slave address  + read
	I2C1->DR = saddr << 1 | 1;

    // wait until addr flag is set
	while(!(I2C1->SR1 & (SR1_ADDR)));

    // clear addr flag
	tmp = I2C1->SR2;

    // enable acknowledge
	I2C1->CR1 |= CR1_ACK;

	while(n > 0U){


		if (n == 1U){

            // disable acknowledge
			I2C1->CR1 &= ~CR1_ACK;

            // generate stop
			I2C1->CR1 |= CR1_STOP;

            // wait for rxne flag set
			while(!(I2C1->SR1 & SR1_RXNE));

            // read data from DR
			*data++ = I2C1->DR;
			break;
		}

		else {
            // wait until rxne flag is set
			while(!(I2C1->SR1 & SR1_RXNE));

            // read data from dr
			(*data++) = I2C1->DR;

			n--;
		}
	}

}


void mpu_init(void){

	// initialize I2C
	I2C1_init();
	// configure the acceleration to 4g mode
	I2C1_burstWrite(MPU_ADDR_DEFAULT, MPU_ACCEL_CONFIG_REG, 1, 0b00001000 );
	// write_mpu(MPU_ACCEL_CONFIG_REG, 0b00001000);

	// configure the GYRO to ± 500 °/s mode

	I2C1_burstWrite(MPU_ADDR_DEFAULT, MPU_GYRO_CONFIG_REG, 1, 0b00001000 );
	// configuring the power register to exit sleep mode
			//and enable the temperature sensor
	I2C1_burstWrite(MPU_ADDR_DEFAULT, 0x6B, 1, 0b00001000 );

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
    *temp = (int16_t)((temperature[0] << 8) | temperature[1]);
}

int16_t Ax, Ay, Az, Gx, Gy, Gz, temperature;

void delayMs(int n) {
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++) ; // Delay loop
}

int main(void){


    mpu_init();
    while(1){
        readMPU6050Data(&Ax, &Ay, &Az, &Gx, &Gy, &Gz, &temperature);
        delayMs(500);
    }

}
