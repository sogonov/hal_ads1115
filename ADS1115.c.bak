/*
 * ads1115.c
 *
 *  Created on: 4 апр. 2023 г.
 *      Author: sogonove
 */
#include <stdint.h>

uint8_t configReg[2];

//Функция, записывающая в два байта информацию,которая запишется в config register
void setting_ADS1115 (uint8_t os, uint8_t mux, uint8_t pga, uint8_t mode, uint8_t dr,uint8_t comp_mode, uint8_t comp_pol, uint8_t comp_lat, uint8_t comp_que){
	configReg[1] = (os << 7)|(mux << 4) | (pga << 1) | mode;
	configReg[0] = (dr << 5) | (comp_mode << 4) | (comp_pol << 3) | (comp_lat << 2) | comp_que;
}

void init_i2c(choice_i2c, slave_address,data,size, timeout){


	HAL_I2C_Master_Transmit(&hi2c2, ADS1115_ADDRESS<<1, setting, 3, 100);


}
void init_ADS1115(){

}




/*void write_data(){









}

void setting_ADS1115(){









}
*/
