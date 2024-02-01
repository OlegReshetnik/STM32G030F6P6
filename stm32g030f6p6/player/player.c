#include "player.h"

static const uint16_t noteFreq[12] = { 0xD7C7, 0xCBAB, 0xC03C, 0xB573, 0xAB43, 0xA1A6, 0x9894, 0x9003, 0x87ED, 0x804D, 0x791A, 0x724E };

//Rescaled for channels count = 2
static const uint8_t sineTbl[64] = {
0x80, 0x86, 0x8C, 0x92, 0x98, 0x9E, 0xA3, 0xA8, 0xAD, 0xB1, 0xB4, 0xB8, 0xBA, 0xBC, 0xBE, 0xBF, 
0xBF, 0xBF, 0xBE, 0xBC, 0xBA, 0xB8, 0xB4, 0xB1, 0xAD, 0xA8, 0xA3, 0x9E, 0x98, 0x92, 0x8C, 0x86, 
0x80, 0x7A, 0x74, 0x6E, 0x68, 0x62, 0x5D, 0x58, 0x53, 0x4F, 0x4C, 0x48, 0x46, 0x44, 0x42, 0x41, 
0x41, 0x41, 0x42, 0x44, 0x46, 0x48, 0x4C, 0x4F, 0x53, 0x58, 0x5D, 0x62, 0x68, 0x6E, 0x74, 0x7A
};

static const uint8_t s_envelope[128] = { 
0xFF, 0xFA, 0xF5, 0xF0, 0xEB, 0xE7, 0xE2, 0xDE, 0xD9, 0xD5, 0xD1, 0xCD, 0xC9, 0xC5, 0xC1, 0xBD, 
0xB9, 0xB6, 0xB2, 0xAE, 0xAB, 0xA8, 0xA4, 0xA1, 0x9E, 0x9B, 0x98, 0x95, 0x92, 0x8F, 0x8C, 0x89, 
0x86, 0x84, 0x81, 0x7F, 0x7C, 0x7A, 0x77, 0x75, 0x73, 0x70, 0x6E, 0x6C, 0x6A, 0x68, 0x66, 0x64, 
0x62, 0x60, 0x5E, 0x5C, 0x5A, 0x58, 0x57, 0x55, 0x53, 0x52, 0x50, 0x4E, 0x4D, 0x4B, 0x4A, 0x48, 
0x47, 0x45, 0x44, 0x43, 0x41, 0x40, 0x3F, 0x3E, 0x3C, 0x3B, 0x3A, 0x39, 0x38, 0x37, 0x36, 0x35, 
0x33, 0x32, 0x31, 0x30, 0x30, 0x2F, 0x2E, 0x2D, 0x2C, 0x2B, 0x2A, 0x29, 0x28, 0x27, 0x26, 0x25, 
0x23, 0x22, 0x21, 0x20, 0x1F, 0x1E, 0x1D, 0x1C, 0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 
0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x00
};

static TPlayerState playerState;
static TNote * mel;
static uint8_t doProcess = 0;

void Player_Init(void) {
	// Clocks enable
	PORT_CLOCK_ON(BEEP_PIN);
	RCC->APBENR2 |= RCC_APBENR2_TIM14EN; // Enable the peripheral clock TIM14
	
	// Pin init
	PIN_MODE(BEEP_PIN, MODE_ALT); SET_ALT(BEEP_PIN, 4); // PA4 TIM14_CH1 -> AF4
	SET_SPEED(BEEP_PIN, SPEED_HIGH);

	// timer counter & prescaller
	TIM14->PSC = 13 - 1; // ‭4 923 077‬ tick/sec
	TIM14->ARR = 256 - 1; // 19 231 Interrupt/sec
	TIM14->CNT = 0;
	TIM14->CCR1 = 0;

	// timer pwm output pin mode
	TIM14->CR1 |=  TIM_CR1_ARPE; // Auto-reload preload enable
	TIM14->CCMR1 |= 0b110 << TIM_CCMR1_OC1M_Pos | TIM_CCMR1_OC1PE; // 0110: PWM mode 1, Output Compare 1 preload enable
	TIM14->CCER |= TIM_CCER_CC1E; // 1: Capture mode enabled / OC1 signal is output on the corresponding output pin

	// IRQ init
	TIM14->DIER |= TIM_DIER_UIE; // Enable tim17 interrupt
	NVIC_EnableIRQ(TIM14_IRQn);  // Enable IRQ
}

void Player_Pause(void) {
	TIM14->CR1 &=  ~(TIM_CR1_CEN); // stop count
}

void Player_Resume(void) {
	TIM14->CR1 |=  TIM_CR1_CEN; // start count
}

// TIM14 timer overflow interrupt, executed 64 MHz / 13 / 256 = 19,231 times per second
void TIM14_IRQHandler(void) {
	uint8_t sample;
	uint8_t i;
	TChannelState* pState; 

	TIM14->SR &= ~TIM_SR_UIF; //Clean UIF Flag

	if (playerState.envelopeSkipCnt == 0) {
		playerState.envelopeSkipCnt = ENVELOPE_SKIP_MAX;
		for (i = 0; i < CH_CNT; i++) { if (playerState.chanSt[i].envCnt < 255) { playerState.chanSt[i].envCnt++; } }
 		playerState.eventCnt--;
		if (playerState.eventCnt == 0) { doProcess = 1; } 
	}
	playerState.envelopeSkipCnt--;           

	sample = 0x80;                  
	pState = &playerState.chanSt[0];
	for (i = 0; i < CH_CNT; i++) {
		uint8_t sineVal;
		uint8_t envelopeVal;
		pState->dds += pState->dds_add;
		sineVal = sineTbl[(pState->dds >> (PLAYER_FREQ_SHR-5)) & 63];
		envelopeVal = s_envelope[pState->envCnt >> 1]; 
		//scale sineVal as unsigned value using simple mul/shift. We divide by 256, not 255 for simplicity.
		sineVal = (uint8_t)((((uint16_t)sineVal)*envelopeVal) >> 8);
		//after scaling, 0x80 (biased zero ) became m_envelopeVal / 2
		//create unsigned value by biasing back    
		sineVal -= envelopeVal >> 1;
		sample += sineVal;
		pState++;        
	}     

	TIM14->CCR1 = sample;
}

static void Player_SetDefs(void) {
	TIM14->CR1 &=  ~(TIM_CR1_CEN); // stop count
	for (uint8_t i=0; i<CH_CNT; i++) {
		playerState.chanSt[i].dds = 0;
		playerState.chanSt[i].dds_add = 0;
		playerState.chanSt[i].envCnt = 0;
	}
	playerState.eventCnt = 1;
	playerState.envelopeSkipCnt = 0;
	playerState.melody = mel;
	TIM14->CR1 |=  TIM_CR1_CEN; // Start count
}

static uint16_t Player_GetNoteFreqAdd(uint8_t _noteNumber) {
	uint8_t noteIndex = 132 - 1 - _noteNumber;
	uint8_t noteDiv = noteIndex / 12;  //how many octaves down
	noteIndex = noteIndex - (12 * noteDiv);
	return noteFreq[noteIndex] >> noteDiv;
}

static void Player_ProcessEvents(void) {
	uint8_t channelIndex;
	uint8_t noteNumber;
	uint16_t delta;
	uint16_t cadd;

	playerState.eventCnt = 0xFFFF;
	delta = playerState.melody->deltaCh;
	noteNumber = playerState.melody->noteNum;
	playerState.melody++;

	channelIndex = delta & 0x7;
	delta >>= 3;

	if (delta == 0) {
		//playerState.melody = 0;
		Player_SetDefs(); // Restart //Player_Finished();
		return;
	}

	channelIndex |= (noteNumber >> (7-3)) & 8;

	noteNumber &= 0x7f;
	if (noteNumber == 0) { cadd = 0; }
	else if (noteNumber == 1) { cadd = 1; }
	else { cadd = Player_GetNoteFreqAdd(noteNumber); }

	playerState.chanSt[channelIndex].dds = 0;
	playerState.chanSt[channelIndex].dds_add = cadd;
	playerState.chanSt[channelIndex].envCnt = 0;    
	playerState.eventCnt = delta;            
}

void Player_StartMelody(const TNote * pesnya) {
	mel = (TNote *)pesnya;
	Player_SetDefs();
}

void Player_Tick(void) {
	if (doProcess) {
		doProcess = 0;
		Player_ProcessEvents();
	} 
}
