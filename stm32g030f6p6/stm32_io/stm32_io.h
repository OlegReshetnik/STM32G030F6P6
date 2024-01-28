//STM32G030F6P6 io library
#pragma once
#include "system.h"
#include "stm32iodefs.h"


#define MODE_INPUT						0 // 00: Input mode
#define MODE_OUTPUT						1 // 01: General purpose output mode
#define MODE_ALT						2 // 10: Alternate function mode
#define MODE_ANALOG						3 // 11: Analog mode (reset state)

#define SPEED_VERYLOW					0 // 00: Very low speed
#define SPEED_LOW						1 // 01: Low speed
#define SPEED_HIGH						2 // 10: High speed
#define SPEED_VERYHIGH					3 // 11: Very high speed

#define PUPD_NO							0 // 00: No pull-up, pull-down
#define PUPD_PU							1 // 01: Pull-up
#define PUPD_PD							2 // 10: Pull-down


#define _CONCAT(A, B)					A##B
#define CONCAT(A, B)					_CONCAT(A, B)

#define GET_GPIO(VAL)					CONCAT(VAL, _GPIO)
#define GET_PIN(VAL)					CONCAT(VAL, _PIN)

#define CLEAR1MASK(PIN)					~(1 << GET_PIN(PIN))
#define SET1MASK(PIN)					1 << GET_PIN(PIN)

#define CLEAR2MASK(PIN)					(~(3 << (GET_PIN(PIN) << 1)))
#define SET2MASK(PIN, VAL)				(VAL << (GET_PIN(PIN) << 1))

#define CLEAR4MASK(PIN)					(~(15 << ((7 & GET_PIN(PIN)) << 2)))
#define SET4MASK(PIN, VAL)				(VAL << ((7 & GET_PIN(PIN)) << 2))

#define PIN_L3(PIN)						(GET_PIN(PIN) >> 3)


#define PORT_NUM(PIN)					( ((uint32_t)GET_GPIO(PIN) - (uint32_t)IOPORT_BASE) / 0x00000400UL )

#define PORT_CLOCK_OFF(PIN)				RCC->IOPENR &= ~(1 << PORT_NUM(PIN))
#define PORT_CLOCK_ON(PIN)				RCC->IOPENR |= 1 << PORT_NUM(PIN)
#define ABC_PORT_CLOCK_ON()				RCC->IOPENR |= 7

#define PIN_MODE(PIN, MODE)				GET_GPIO(PIN)->MODER = (GET_GPIO(PIN)->MODER & CLEAR2MASK(PIN)) | SET2MASK(PIN, MODE)

#define SET_SPEED(PIN, SPEED)			GET_GPIO(PIN)->OSPEEDR = (GET_GPIO(PIN)->OSPEEDR & CLEAR2MASK(PIN)) | SET2MASK(PIN, SPEED)

#define SET_ALT(PIN, AF_NUM)			GET_GPIO(PIN)->AFR[PIN_L3(PIN)] = (GET_GPIO(PIN)->AFR[PIN_L3(PIN)] & CLEAR4MASK(PIN)) | SET4MASK(PIN, AF_NUM)

#define OPEN_DRAIN(PIN)					GET_GPIO(PIN)->OTYPER |= SET1MASK(PIN)
#define PUSH_PULL(PIN)					GET_GPIO(PIN)->OTYPER &= CLEAR1MASK(PIN)

#define SET_PUP_PUD(PIN, PUPD)			GET_GPIO(PIN)->PUPDR = (GET_GPIO(PIN)->PUPDR & CLEAR2MASK(PIN)) | SET2MASK(PIN, PUPD)

#define PIN_SET(PIN)					GET_GPIO(PIN)->BSRR = SET1MASK(PIN)
#define PIN_RESET(PIN)					GET_GPIO(PIN)->BRR = SET1MASK(PIN)

#define PIN_WRITE(PIN, VAL)				GET_GPIO(PIN)->BSRR = 1 << (GET_PIN(PIN) + (16*((VAL)==0)))

#define PIN_TOOGLE(PIN)					GET_GPIO(PIN)->ODR ^= SET1MASK(PIN)

#define PIN_GET(PIN)					(GET_GPIO(PIN)->IDR & SET1MASK(PIN))
