#include "ws2812_TIM17dma.h"

uint8_t leds[NUM_BYTES];
uint8_t wsdata[NUM_DMA_BYTES];

#ifdef WS_GAMMA_TABLE
	const uint8_t wsGammaTable[256] = {
		0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
		2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5,
		5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 8, 8, 8, 8, 8,
		9, 10, 10, 10, 10, 11, 11, 12, 12, 12, 12, 13, 13, 13, 14, 14,
		15, 15, 15, 16, 17, 17, 17, 17, 18, 18, 19, 20, 20, 20, 20, 21,
		22, 22, 23, 23, 23, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 30,
		30, 31, 31, 32, 33, 33, 34, 35, 35, 36, 37, 37, 38, 38, 39, 40,
		41, 41, 42, 43, 44, 45, 45, 46, 47, 47, 48, 49, 50, 51, 52, 53,
		54, 54, 55, 56, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
		68, 69, 69, 70, 71, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 84,
		85, 86, 87, 89, 90, 91, 93, 94, 95, 96, 97, 98, 100, 101, 102, 103,
		105, 106, 108, 109, 110, 111, 113, 115, 117, 118, 119, 121, 122, 123, 124, 127,
		128, 130, 131, 133, 134, 136, 137, 139, 140, 143, 145, 146, 147, 148, 151, 153,
		154, 156, 158, 159, 162, 163, 165, 167, 169, 171, 173, 174, 176, 179, 180, 182,
		185, 186, 188, 190, 192, 194, 196, 199, 201, 202, 205, 207, 209, 211, 214, 216,
		218, 220, 223, 225, 226, 230, 231, 235, 236, 240, 241, 245, 246, 250, 251, 255,
	};
	#define wsGamma(val)    wsGammaTable[val]
#else
	uint8_t wsGamma(uint8_t val) { return ((uint32_t)val * val + 255) >> 8; }
#endif


void WsInit(void) {
	// Clocks enable
	PORT_CLOCK_ON(WS_LED_PIN);
	RCC->APBENR2 |= RCC_APBENR2_TIM17EN; // Enable the peripheral clock TIM17
	RCC->AHBENR |= RCC_AHBENR_DMA1EN; // Enable DMA1 clock

	// Pin init
	PIN_MODE(WS_LED_PIN, MODE_ALT); SET_ALT(WS_LED_PIN, 5); // PA7 TIM17_CH1 -> AF5
	SET_SPEED(WS_LED_PIN, SPEED_HIGH);

	// timer counter & prescaller
	TIM17->PSC = 0; // 64 Mhz
	TIM17->ARR = WS_INT-1; // 800 kHz
	TIM17->CNT = 0;
	TIM17->CCR1 = 0;

	// timer pwm output pin mode
	TIM17->CR1 |=  TIM_CR1_ARPE; // Auto-reload preload enable
	TIM17->CCMR1 |= 0b110 << TIM_CCMR1_OC1M_Pos | TIM_CCMR1_OC1PE; // 0110: PWM mode 1, Output Compare 1 preload enable
	TIM17->BDTR |= TIM_BDTR_MOE; // 1: OC and OCN outputs are enabled
	TIM17->CCER |= TIM_CCER_CC1E; // 1: Capture mode enabled / OC1 signal is output on the corresponding output pin

	// timer dma set
	TIM17->DIER |= TIM_DIER_UDE; // Update DMA request enable

	TIM17->CR1 |= TIM_CR1_CEN; // Start count

	// Set DMA
	DMAMUX1_Channel0->CCR = 49; // DMA request MUX input TIM17_UP -> 49

	DMA1_Channel1->CCR |=	(0b01U << DMA_CCR_PL_Pos) |// Channel priority level 01: Medium
							(0b00U << DMA_CCR_MSIZE_Pos) |// Memory size 00: 8-bits
							(0b01U << DMA_CCR_PSIZE_Pos) |// Peripheral size 01: 16-bits
							DMA_CCR_MINC |// 1: Memory increment mode enabled
							DMA_CCR_DIR;// 1: Read from memory

	DMA1_Channel1->CPAR = (uint32_t) (&(TIM17->CCR1));
	DMA1_Channel1->CMAR = (uint32_t)(wsdata);
	
}

//Send RGB data to led strip
void WsSend(void) {
	uint8_t b, m;
	uint8_t *pleds = leds;
	uint8_t *pwsdata = wsdata;

	*pwsdata++ = 0;
	for (uint8_t i=0; i<NUM_BYTES; i++) {
		b = *pleds++; m = 0x80;
		while (m) { *pwsdata++ = (m & b)? WS_HI: WS_LO; m >>= 1; }
	}
	*pwsdata = 0;

	DMA1_Channel1->CCR &= ~(DMA_CCR_EN);
	DMA1_Channel1->CNDTR = NUM_DMA_BYTES;
	DMA1_Channel1->CCR |= DMA_CCR_EN;
}

uint16_t random16(void) {
	static uint16_t rand16seed;
	rand16seed *= (uint16_t)2053;
	rand16seed += (uint16_t)13849;
	return rand16seed;
}

uint8_t random8(void) {
	uint16_t rand = random16();
	return (uint8_t)(((uint8_t)(rand & 0xFF)) + ((uint8_t)(rand >> 8)));
}

uint8_t random8_1(uint8_t lim) {
	return ((random8()*lim + lim) >> 8);
}

uint8_t random8_2(uint8_t min, uint8_t lim) {
	return (random8_1(lim - min) + min);
}

uint8_t wsSub(uint8_t i, uint8_t j) {
	int t = i - j;
	if (t < 0) t = 0;
	return t;
}

uint8_t wsAdd( uint8_t i, uint8_t j) {
	unsigned int t = i + j;
	if( t > 255) t = 255;
	return t;
}

void wsClear(void) { for (uint16_t i=0; i<NUM_BYTES; i++) { leds[i] = 0; } }

uint8_t wsFade(uint8_t val, uint8_t fade) { return ( ((uint32_t)val * (uint32_t)fade) >> 8 ) + ((val && fade)? 1: 0); }

void setRGB(uint8_t r, uint8_t g, uint8_t b, uint16_t idx, uint8_t fade) {
	uint8_t *grb = &leds[idx*3];
	*grb++ = wsGamma(wsFade(g, fade));
	*grb++ = wsGamma(wsFade(r, fade));
	*grb++ = wsGamma(wsFade(b, fade));
}

// CRGB HeatColor(uint8_t temperature)
// Approximates a 'black body radiation' spectrum for
// a given 'heat' level.  This is useful for animations of 'fire'.
// Heat is specified as an arbitrary scale from 0 (cool) to 255 (hot).
// This is NOT a chromatically correct 'black body radiation'
// spectrum, but it's surprisingly close, and it's fast and small.
void HeatColor(uint8_t temperature, uint16_t idx, uint8_t fade) {
	uint8_t r, g, b;
	uint8_t t192 = wsFade(temperature, 191);
	uint8_t heatramp = t192 & 0x3F;
	heatramp <<= 2;
	if (t192 & 0x80) { r = 255; g = 255; b = heatramp; }
	else if (t192 & 0x40) { r = 255; g = heatramp; b = 0; }
	else { r = heatramp; g = 0; b = 0; }
	setRGB(r, g, b, idx, fade);
}

void HSV2RGB(uint8_t h, uint8_t s, uint8_t v, uint16_t idx, uint8_t fade) {
	uint8_t r, g, b;
	uint8_t value = ((24 * h / 17) / 60) % 6;
	uint8_t vmin = (long)v - v * s / 255;
	uint8_t a = (long)v * s / 255 * (h * 24 / 17 % 60) / 60;
	uint8_t vinc = vmin + a;
	uint8_t vdec = v - a;
	switch (value) {
		case 0: r = v; g = vinc; b = vmin; break;
		case 1: r = vdec; g = v; b = vmin; break;
		case 2: r = vmin; g = v; b = vinc; break;
		case 3: r = vmin; g = vdec; b = v; break;
		case 4: r = vinc; g = vmin; b = v; break;
		case 5: r = v; g = vmin; b = vdec; break;
	}
	setRGB(r, g, b, idx, fade);
}

// *************************** Wheel **************************
void wsWeel(void) {
	static uint8_t nn = 255, fade = 127, inc = 1;

	for (uint8_t i=0; i<NUM_LEDS; i++) { HSV2RGB(nn+i*11, 255, 255, i, fade); }
	nn -= 2;
	fade += inc;
	if (fade > 254) { inc = 255; }
	else if (fade < 40) { inc = 1; }
}
// *************************** End Wheel **************************

// *************************** Fire **************************
#define COOLING		900
#define SPARKING	150
#define MIN_HOT	    120
#define MAX_HOT	    230
void wsFire(void) {
	static uint8_t heat[NUM_LEDS];
		for (uint16_t i = 0; i < NUM_LEDS; i++) { // Step 1.  Cool down every cell a little
			heat[i] = wsSub( heat[i], random8_1(COOLING/NUM_LEDS+2) );
		}
		for (uint16_t i = NUM_LEDS - 1; i > 1; i--) { // Step 2.  Heat from each cell drifts 'up' and diffuses a little
			heat[i] = (heat[i-1] + heat[i-2] + heat[i-2] ) / 3;
		}
		if (random8() < SPARKING) { // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
			uint8_t y = random8_1(4); heat[y] = wsAdd(heat[y], random8_2(MIN_HOT, MAX_HOT));
		}
		for (uint16_t i = 0; i < NUM_LEDS; i++) { // Step 4.  Map from heat cells to LED colors
			uint8_t hh = wsFade(heat[i], 240); HeatColor(hh, i, 255);
		}
}
// *************************** End Fire **************************

// ****************************** Colors ******************************
void wsColors(void) {
	static uint8_t hue = 0, fade = 127, inc = 1;

	hue += 3;
	fade += inc;
	if (fade > 254) { inc = 255; }
	else if (fade < 10) { inc = 1; }
	for (uint16_t i=0; i<NUM_LEDS; i++) { HSV2RGB(hue, 255, 255, i, fade); }
}

// ****************************** Confetti ******************************
#define TRACK_STEP 40
void wsSparkles(void) {
	for (uint16_t i=0; i<NUM_BYTES; i++) { leds[i] = wsSub(leds[i], TRACK_STEP); }
	uint16_t i = random8_1(NUM_LEDS);
	if ((leds[i*3] + leds[i*3+1] + leds[i*3+2]) == 0) { HSV2RGB(random8(), 255, 255, i, random8_2(100, 255)); }
}

// ****************************** Fireflies ******************************
#define MAX_SPEEDx2		500
#define BUGS_AMOUNT 	3
void wsBugs(void) {
	static int8_t speed[BUGS_AMOUNT];
	static int8_t pos[BUGS_AMOUNT];
	static uint8_t bugsHue[BUGS_AMOUNT];
	static uint8_t flag = 1;
	
	wsClear();

	for (uint8_t i = 0; i < BUGS_AMOUNT; i++) {
		bugsHue[i] += (flag)? random8(): random8_1(2);

		speed[i] += (int8_t)(random8()) - 127;
		if (speed[i] == 0) { speed[i] = (int8_t)(random8()) - 127; }
		else if ((speed[i]*2) > MAX_SPEEDx2) { speed[i] = 0; }

		pos[i] += (flag)? random8_1(NUM_LEDS): speed[i]/100;
		if (pos[i] < 0) { pos[i] = 0; speed[i] = -speed[i]; }
		else if(pos[i] > (NUM_LEDS - 1)) { pos[i] = NUM_LEDS - 1; speed[i] = -speed[i]; }

		HSV2RGB(bugsHue[i], 255, 255, (uint8_t)pos[i], 255);
	}

	if (flag) {flag = 0;}
}

// ****************************** wsBeat ******************************
#define BEAT_START	75
#define BEAT_STEP	(210/NUM_LEDS)
void wsBeat(void) {
	static uint8_t pos = 0, inc = 4, fade = 127, finc = 1;

	wsClear();
	for(uint8_t i=0; i<pos; i++) { HSV2RGB(BEAT_STEP*i+BEAT_START, 255, 255, i, fade); }

	pos += inc;
	if (pos > (NUM_LEDS - 1)) { inc = 255; }
	else if (pos < 1) { inc = 4; }

	fade += finc;
	if (fade > 254) { finc = 255; }
	else if (fade < 40) { finc = 1; }
}
