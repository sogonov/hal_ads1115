/*
 * ads1115.h
 *
 *  Created on: 4 апр. 2023 г.
 *      Author: sogonove
 */
#include "stm32f4xx_hal.h"


#ifndef SRC_ADS1115_H_
#define SRC_ADS1115_H_


#define slaveAddress 0x48

#define CONV_REG  0x0
#define CONF_REG  0x1
#define Lo_thresh  0x2
#define Hi_thresh  0x3

//Config Register
//OS
#define W_NE  0x0
#define W_SC  0x1
#define R_CC  0x0
#define R_NCC  0x1

//MUX
#define AIN0_AIN1  0b0
#define	AIN0_AIN3  0b1
#define AIN1_AIN3  0b10
#define AIN2_AIN3  0b11

#define AIN0_AIN_GND  0b100
#define AIN1_AIN_GND  0b101
#define AIN2_AIN_GND  0b110
#define AIN3_AIN_GND  0b111

//Programmable gain amplifier configuration
// PGA
#define FS6144  0b0
#define FS4096  0b1
#define FS2048  0b10
#define FS1024  0b11
#define FS0512  0b100
#define FS0256_0  0b101
#define FS0256_1  0b110
#define FS0256_2  0b111



//MODE

#define CCM  0b0 //: Continuous-conversion mode
#define SSPDM  0b1 //Single-shot mode or power-down state


//DR
// Data rate

#define SPS8  0b0
#define SPS16  0b1
#define SPS32  0b10
#define SPS64  0b11
#define SPS128  0b100
#define SPS250  0b101
#define SPS475  0b110
#define SPS860  0b111

//Comparator mode
#define TC  0b0
#define WC  0b1


// Comparator polarity
#define AL  0b0
#define AH  0b1

// Latching comparator
#define NLC  0b0
#define LC  0b1


//Comparator queue and disable
#define AAOC  0b0
#define AATC  0b1
#define AAFC  0b10
#define DC  0b11

// channel
#define Channel_0  AIN0_AIN_GND
#define Channel_1  AIN1_AIN_GND
#define Channel_2  AIN2_AIN_GND 
#define Channel_3  AIN3_AIN_GND 
#define Differential_0_1  AIN0_AIN1
#define Differential_0_3  AIN0_AIN3 
#define Differential_1_3  AIN1_AIN3 
#define Differential_2_3  AIN2_AIN3



void setting_ADS1115();












#endif /* SRC_ADS1115_H_ */
