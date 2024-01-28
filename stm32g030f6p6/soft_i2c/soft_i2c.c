#include "soft_i2c.h"

static void I2C_Delay(void) {
	volatile uint32_t us = _DEL_I2C;
	while(us--);
}

void I2C_Init(void) {
	PORT_CLOCK_ON(I2C_SCL);
	PIN_MODE(I2C_SCL, MODE_OUTPUT);
	SET_SPEED(I2C_SCL, SPEED_LOW);
	OPEN_DRAIN(I2C_SCL);
	SET_PUP_PUD(I2C_SCL, PUPD_NO);
	PIN_SET(I2C_SCL);

	PORT_CLOCK_ON(I2C_SDA);
	PIN_MODE(I2C_SDA, MODE_OUTPUT);
	SET_SPEED(I2C_SDA, SPEED_LOW);
	OPEN_DRAIN(I2C_SDA);
	SET_PUP_PUD(I2C_SDA, PUPD_NO);
	PIN_SET(I2C_SDA);
}

void I2C_Start(void) {
	I2C_Delay();
	PIN_RESET(I2C_SDA);
	I2C_Delay();
	PIN_RESET(I2C_SCL);
	I2C_Delay();
}

void I2C_Stop(void) {
	//I2C_Delay();
	PIN_SET(I2C_SCL);
	I2C_Delay();
	PIN_SET(I2C_SDA);
	I2C_Delay();
}

uint8_t I2C_WriteByte(uint8_t b) {
	uint8_t m = 0x80, ack;

	while (m) {
		PIN_WRITE(I2C_SDA, (m & b));
		I2C_Delay();
		PIN_SET(I2C_SCL);
		I2C_Delay();
		PIN_RESET(I2C_SCL);
		I2C_Delay();
		m >>= 1;
	}
	PIN_SET(I2C_SDA);
	I2C_Delay();
	PIN_SET(I2C_SCL);
	I2C_Delay();
	ack = (0 == PIN_GET(I2C_SDA));
	PIN_RESET(I2C_SCL);
	I2C_Delay();
	PIN_RESET(I2C_SDA);
	return ack;
}

uint8_t I2C_Write(uint8_t address, uint8_t *data, uint8_t cnt) { // the address must already be shifted
	I2C_Start();
	I2C_WriteByte(address);
	while (cnt--) { I2C_WriteByte(*data++); }
	I2C_Stop();
}
