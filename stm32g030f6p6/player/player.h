#pragma once
#include "system.h"
#include "stm32_io.h"


#define BEEP_PIN			A4

#define CH_CNT				2
#define ENVELOPE_SKIP_MAX	74

//Player frequency = 19 231
//Pitch = 12
#define PLAYER_FREQ_SHR               13 // 14 - pitch 12
#define PLAYER_COUNTER_MAX_VAL        0


typedef struct __attribute__((packed)) {
	uint8_t		noteNum;// highest bit is highest for channel index
	uint16_t	deltaCh;// 15-3 bits - Delta value, 2,1,0 bits - channel index  
} TNote;

typedef struct __attribute__((packed)) {
	uint32_t	dds;//sample index counter
	uint16_t	dds_add;//0 - off, 1 - drum, >0 - add value for counter
	uint8_t		envCnt;    
} TChannelState;

typedef struct __attribute__((packed)) {
	TNote * melody;
	uint16_t		eventCnt;
	uint8_t			envelopeSkipCnt;    
	TChannelState	chanSt[CH_CNT];
} TPlayerState;


void Player_Init(void); // call before Player_StartMelody

void Player_StartMelody(const TNote * pesnya);

void Player_Tick(void); // call in main loop

void Player_Pause(void);

void Player_Resume(void);
