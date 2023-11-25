#include "stm32f4xx.h"

/*
 * I2C.h
 *
 *  Created on: Nov 5, 2023
 *      Author: Hairu Mossa
 */



#ifndef I2C_H_
#define I2C_H_

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


void I2C1_init(void);
void I2C1_byteRead(char saddr, char maddr, char* data);
void I2C1_burstRead(char saddr , char maddr, int n, char* data);
void I2C1_burstWrite(char saddr, char maddr, int n, char* data );

#endif /* I2C_H_ */
