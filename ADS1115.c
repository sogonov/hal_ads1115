/*
 * ads1115.c
 *
 *  Created on: 4 апр. 2023 г.
 *      Author: sogonove
 */
#include <stdint.h>
#include "stdlib.h"
#include "ads1115.h"


struct ADS1115_Config_Tag{
	I2C_HandleTypeDef 	*hi2c;
	uint16_t 			address;
	ADS1115_Config_t 	config;
};

static void prepareConfigFrame(uint8_t *pOutFrame, ADS1115_Config_t config){
	pOutFrame[0] = 0x01;
	pOutFrame[1] |= (config.operatingMode << 7) |(config.channel << 4)
			|(config.pgaConfig << 1)|(config.modeConfig << 0);

	pOutFrame[2] |= (config.dataRate << 5) | (config.compareMode << 4) | (config.polarityMode << 3)
			| (config.latchingMode << 2) | config.queueComparator;
}


ADS1115_Handle_t* ADS1115_init(I2C_HandleTypeDef *hi2c, uint16_t Addr, ADS1115_Config_t config){
	ADS1115_Handle_t *pConfig = malloc(sizeof(ADS1115_Handle_t));
	pConfig->hi2c = hi2c;
	pConfig->address = Addr;
	pConfig->config = config;
	return pConfig;
}

void ADS1115_deinit(ADS1115_Handle_t* pConfig){
	free(pConfig);
}

void ADS1115_updateConfig(ADS1115_Handle_t *pConfig, ADS1115_Config_t config){
	pConfig->config = config;
	uint8_t bytes[3] = {0};
	prepareConfigFrame(bytes, pConfig->config);
	HAL_I2C_Master_Transmit(pConfig->hi2c, (pConfig->address << 1), bytes, 3, 100);
}

void ADS1115_updateI2Chandler(ADS1115_Handle_t *pConfig, I2C_HandleTypeDef *hi2c){
	pConfig->hi2c = hi2c;
}

void ADS1115_updateAddress(ADS1115_Handle_t *pConfig, uint16_t address){
	pConfig->address = address;
}
void ADS1115_SetDefault(ADS1115_Handle_t *pConfig){
	uint8_t bytes[3] = {0};
	ADS1115_Config_t configReg = pConfig->config;
	configReg.operatingMode = W_NE;
	configReg.channel = AIN3_AIN_GND;

	configReg.pgaConfig = FS4096;
	configReg.modeConfig =CCM;

	configReg.dataRate = SPS250;
	configReg.compareMode = TCWH;

	configReg.polarityMode = AL;
	configReg.latchingMode = NLC;
	configReg.queueComparator = DC;

	prepareConfigFrame(bytes, configReg);
	HAL_I2C_Master_Transmit(pConfig->hi2c, (pConfig->address << 1), bytes, 3, 100);

}

int16_t ADS1115_oneShotMeasure(ADS1115_Handle_t *pConfig){
	uint8_t bytes[3] = {0};
	prepareConfigFrame(bytes, pConfig->config);
	bytes[1] |= (1 << 7); // OS one shot measure
	HAL_I2C_Master_Transmit(pConfig->hi2c, (pConfig->address << 1), bytes, 3, 100);
	return ADS1115_getData(pConfig);
}


int16_t ADS1115_getData(ADS1115_Handle_t *pConfig){
	uint8_t bytes[2] = {0};
	bytes[0] = 0x00;
	HAL_I2C_Master_Transmit(pConfig->hi2c, (pConfig->address << 1), bytes, 1, 50);
	if(HAL_I2C_Master_Receive(pConfig->hi2c, (pConfig->address << 1), bytes, 2, 50) != HAL_OK)
		return 0;
	int16_t readValue = ((bytes[0] << 8) | bytes[1]);
	if(readValue < 0)
		readValue = 0;
	return readValue;
}


void ADS1115_setThresholds(ADS1115_Handle_t *pConfig, int16_t lowValue, int16_t highValue){
	uint8_t ADSWrite[3] = { 0 };

	//hi threshold reg
	ADSWrite[0] = 0x03;
	ADSWrite[1] = (uint8_t)((highValue & 0xFF00) >> 8);
	ADSWrite[2] = (uint8_t)(highValue & 0x00FF);
	HAL_I2C_Master_Transmit(pConfig->hi2c, (pConfig->address << 1), ADSWrite, 3, 100);

	//lo threshold reg
	ADSWrite[0] = 0x02;
	ADSWrite[1] = (uint8_t)((lowValue & 0xFF00) >> 8);
	ADSWrite[2] = (uint8_t)(lowValue & 0x00FF);
	HAL_I2C_Master_Transmit(pConfig->hi2c, (pConfig->address << 1), ADSWrite, 3, 100);
}

void ADS1115_setConversionReadyPin(ADS1115_Handle_t* pConfig){
	ADS1115_setThresholds(pConfig, 0x0000, 0xFFFF);
}

void ADS1115_startContinousMode(ADS1115_Handle_t* pConfig){
	uint8_t bytes[3] = {0};
	ADS1115_Config_t configReg = pConfig->config;
	configReg.modeConfig = CCM;
	prepareConfigFrame(bytes, configReg);
	HAL_I2C_Master_Transmit(pConfig->hi2c, (pConfig->address << 1), bytes, 3, 100);
}

void ADS1115_stopContinousMode(ADS1115_Handle_t* pConfig){
	uint8_t bytes[3] = {0};
	ADS1115_Config_t configReg = pConfig->config;
	configReg.modeConfig = PDSSM;
	prepareConfigFrame(bytes, configReg);
	HAL_I2C_Master_Transmit(pConfig->hi2c, (pConfig->address << 1), bytes, 3, 100);
}




/*void ADS1115_CustomMode(ADS1115_Handle_t* pConfig, char configParameter, uint8_t valueParameter){
	uint8_t bytes[3] = {0};
	char confParameter[8] ={operatingMode,channel,pgaConfig,dataRate,compareMode,polarityMode,latchingMode,queueComparator};
    if(configParameter==operatingMode){
			ADS1115_Config_t configReg = pConfig->config;
			configReg.operatingMode = valueParameter;
			HAL_I2C_Master_Transmit(pConfig->hi2c, (pConfig->address << 1), bytes, 3, 100);
	}

    if(configParameter==channel){
			ADS1115_Config_t configReg = pConfig->config;
			configReg.channel = valueParameter;
			HAL_I2C_Master_Transmit(pConfig->hi2c, (pConfig->address << 1), bytes, 3, 100);
	}

    if(configParameter==pgaConfig){
  			ADS1115_Config_t configReg = pConfig->config;
  			configReg.pgaConfig = valueParameter;
  			HAL_I2C_Master_Transmit(pConfig->hi2c, (pConfig->address << 1), bytes, 3, 100);
  	}
    if(configParameter==dataRate){
    			ADS1115_Config_t configReg = pConfig->config;
    			configReg.dataRate = valueParameter;
    			HAL_I2C_Master_Transmit(pConfig->hi2c, (pConfig->address << 1), bytes, 3, 100);
    }
    if(configParameter==compareMode){
       			ADS1115_Config_t configReg = pConfig->config;
       			configReg.compareMode = valueParameter;
       			HAL_I2C_Master_Transmit(pConfig->hi2c, (pConfig->address << 1), bytes, 3, 100);
      }
    if(configParameter==polarityMode){
       			ADS1115_Config_t configReg = pConfig->config;
       			configReg.polarityMode = valueParameter;
       			HAL_I2C_Master_Transmit(pConfig->hi2c, (pConfig->address << 1), bytes, 3, 100);
      }
    if(configParameter==latchingMode){
       			ADS1115_Config_t configReg = pConfig->config;
       			configReg.latchingMode = valueParameter;
       			HAL_I2C_Master_Transmit(pConfig->hi2c, (pConfig->address << 1), bytes, 3, 100);
      }
    if(configParameter==queueComparator){
           			ADS1115_Config_t configReg = pConfig->config;
           			configReg.queueComparator = valueParameter;
           			HAL_I2C_Master_Transmit(pConfig->hi2c, (pConfig->address << 1), bytes, 3, 100);
          }


}*/










/*

uint8_t configReg[3];
uint8_t checkReg[2];
uint8_t result_conv[2];
int resi;






//Функция, записывающая в два байта информацию,которая запишется в config register по i2c
void setting_ADS1115 (
		uint8_t os, uint8_t mux, uint8_t pga, uint8_t mode, uint8_t dr,uint8_t comp_mode,
		uint8_t comp_pol, uint8_t comp_lat, uint8_t comp_que
		){
	configReg[1] = (os << 7)|(mux << 4) | (pga << 1) | mode;
	configReg[2] = (dr << 5) | (comp_mode << 4) | (comp_pol << 3) | (comp_lat << 2) | comp_que;
	}



void write_ADS1115 (uint8_t os, uint8_t mux, uint8_t pga, uint8_t mode,
		uint8_t dr,uint8_t comp_mode, uint8_t comp_pol, uint8_t comp_lat, uint8_t comp_que,

		uint8_t slave_address, uint32_t timeout, I2C_HandleTypeDef *hi2c,
		uint8_t reg
		)
{
	configReg[0] = reg;//выбор регистра
	setting_ADS1115 (os, mux, pga, mode, dr, comp_mode, comp_pol, comp_lat, comp_que); //настраиваем параметры конф. регистра
	HAL_I2C_Master_Transmit(hi2c, slave_address<<1, configReg, 3, timeout);//посылаем настройки и записываем их в ads
	HAL_I2C_Master_Receive (hi2c, slave_address<<1, checkReg, 2, timeout);//считываем регистр конфигурации для проверки

}

void read_date_ADS1115 (uint8_t os, uint8_t mux, uint8_t pga, uint8_t mode,
		uint8_t dr,uint8_t comp_mode, uint8_t comp_pol, uint8_t comp_lat, uint8_t comp_que,
		uint8_t slave_address, uint32_t timeout, I2C_HandleTypeDef *hi2c,
		uint8_t reg
		){
	write_ADS1115(os, mux, pga, mode, dr, comp_mode, comp_pol, comp_lat, comp_que, slave_address, timeout, hi2c, reg);
	HAL_I2C_Master_Receive(hi2c, slave_address<<1, result_conv, 2, timeout);
}


*/

/*void write_data(){









}

void setting_ADS1115(){









}
*/
