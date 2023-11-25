#include "I2C.h"


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
	I2C1->DR = maddr;

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
