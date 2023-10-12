


typedef struct{
	OperatingMode_t 	operatingMode;
	MultiplexerConfig_t channel;
	PGA_Config_t 		pgaConfig;
	MODEconfig_t 		modeConfig;
	DataRate_t			dataRate;
	ADC_TSmode_t		ADC_TSmode;
	PULL_t 	   				pullMode;
	Valid_t					validMode;
	RDY_t    				readyMode;
}ADS1118_Config_t;


typedef enum {
	CONV = 0x1,          
    SCS = 0x1
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


typedef enum
{
	FS6144 = 0x0,
	FS4096 = 0x1,
	FS2048 = 0x2,
	FS1024 = 0x3,
	FS0512 = 0x4,
    FS0256 = 0x5
} PGA_Config_t;


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

//mode(adc/temperature sensor)
typedef enum
{
    ADC_MODE    			=   0x0,
    TEMPERATURE_MODE 	=  	0x1
} ADC_TSmode_t;


//pull enable/disable
typedef enum
{
    PULL_UP_DIS = 0x0,
    PULL_UP_EN  = 0x1
} PULL_t;
//data valid
typedef enum
{
    DATA_VALID      = 0x1,
    DATA_INVALID    = 0x2
} Valid_t;


typedef enum
{
    DATA_READY 	= 0x0,
    DATA_NREADY = 0x1
} RDY_t;

#define DEFAULT_PGA		PGA_2048

void ads1118_config(void);

void SPI_config(void);
float ads1118_get_temperature(void);

void ads1118_init(ADS_InitTypeDef* ConfigReg);

uint16_t ads1118_convert(unsigned char channel);
uint16_t ads1118_get_differential_0_1(uint8_t PGA);
uint16_t ads1118_get_differential_2_3(uint8_t PGA);
double	ads1118_get_differential_0_1_mv(uint8_t PGA);
double	ads1118_get_differential_2_3_mv(uint8_t PGA);
uint8_t SPI_send_Byte(uint8_t byte);
uint16_t SPI_read_write_Reg(uint16_t CofigReg);
uint16_t SPI_read_write_Byte(uint16_t TxData);
void SPI_RCC_Configuration(void);
void SPI_GPIO_Configuration(void);
void SPI_NVIC_Configuration(void);
int8_t  ads1118_get_temp_thermo_type_k( double input_voltage_mV, double input_cold_junction_C, double *output_hot_junction_C);
uint16_t ads1118_median_filter(uint16_t *pbuffer);
double ads1118_median_average_filter(double *pbuffer);
#endif