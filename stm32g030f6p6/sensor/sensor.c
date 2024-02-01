#include "sensor.h"

static uint16_t SensorStartValue;
static uint32_t SensorOldT;
static uint8_t SensorCnt = 0;

static uint8_t SensorReadOnce(void) {
	uint8_t cnt = 0;

	PIN_MODE(SENSOR_PIN, MODE_OUTPUT);
	PIN_RESET(SENSOR_PIN);
	asm volatile(" nop; nop; nop; nop;");

	PIN_MODE(SENSOR_PIN, MODE_INPUT);
	SET_PUP_PUD(SENSOR_PIN, PUPD_PU);

	while((PIN_GET(SENSOR_PIN)) == 0) { cnt++; }

	return cnt;
}

uint16_t SensorRead(uint16_t numRead) {
	uint16_t sum = 0;
	while (numRead--) { sum += SensorReadOnce(); }
	return sum;
}

uint8_t SensorTouch(void) {
	uint16_t val = SensorRead(SENSOR_NUM_READ);
	if (val > SensorStartValue) { SensorStartValue++; }
	else { SensorStartValue--; }
	uint16_t tresh = SensorStartValue+SensorStartValue/SENSOR_TRESH;
	return (val > tresh);
}

void SensorInit(void) {
	PORT_CLOCK_ON(SENSOR_PIN);
	SensorStartValue = SensorRead(SENSOR_NUM_READ);
	SensorOldT = millis;
}

uint8_t SensorTick(void) {
	uint8_t ret = 0;
	if ((millis-SensorOldT) > SENSOR_PERIOD) {
		SensorOldT = millis;
		if (SensorTouch()) { SensorCnt++; }
		else {
			if (SensorCnt > SENSOR_LONG_CLICK) { ret = S_LNG_CLK; }
			else if (SensorCnt > SENSOR_CLICK) { ret = S_CLK; }
			SensorCnt = 0;
		}
	}
	return ret;
}
