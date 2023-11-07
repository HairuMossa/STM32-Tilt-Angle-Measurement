#include "stm32f4xx.h"
#include "I2C.h"

#define SR2_BUSY 		(1U<<1)
#define CR1_START 		(1U<<8)
#define SR1_SB 			(1U<<0)
#define SR1_ADDR 		(1U<<1)
#define SR1_TXE 		(1U<<7)
#define CR1_ACK 		(1U<<10)
#define CR1_STOP 		(1U<<9)
#define SR1_RXNE 		(1U<<6)
#define SR1_BTF			(1U<<2)




void I2C1_burstWrite(char saddr, char maddr, int n, char* data ){

	volatile int tmp;

	while(I2C1->SR2 & (SR2_BUSY)){}

	I2C1->CR1 |= CR1_START;

	while(!(I2C1->SR1 & (SR1_SB))){}

	I2C1->DR = saddr << 1;

	while(!(I2C1->SR1 & (SR1_ADDR))){}

	tmp = I2C1->SR2;

	while(!(I2C1->SR1 & (SR1_TXE))){}

	I2C1->DR = maddr;

	for (int i=0; i<n; i++){
		while(!(I2C1->SR1 & (SR1_TXE))){}

		I2C1->DR = *data++;
	}
	while(!(I2C1->SR1 & (SR1_BTF))){}

	I2C1->CR1 |= CR1_STOP;
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

	I2C1->CR1 |= CR1_START;

	while(!(I2C1->SR1 & (SR1_SB))){}

	I2C1->DR = saddr << 1 | 1;

	while(!(I2C1->SR1 & (SR1_ADDR))){}

	tmp = I2C1->SR2;

	I2C1->CR1 |= CR1_ACK;

	while(n>0U){


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

	I2C1->DR = saddr << 1 | 1;

	while(!(I2C1->SR1 & (SR1_ADDR))){}

	I2C1->CR1 &= ~CR1_ACK;

	tmp = I2C1->SR2;

	I2C1->CR1 |= CR1_STOP;

	while(!(I2C1->SR1 & (SR1_RXNE))){}

	*data++ = I2C1->DR;



}







void I2C1_init(void){
	// for more detail on the steps see onenote on pc

	// enable clock access
	RCC->AHB1ENR |= (GPIOBEN);

	// set pins to alternate function mode
	GPIOB->MODER &= ~(1U<<16);
	GPIOB->MODER |= (1U<<17);

	GPIOB->MODER &= ~(1U<<18);
	GPIOB->MODER |= (1U<<19);

	// set pins to opendrain
	GPIOB->OTYPER |= (1U<<8);
	GPIOB->OTYPER |= (1U<<9);

	// ENABLE PULL UP
	GPIOB->PUPDR |=(1U<<16);
	GPIOB->PUPDR &=~(1U<<17);

	GPIOB->PUPDR |=(1U<<18);
	GPIOB->PUPDR &=~(1U<<19);

	// ENABLOE CLOCK ACCESS TO I2C1
	RCC->APB1ENR |= (I2C1EN);

	// ENTER RESET MODE
	I2C1->CR1 |= (1U<<15);

	// COME OUT OF RESET MODE
	I2C1->CR1 &=~(1U<<15);


	// SET PERIPHERAL CLOCK FREQUENC
	I2C1->CR2 = (1U<<4);  // 16Mhz

	/// SET I2C TO STANDARD MODE , 100KHZ CLOCK
	I2C1->CCR = I2C_100KHZ;


	// SET RISE TIME
	I2C1->TRISE = SD_MODE_MAX_RISE_TIME;

	// ENABLE I2C1 MODULE
	I2C1->CR1 |= CR1_PE;





}
