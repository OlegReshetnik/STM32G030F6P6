#pragma once
#include "stm32_io.h"

#define I2C2_SDA				A12
#define I2C2_SCL				A11

#define I2C2_SPEED_STANDART		0x10707DBCUL
#define I2C2_SPEED_FAST			0x00602173UL
#define I2C2_SPEED_FASTPLUS		0x00300B29UL


void I2C_Init(void);

uint8_t I2C_Write(uint8_t address, uint8_t *data, uint8_t cnt); // адрес должен быть уже сдвинут
