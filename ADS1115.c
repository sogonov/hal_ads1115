/*
 * ads1115.c
 *
 *  Created on: 4 апр. 2023 г.
 *      Author: sogonove
 */
#include <stdint.h>
#include "ads1115.h"

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
	if (reg==CONF_REG){
		setting_ADS1115 (os, mux, pga, mode, dr, comp_mode, comp_pol, comp_lat, comp_que); //настраиваем параметры конф. регистра
		HAL_I2C_Master_Transmit(hi2c, slave_address<<1, configReg, 3, timeout);//посылаем настройки и записываем их в ads
		HAL_I2C_Master_Receive(hi2c, slave_address<<1, checkReg, 2, timeout);//считываем регистр конфигурации для проверки
	}
	if (reg==CONV_REG){
			setting_ADS1115 (os, mux, pga, mode, dr, comp_mode, comp_pol, comp_lat, comp_que); //настраиваем параметры конф. регистра
			HAL_I2C_Master_Transmit(hi2c, slave_address<<1, CONV_REG, 1, timeout);//переключились на чтение конв. регистра
	}
}

void read_date_ADS1115 (uint8_t os, uint8_t mux, uint8_t pga, uint8_t mode,
		uint8_t dr,uint8_t comp_mode, uint8_t comp_pol, uint8_t comp_lat, uint8_t comp_que,
		uint8_t slave_address, uint32_t timeout, I2C_HandleTypeDef *hi2c,
		uint8_t reg
		){
	write_ADS1115(os, mux, pga, mode, dr, comp_mode, comp_pol, comp_lat, comp_que, slave_address, timeout, hi2c, reg);
	HAL_I2C_Master_Receive(hi2c, slave_address<<1, result_conv, 2, timeout);
}




/*void write_data(){









}

void setting_ADS1115(){









}
*/
