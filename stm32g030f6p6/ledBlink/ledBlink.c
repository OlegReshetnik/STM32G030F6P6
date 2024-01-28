#include "ledBlink.h"

static uint32_t _led_del = LED_DEFAULT_DEL; 

void ledBlinkInit(void) {
    PORT_CLOCK_ON(LED_PIN);
	PIN_MODE(LED_PIN, MODE_OUTPUT);
}

void ledBlinkTick(void) {
    static uint32_t oldT;
    if ((millis-oldT) > _led_del) {
		oldT = millis;
		PIN_TOOGLE(LED_PIN);
	}
}

void ledBlinkSet(uint32_t del) { _led_del = del; }
