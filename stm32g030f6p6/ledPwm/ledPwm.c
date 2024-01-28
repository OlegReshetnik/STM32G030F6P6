#include "ledPwm.h"

static uint32_t _led_del = LED_DEFAULT_DEL; 

void ledPwmInit(void) {
	PORT_CLOCK_ON(LED_PIN);
	PIN_MODE(LED_PIN, MODE_ALT);
	SET_SPEED(LED_PIN, SPEED_HIGH);
	SET_ALT(LED_PIN, 1);

	RCC->APBENR1 |= RCC_APBENR1_TIM3EN; // TIMER 3 clock enable
	TIM3->CCMR2 |= 6 << TIM_CCMR2_OC3M_Pos; // 0110: PWM mode 1 - In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1 else inactive.
	TIM3->CCER |= TIM_CCER_CC3E; // OC3 signal is output on the corresponding output pin
	TIM3->PSC = 63; // clock to 1 mhz
	TIM3->ARR = 256-1; // count to 256
	TIM3->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE; // Counter enabled, Auto-reload preload enable
}

void ledPwmTick(void) {
    static uint32_t oldT = 0;
    static uint8_t pwm = 0;
    static uint8_t delta = 1;

    if ((millis-oldT) > _led_del) {
            oldT = (uint16_t)millis;
			
            TIM3->CCR3 = pwm; //LedGamma(pwm);
            pwm += delta;
            if (pwm == 255) { delta = 255; }
            else if (pwm == 0) { delta = 1; }
        }
}

void ledPwmSet(uint32_t del) { _led_del = del; }
