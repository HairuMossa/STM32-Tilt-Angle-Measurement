#include "I2C.h"

int checking=0;

// pb8 scl
// pb9 sda


void I2C1_burstWrite(char saddr, char maddr, int n, char* data ){


	volatile int tmp;


	while(I2C1->SR2 & (SR2_BUSY)){checking = -1;} // check if the bus is free

	I2C1->CR1 |= CR1_START; // generate a start

	while(!(I2C1->SR1 & (SR1_SB))){checking = 1;} // wait until start flag is set

	I2C1->DR = saddr << 1; // transmit slave address

	while(!(I2C1->SR1 & (SR1_ADDR))){checking = 2;} // wait until address flag is set

	tmp = I2C1->SR2; // clear address flag

	while(!(I2C1->SR1 & (SR1_TXE))){checking = 3;} // wait until data register is empty

	I2C1->DR = maddr; // send memory address

	for (int i=0; i<n; i++){


		// wait until data register is empty
		while(!(I2C1->SR1 & (SR1_TXE))){checking = 4;}

		// transmit data to memory address
		I2C1->DR = *data++;
	}
	// wait until transfer is complete
	while(!(I2C1->SR1 & (SR1_BTF))){checking = 5;}

	I2C1->CR1 |= CR1_STOP; // generate a stop
}


void I2C1_burstRead(char saddr , char maddr, int n, char* data){

	volatile int tmp;

	while(I2C1->SR2 & (SR2_BUSY)){}

	I2C1->CR1 |= CR1_START;

	while(!(I2C1->SR1 & (SR1_SB))){}

	I2C1->DR = saddr << 1;

	while(!(I2C1->SR1 & (SR1_ADDR))){}

	tmp = I2C1->SR2;

	while(!(I2C1->SR1 & (SR1_TXE))){}

	I2C1->CR1 |= maddr;

	while(!(I2C1->SR1 & (SR1_TXE))){}

	I2C1->CR1 |= CR1_START;

	while(!(I2C1->SR1 & (SR1_SB))){}

	I2C1->DR = saddr << 1 | 1;

	while(!(I2C1->SR1 & (SR1_ADDR))){}

	tmp = I2C1->SR2;

	I2C1->CR1 |= CR1_ACK;

	while(n > 0U){


		if (n == 1U){
			I2C1->CR1 &= ~CR1_ACK;

			I2C1->CR1 |= CR1_STOP;

			while(!(I2C1->SR1 & SR1_RXNE)){}

			*data++ = I2C1->DR;
			break;
		}

		else {
			while(!(I2C1->SR1 & SR1_RXNE)){}

			(*data++) = I2C1->DR;

			n--;
		}
	}

}


void I2C1_byteRead(char saddr, char maddr, char* data){

	volatile int tmp;

	while(I2C1->SR2 & (SR2_BUSY)){}

	I2C1->CR1 |= CR1_START;

	while(!(I2C1->SR1 & (SR1_SB))){}

	I2C1->DR = saddr << 1;

	while(!(I2C1->SR1 & (SR1_ADDR))){}

	tmp = I2C1->SR2;

	I2C1->DR = maddr;

	while(!(I2C1->SR1 & (SR1_TXE))){}

	I2C1 ->CR1 |= CR1_START;

	while(!(I2C1->SR1 & (SR1_SB))){}

//	I2C1->CR1 |= CR1_START;
//
//	while(!(I2C1->SR1 & (SR1_SB))){}

	I2C1->DR = saddr << 1 | 1;

	while(!(I2C1->SR1 & (SR1_ADDR))){}

	I2C1->CR1 &= ~CR1_ACK;

	tmp = I2C1->SR2;

	I2C1->CR1 |= CR1_STOP;

	while(!(I2C1->SR1 & (SR1_RXNE))){}

	*data++ = I2C1->DR;



}







void I2C1_init(void){

	// enable clock access to gpio
	RCC->AHB1ENR |= (GPIOBEN);

	// configure the pins to Alternate function mode
    GPIOB->AFR[1] &= ~0x000000FF;
    GPIOB->AFR[1] |= 0x00000044;
    GPIOB->MODER &= ~0x000F0000;
    GPIOB->MODER |= 0x000A0000;

	// set pins to open drain
	GPIOB->OTYPER |= (1U<<8);
	GPIOB->OTYPER |= (1U<<9);

	// enable pull-up
	GPIOB->PUPDR |=(1U<<16);
	GPIOB->PUPDR &=~(1U<<17);

	GPIOB->PUPDR |=(1U<<18);
	GPIOB->PUPDR &=~(1U<<19);

	// enable clock access to the i2c
	RCC->APB1ENR |= (I2C1EN);

	// enter reset mode
	I2C1->CR1 |= (1U<<15);

	// exit out of reset mode
	I2C1->CR1 &=~(1U<<15);


	// set peripheral clock frequency
	I2C1->CR2 = (1U<<4);  // 16Mhz

	/// SET I2C TO STANDARD MODE , 100KHZ CLOCK
	I2C1->CCR = I2C_100KHZ;


	// SET RISE TIME
	I2C1->TRISE = SD_MODE_MAX_RISE_TIME;

	// ENABLE I2C1 MODULE
	I2C1->CR1 |= CR1_PE;
}
