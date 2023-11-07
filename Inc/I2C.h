#include "stm32f4xx.h"
/*
 * I2C.h
 *
 *  Created on: Nov 5, 2023
 *      Author: USER
 */



#ifndef I2C_H_
#define I2C_H_
fd
void I2C1_init(void);
void I2C1_byteRead(char saddr, char maddr, char* data);
void I2C1_burstRead(char saddr , char maddr, int n, char* data);
void I2C1_burstWrite(char saddr, char maddr, int n, char* data );

#define GPIOBEN 				(1U<<1)
#define I2C1EN 					(1U<<21)
#define I2C_100KHZ				80
#define SD_MODE_MAX_RISE_TIME 	17
#define CR1_PE					(1U<<0)


#endif /* I2C_H_ */
