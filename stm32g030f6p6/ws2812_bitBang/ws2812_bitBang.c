#include "ws2812_bitBang.h"

uint8_t leds[NUM_BYTES];

#ifdef WS_GAMMA_TABLE
const uint8_t wsGammaTable[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
		1, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6,
		6, 7, 7, 7, 8, 8, 8, 8, 8, 9, 10, 10, 10, 10, 11, 11, 12, 12, 12, 12,
		13, 13, 13, 14, 14, 15, 15, 15, 16, 17, 17, 17, 17, 18, 18, 19, 20, 20,
		20, 20, 21, 22, 22, 23, 23, 23, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29,
		30, 30, 31, 31, 32, 33, 33, 34, 35, 35, 36, 37, 37, 38, 38, 39, 40, 41,
		41, 42, 43, 44, 45, 45, 46, 47, 47, 48, 49, 50, 51, 52, 53, 54, 54, 55,
		56, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 69, 70, 71,
		73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 84, 85, 86, 87, 89, 90, 91, 93,
		94, 95, 96, 97, 98, 100, 101, 102, 103, 105, 106, 108, 109, 110, 111,
		113, 115, 117, 118, 119, 121, 122, 123, 124, 127, 128, 130, 131, 133,
		134, 136, 137, 139, 140, 143, 145, 146, 147, 148, 151, 153, 154, 156,
		158, 159, 162, 163, 165, 167, 169, 171, 173, 174, 176, 179, 180, 182,
		185, 186, 188, 190, 192, 194, 196, 199, 201, 202, 205, 207, 209, 211,
		214, 216, 218, 220, 223, 225, 226, 230, 231, 235, 236, 240, 241, 245,
		246, 250, 251, 255, };
inline uint8_t WsGamma(uint8_t val) { return wsGammaTable[val]; }
#else
    uint8_t WsGamma(uint8_t val) { return ((uint32_t)val * val + 255) >> 8; }
#endif

void WsInit(void) {
	PORT_CLOCK_ON(WS_PIM);
	PIN_MODE(WS_PIN, MODE_OUTPUT);
	SET_SPEED(WS_PIN, SPEED_HIGH);
}

void WsSend(void) {
	uint8_t b, m;
	uint8_t *data = leds;
	uint16_t len = NUM_BYTES;
	__disable_irq();
	while (len--) {
		b = *data++;
		m = 0x80;
		while (m) {
			if (m & b) {
				PIN_SET(WS_PIN);
				asm volatile(" nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;");
				asm volatile(" nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;");
				asm volatile(" nop; nop; nop; nop; nop; nop; nop; nop;"); // 28
				PIN_RESET(WS_PIN);
				asm volatile(" nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;");
				asm volatile(" nop; nop; nop; nop;"); // 14
			} else {
				PIN_SET(WS_PIN);
				asm volatile(" nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;");
				asm volatile(" nop; nop; nop; nop;"); // 14
				PIN_RESET(WS_PIN);
				asm volatile(" nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;");
				asm volatile(" nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;");
				asm volatile(" nop; nop; nop; nop; nop; nop; nop; nop;"); // 28
			}
			m >>= 1;
		}
	}
	__enable_irq();
}

uint8_t WsFade(uint8_t val, uint8_t fade) {
	return (((uint32_t) val * (uint32_t) fade) >> 8) + ((val && fade) ? 1 : 0);
}

void WsSetRGB(uint8_t r, uint8_t g, uint8_t b, uint16_t idx, uint8_t fade) {
	uint8_t *grb = &leds[idx * 3];
	*grb++ = WsGamma(WsFade(g, fade));
	*grb++ = WsGamma(WsFade(r, fade));
	*grb++ = WsGamma(WsFade(b, fade));
}

void WsWheel(uint8_t color, uint16_t idx, uint8_t fade) {
	uint8_t shift, r, g, b;
	if (color > 170) {
		shift = (color - 170) * 3;
		r = shift;
		g = 0;
		b = 255 - shift;
	} else if (color > 85) {
		shift = (color - 85) * 3;
		r = 0;
		g = 255 - shift;
		b = shift;
	} else {
		shift = color * 3;
		r = 255 - shift;
		g = shift;
		b = 0;
	}
	WsSetRGB(r, g, b, idx, fade);
}

// *************************** Wheel **************************
//#define WEEL_FPS    20
void WsWeel(void) {
	static uint8_t nn = 255, fade = 127, inc = 1;

	for (uint8_t i = 0; i < NUM_LEDS; i++) { WsWheel(nn + i * 11, i, fade); }
	nn -= 2;
	fade += inc;
	if (fade > 254) { inc = 255; }
	else if (fade < 70) { inc = 1; }
}
// *************************** End Wheel **************************
