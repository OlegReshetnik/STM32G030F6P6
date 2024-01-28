#pragma once
#include "stm32_io.h"

#ifndef I2C_SDA
	#define I2C_SDA				A12
	#define I2C_SCL				A11
#endif

#define _DEL_I2C				1 // 65/5 * 10

void I2C_Init(void);

//uint8_t I2C_WriteByte(uint8_t b); // return 1 if get ACK

uint8_t I2C_Write(uint8_t address, uint8_t *data, uint8_t cnt); // адрес должен быть сдвинут

