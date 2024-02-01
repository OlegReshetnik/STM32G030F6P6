#pragma once

#include "system.h"
#include "stm32_io.h"


#define SENSOR_PIN			A5

#define SENSOR_NUM_READ		1000
#define SENSOR_PERIOD		50
#define SENSOR_CLICK		(400/SENSOR_PERIOD)
#define SENSOR_LONG_CLICK	(1200/SENSOR_PERIOD)

#define SENSOR_TRESH		((uint16_t)16)

void SensorInit(void);
uint8_t SensorTouch(void); // return !=0 is touch


#define S_NO			0
#define S_CLK			1
#define S_LNG_CLK		2

uint8_t SensorTick(void);
