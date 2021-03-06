#ifndef __MCRuntime_H__
#define __MCRuntime_H__

typedef struct{
	uint32_t param_index;
	int32_t param_value;
} watch_data_t;

//Core Functions
void mcCore_SM();

uint8_t register_user_callback_func(uint8_t slot, void (*p) (void*));

#define MODBUS_UART_CALLBACK_FUNCTION_SLOT 0


#endif
