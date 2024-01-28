#include "system.h"

void SysClock64(void) {
	RCC->PLLCFGR = RCC_PLLCFGR_PLLSRC_HSI // PLL clock source 10: HSI16
	| RCC_PLLCFGR_PLLN_3 // PLL frequency multiplication factor N 0001000: 8
	| RCC_PLLCFGR_PLLR_0 // PLL VCO division factor R for PLLRCLK clock output 001: 2
	| RCC_PLLCFGR_PLLREN ; // PLLRCLK clock output enable

	RCC->CR |= RCC_CR_PLLON; // Turn ON PLL

	FLASH->ACR |= FLASH_ACR_LATENCY_1; // Two wait states

	while(!(RCC->CR & RCC_CR_PLLRDY)); // Wait for PLL ready flag

	RCC->CFGR |= RCC_CFGR_SW_1; // Switch system clock to PLL  SWS[2:0]: System clock switch status 010: PLLRCLK
}

volatile uint32_t millis = 0;

void SysTick_Handler(void) { millis++; }

void delay(uint32_t msval) {
	uint32_t oldT = millis;
	while ((millis - oldT) < msval);
}

void SysInit() {
    SysClock64();
	SysTick_Config(64000);
}
