struct ADS1118_Config_t configReg1118 ;

void ads1118_config(void)
{
	configReg1118.operatingMode					=	SCS;
    configReg1118.channel						=	AIN0_AIN1;
	configReg1118.pgaConfig				    =	FS4096;
	configReg1118.modeConfig				=	PDSSM;
	configReg1118.dataRate						=	SPS250;
	configReg1118.ADC_TSmode						=	ADC_MODE;
	configReg1118.pullMode						=	PULL_UP_DIS;
	configReg1118.validMode						=	DATA_VALID;   
	configReg1118.readyMode                    = DATA_READY;
}


void ADS1118_SetDefault(ADS1115_Handle_t *pConfig){
	configReg1118.operatingMode					=	SCS;
    configReg1118.channel						=	AIN0_AIN1;
	configReg1118.pullMode						=	PULL_UP_DIS;
	configReg1118.readyMode                    = DATA_READY;
	
	configReg1118.ADC_TSmode						=	ADC_MODE;
	configReg1118.validMode						=	DATA_VALID;   

	configReg1118.pgaConfig				    =	FS4096;
	configReg1118.dataRate						=	SPS250;
	configReg1118.modeConfig				=	CCM;


	prepareConfigFrame(bytes, configReg1118);

}



void SPI_config(void)
{
	
}


float raw_to_voltage(
		uint16_t raw,
		ADS1118_Handle_t *pConfig)
{
	ADS1118_Config_t configReg1118 = pConfig->config;
	uint8_t PGA = configReg.pgaConfig;
	float LSB;
	switch (PGA)
	{
		case 0x0:{LSB=6.144/32768;} break;
		case 0x1:{LSB=4.048/32768.;}break;
		case 0x2:{LSB=2.048/32768.;}break;
		case 0x3:{LSB=1.024/32768;}break;
		case 0x4:{LSB=0.512/32768;}break;
		default: {LSB=0.256/32768;};
	}
	float voltage = ((float)raw)*LSB;
	return voltage;
};





float voltage_to_resistance(
		float voltage,
		float vRef,
		float Res)
{
	float Th_resist=(voltage*Res*1000.)/( vRef - voltage);
	return Th_resist;
};



float resistance_to_temperature(float resistance)
{
	float Beta=4381.5;
	float R0 = 100000.;
	float T0 = 25.+273.15;
	float temperature =1/(1/Beta*log(resistance/R0)+1/(T0))-273.15;
	return temperature;
};

