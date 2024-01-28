#include "hard_i2c_2.h"

void I2C_Init(void) {
	RCC->APBENR1 |= RCC_APBENR1_I2C2EN; // Enable the peripheral clock I2C2
	PORT_CLOCK_ON(I2C2_SCL);

	PIN_MODE(I2C2_SCL, MODE_ALT);
	SET_SPEED(I2C2_SCL, SPEED_LOW);
	OPEN_DRAIN(I2C2_SCL);
	//SET_PUP_PUD(I2C2_SCL, PUPD_NO);
	SET_ALT(I2C2_SCL, 6); // i2c2 alt 6

	//PORT_CLOCK_ON(I2C2_SDA);
	PIN_MODE(I2C2_SDA, MODE_ALT);
	SET_SPEED(I2C2_SDA, SPEED_LOW);
	OPEN_DRAIN(I2C2_SDA);
	//SET_PUP_PUD(I2C2_SDA, PUPD_NO);
	SET_ALT(I2C2_SDA, 6); // i2c2 alt 6

	I2C2->TIMINGR = I2C2_SPEED_FAST; // Timing register value
	I2C2->CR1 = I2C_CR1_PE; // Periph enable
}

uint8_t I2C_Write(uint8_t address, uint8_t *data, uint8_t cnt) { // the address must already be shifted
	uint32_t timeout;

	I2C2->CR2 |= (cnt << I2C_CR2_NBYTES_Pos) | address;
	for (uint8_t i=0; i<cnt; i++) {
		if (i==0) { I2C2->CR2 |= I2C_CR2_START; }
		if (i==1 || cnt == 1 ) { I2C2->CR2 |= I2C_CR2_AUTOEND; }

		I2C2->TXDR = *data++;
		timeout = 10000UL;
		while(!(I2C2->ISR & I2C_ISR_TXE)) { // Wait for data to be transmitted
			if ((timeout--) == 0) { return 0; } // Time out
		}
	}

	return 1;
}
