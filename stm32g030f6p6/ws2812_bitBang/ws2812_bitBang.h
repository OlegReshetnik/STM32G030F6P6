#pragma once

#include "system.h"
#include "stm32_io.h"

#define WS_GAMMA_TABLE // Use the gamma correction table. If not defined, then use the calculation

// Настройки порта ленты
#define WS_PIN	    	B7 // ws2812 pin

#define NUM_LEDS		12 // num leds in strip
#define NUM_BYTES       (NUM_LEDS*3)

uint8_t WsGamma(uint8_t val);

void WsInit(void);
void WsSend(void);

void WsWeel(void);
