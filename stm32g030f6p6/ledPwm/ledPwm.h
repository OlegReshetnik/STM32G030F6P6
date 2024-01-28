#pragma once

#include "system.h"
#include "stm32_io.h"

#define LED_PIN				B0

#define LED_DEFAULT_DEL		3

void ledPwmInit(void);
void ledPwmTick(void);
void ledPwmSet(uint32_t del);
