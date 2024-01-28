#pragma once

#include "system.h"
#include "stm32_io.h"

#define LED_PIN				B0

#define LED_DEFAULT_DEL		500

void ledBlinkInit(void);
void ledBlinkTick(void);
void ledBlinkSet(uint32_t del);
