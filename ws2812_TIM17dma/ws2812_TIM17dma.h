#pragma once

#include "system.h"
#include "stm32_io.h"

#define WS_GAMMA_TABLE // Use the gamma correction table. If not defined, then use the calculation

// ws2812 led strip: PA7, timer 17, pmm mode, channel 1 ouput
#define WS_LED_PIN		A7

//64 mHz timer values
#define WS_LO			24 // 19 - 48 mHz
#define WS_HI			48 // 39 - 48 mHz
#define WS_INT			80 // 60 - 48 mHz


#define NUM_LEDS		12 // num leds in strip
#define NUM_BYTES		(NUM_LEDS*3)
#define NUM_DMA_BYTES	(NUM_BYTES*8+2)

void WsInit(void); // Call after calling any of the effects 
void WsSend(void); // Send RGB data to led strip

void wsWeel(void); // Call every 30 milliseconds
void wsFire(void); // Call every 80 milliseconds
void wsColors(void); // Call every 60 milliseconds
void wsSparkles(void); // Call every 60 milliseconds
void wsBugs(void); // Call every 40 milliseconds
void wsBeat(void); // Call every 70 milliseconds

/*
Example:

#define NEXT_TIME	10000UL // 10 sec switch to next effect

typedef struct {
	void (*p_effect)(void);
	uint32_t fps;
} effect_t;

const effect_t effects[] = {
	{wsFire, 80},
	{wsWeel, 30},
	{wsBugs, 40},
	{wsColors, 60},
	{wsSparkles, 60},
	{wsBeat, 70}
};
#define N_EFF	(sizeof(effects)/sizeof(effects[0]))

int main (void) {
	uint32_t oldFps = 0, oldT = 0;
	uint16_t i_effect = 0;

	SysInit();
	WsInit();

	for(;;) {
		if ((millis-oldFps) > effects[i_effect].fps) {
			oldFps = millis;

			effects[i_effect].p_effect(); // Call effect

			WsSend(); // Send RGB data to led strip
		}

		if ((millis-oldT) > NEXT_TIME) {
			oldT = millis;
			if (++i_effect == N_EFF) { i_effect = 0; }
		}
	}
}
*/
