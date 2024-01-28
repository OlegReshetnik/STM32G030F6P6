#pragma once
#include "stm32g0xx.h"

extern volatile uint32_t millis;

void delay(uint32_t msval);

void SysInit();
