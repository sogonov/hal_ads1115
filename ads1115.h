/*
 * ads1115.h
 *
 *  Created on: 4 апр. 2023 г.
 *      Author: sogonove
 */
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <math.h>




#ifndef SRC_ADS1115_H_
#define SRC_ADS1115_H_


#define ADS1115_ADDRESS 0x48
typedef enum{
	CONV_REG = 0x0,
	CONF_REG = 0x1,
	Lo_thresh_REG = 0x2,
	Hi_thresh_REG = 0x3
}reg_t;

//OS
typedef enum {
	W_NE = 0x0,
	W_SC = 0x1,
	R_CC = 0x0,
	R_NCC = 0x1
}OperatingMode_t;

//MUX
typedef enum{
	AIN0_AIN1 = 0x0,
	AIN0_AIN3 = 0x1,
	AIN1_AIN3 = 0x2,
	AIN2_AIN3 = 0x3,
	AIN0_AIN_GND = 0x4,
	AIN1_AIN_GND = 0x5,
	AIN2_AIN_GND = 0x6,
	AIN3_AIN_GND = 0x7
}MultiplexerConfig_t;


//Programmable gain amplifier configuration
typedef enum {
	FS6144 = 0x0,
	FS4096 = 0x1,
	FS2048 = 0x2,
	FS1024 = 0x3,
	FS0512 = 0x4,
	FS0256_0 = 0x5,
	FS0256_1 = 0x6,
	FS0256_2 = 0x7
}PGA_Config_t;

//MODE
typedef enum {
	CCM = 0x0,//: Continuous-conversion mode
	PDSSM = 0x1 //Single-shot mode or power-down state
}MODEconfig_t;
// Data rate
typedef enum {
	SPS8 = 0x0,
	SPS16 = 0x1,
	SPS32 = 0x2,
	SPS64 = 0x3,
	SPS128 = 0x4,
	SPS250 = 0x5,
	SPS475 = 0x6,
	SPS860 = 0x7
}DataRate_t;
//Comparator mode
typedef enum COMP_MODE {
	TCWH = 0x0,
	WC = 0x1
}CompareMode_t;

// Comparator polarity
typedef enum{
	AL = 0x0,
	AH = 0x1
}ComparePolarity_t;

// Latching comparator
typedef enum  {
	NLC = 0x0,
	LC = 0x1
}LatchingMode_t;

//Comparator queue and disable
typedef enum{
	AAOC = 0x0,
	AATC = 0x1,
	AAFC = 0x2,
	DC = 0x3
}QueueComparator_t;


// channel
enum channel {
	Channel_0 = AIN0_AIN_GND,
	Channel_1 = AIN1_AIN_GND,
	Channel_2 = AIN2_AIN_GND,
	Channel_3 = AIN3_AIN_GND,
	Differential_0_1 = AIN0_AIN1,
	Differential_0_3 = AIN0_AIN3,
	Differential_1_3 = AIN1_AIN3,
	Differential_2_3 = AIN2_AIN3
};

typedef struct{
	OperatingMode_t 	operatingMode;
	MultiplexerConfig_t channel;
	PGA_Config_t 		pgaConfig;
	MODEconfig_t 		modeConfig;
	DataRate_t			dataRate;
	CompareMode_t		compareMode;
	ComparePolarity_t	polarityMode;
	LatchingMode_t		latchingMode;
	QueueComparator_t	queueComparator;
}ADS1115_Config_t;

typedef struct{
	uint16_t	operationalStatus	: 1;
	uint16_t	inputMultiplexer	: 3;
	uint16_t	pgaConfig			: 3;
	uint16_t	operatingMode		: 1;
	uint16_t	dataRate			: 3;
	uint16_t	comparatorMode		: 1;
	uint16_t	comparatorPolarity	: 1;
	uint16_t	compartorLatching	: 1;
	uint16_t	comparatorQueue		: 2;
}ADS1115_ConfigReg_t;



#define v_reference 3.3

/*typedef enum {
	vol3_3 = 33,
	vol5_0 = 5
}Vref_t;*/




typedef struct ADS1115_Config_Tag ADS1115_Handle_t;
ADS1115_Handle_t* ADS1115_init(I2C_HandleTypeDef *hi2c, uint16_t Addr, ADS1115_Config_t config);
void ADS1115_deinit();
void ADS1115_updateConfig(ADS1115_Handle_t *pConfig, ADS1115_Config_t config);
void ADS1115_updateI2Chandler(ADS1115_Handle_t *pConfig, I2C_HandleTypeDef *hi2c);
void ADS1115_updateAddress(ADS1115_Handle_t *pConfig, uint16_t address);
void ADS1115_SetDefault(ADS1115_Handle_t *pConfig);
int16_t ADS1115_oneShotMeasure();
int16_t ADS1115_getData();
void ADS1115_setThresholds(ADS1115_Handle_t *pConfig, int16_t lowValue, int16_t highValue);
void ADS1115_startContinousMode(ADS1115_Handle_t* pConfig);
void ADS1115_stopContinousMode(ADS1115_Handle_t* pConfig);
void ADS1115_setConversionReadyPin(ADS1115_Handle_t* pConfig);




float raw_to_voltage(
		uint16_t raw,
		ADS1115_Handle_t *pConfig);

float voltage_to_resistance(
		float voltage,
		float vRef,
		float Res
		);

float resistance_to_temperature(
		float resistance
		);











void setting_ADS1115 (uint8_t os, uint8_t mux, uint8_t pga, uint8_t mode, uint8_t dr,uint8_t comp_mode,
		uint8_t comp_pol, uint8_t comp_lat, uint8_t comp_que);

void write_ADS1115 (uint8_t os, uint8_t mux, uint8_t pga, uint8_t mode, uint8_t dr,uint8_t comp_mode, uint8_t comp_pol, uint8_t comp_lat, uint8_t comp_que,

		uint8_t slave_address, uint32_t timeout, I2C_HandleTypeDef *hi2c,
		uint8_t reg
		);

void read_date_ADS1115 (uint8_t os, uint8_t mux, uint8_t pga, uint8_t mode,
		uint8_t dr,uint8_t comp_mode, uint8_t comp_pol, uint8_t comp_lat, uint8_t comp_que,

		uint8_t slave_address, uint32_t timeout, I2C_HandleTypeDef *hi2c,
		uint8_t reg
		);



#endif /* SRC_ADS1115_H_ */
