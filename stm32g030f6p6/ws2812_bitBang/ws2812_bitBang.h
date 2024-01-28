#pragma once

#include "system.h"
#include "stm32_io.h"

#define WS_GAMMA_TABLE // использовать таблицу гамма коррекции, если не определено, то используем вычисление

// Настройки порта ленты
#define WS_PIN	    	7 // пин подключения
#define WS_PORT		    GPIOB // порт подключения

#define NUM_LEDS		12 // количество светодиодов
#define NUM_BYTES       (NUM_LEDS*3)

uint8_t WsGamma(uint8_t val);

void WsInit(void);
void WsSend(void);

void WsWeel(void);
