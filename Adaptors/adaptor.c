/*
 * adaptor.c
 *
 *  Created on: Nov 8, 2021
 *      Author: mg
 */

#include "stm32f7xx_hal.h"

extern TIM_HandleTypeDef htim6;
extern UART_HandleTypeDef huart7;

typedef struct S_Adaptation{
	TIM_HandleTypeDef* comm_htim;
	UART_HandleTypeDef* comm_huart;
} runtime_adaptor;

typedef struct S_Digital_Channel{
	uint32_t port;
	uint32_t pin;
} Digital_Channel;

static Digital_Channel inputChannel[3];
static Digital_Channel outputChannel[3];

void initiate_input_channels(){
	inputChannel[0].port = GPIOF;
	inputChannel[0].pin = GPIO_PIN_3; //EXT3

	inputChannel[1].port = GPIOF;
	inputChannel[1].pin = GPIO_PIN_5; //EXT5

	inputChannel[2].port = GPIOF;
	inputChannel[2].pin = GPIO_PIN_10; //EXT10
}

void initiate_output_channels(){
	outputChannel[0].port = GPIOF;
	outputChannel[0].pin = GPIO_PIN_0;

	outputChannel[1].port = GPIOF;
	outputChannel[1].pin = GPIO_PIN_2;

	outputChannel[2].port = GPIOF;
	outputChannel[2].pin = GPIO_PIN_13;
}

runtime_adaptor ra = {.comm_htim = &htim6, .comm_huart = &huart7};

uint8_t uart_rx_data;

//Leds are connected to: PF0-PF2-PF13
//Please write down GPIO output function in your hardware
void hal_gpio_write_pin(uint16_t chNum, uint8_t value){
	HAL_GPIO_WritePin(outputChannel[chNum].port, outputChannel[chNum].pin, value);
}

uint8_t  hal_gpio_read_pin(uint32_t chNum){
	//return values >= 2, depicts error
	return HAL_GPIO_ReadPin(inputChannel[chNum].port, inputChannel[chNum].pin);

}

//Please write down "get system tick" function in your hardware
uint32_t hal_get_tick(){
	return HAL_GetTick();
}

void /*__attribute__((weak))*/ hal_init_tick(){
	HAL_InitTick(0);
}

//Communication Channel Adaptation

//Please write down functions for your communication channel
//And put this function right after your initialisations
void init_comm_data_service(){
	HAL_UART_Receive_IT(ra.comm_huart, &uart_rx_data, 1);
}

//Please write down functions for communication timing services
//And put this function right after your initialisations
void init_comm_timing_service(){
	HAL_TIM_Base_Start_IT(ra.comm_htim);
	stop_comm_timer(ra.comm_htim);
}

void start_comm_timer(TIM_HandleTypeDef* htim){
	htim->Instance->CR1 &= ~0x01; //Stop Timer
	htim->Instance->CNT = 0; //Reset Counter
	htim->Instance->CR1 |= 0x01; //Start Timer
}

void stop_comm_timer(TIM_HandleTypeDef* htim){
	htim->Instance->CR1 &= ~0x01; //Stop Timer
	htim->Instance->CNT = 0; //Reset Counter
}

//Callbacks
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_Receive_IT(huart, &uart_rx_data, 1);
	start_comm_timer(ra.comm_htim);

	//When new data received copy this data to the runtime buffers
	Runtime_CommDataService_NewData_Received(0, &uart_rx_data, 1);
/*
	//Call user callback
	User_Callback_Function* ucf = get_ucf();
	if(ucf[MODBUS_UART_CALLBACK_FUNCTION_SLOT].f != 0){
		ucf[MODBUS_UART_CALLBACK_FUNCTION_SLOT].f(&uart_rx_data);
	}
	*/
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if(htim == ra.comm_htim){
		stop_comm_timer(ra.comm_htim);

		//External trigger makes runtime to process data
		//This trigging is needed for Modbus (3.5 Char)
		Runtime_CommDataService_Process_DataBuffer(0);
	}
}

//Modbus UART Transmit Functions
void /*__attribute__((weak))*/ hal_modbus_uart_tx(uint8_t* pData, uint16_t Size){
	HAL_UART_Transmit_IT(ra.comm_huart, pData, Size);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
}

//-----------------------UNIQUE ID-----------------------------------------
void get_uniqueid(uint8_t* id, uint16_t len){
	uint32_t* buf = id;
	buf[0] 	= (uint32_t) READ_REG(*((uint32_t *)UID_BASE));
	buf[1] = (uint32_t) READ_REG(*((uint32_t *)(UID_BASE + 0x04U)));
	buf[2] = (uint32_t) READ_REG(*((uint32_t *)(UID_BASE + 0x14U)));
}

//---------------------Flash functions---------------------------------------


#define ADDR_FLASH_SECTOR_23     ((uint32_t)0x081E0000)
#define FLASH_MEMORY_SIZE (128*1024)

void get_flash_memory_info(uint32_t* start_addr, uint32_t* size){
	*start_addr = ADDR_FLASH_SECTOR_23;
	*size = FLASH_MEMORY_SIZE;
}

uint8_t write_to_flash(uint8_t* p, uint32_t start_addr, uint16_t size)
{
	uint8_t ret = 0;
	uint16_t i;
	uint32_t data;

	HAL_FLASH_Unlock();

	for (i = 0; i < size; i+=4) {
                data = *(uint32_t*)(p+i);
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, start_addr + i, data) == HAL_OK) ret = 1;
		else {
			ret = 0;
			break;
		}
	}

	HAL_FLASH_Lock();
}

uint8_t erase_flash(uint32_t start_addr)
{
	uint8_t ret = 0;

	uint32_t SectorError = 0;
    FLASH_EraseInitTypeDef EraseInitStruct;


	/* Unlock the Flash to enable the flash control register access *************/
	HAL_FLASH_Unlock();

	/* Erase the user Flash area
	(area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS         ;
	EraseInitStruct.NbSectors = 1;
	EraseInitStruct.Sector = FLASH_SECTOR_23;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
	{
		ret = 1;
	}

	else ret = 0;

	return ret;
}


void initiate_runtime()
{
	  init_comm_data_service();
	  init_comm_timing_service();
	  initiate_input_channels();
	  initiate_output_channels();
}
