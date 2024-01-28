#include "lcdLib.h"

static const uint8_t _charGen[] = {
0b11111,0b10000,0b10000,0b11110,0b10001,0b10001,0b11110,0b00000, //0 Б
0b11111,0b10000,0b10000,0b10000,0b10000,0b10000,0b10000,0b00000, //1 Г
0b00110,0b01010,0b01010,0b01010,0b01010,0b01010,0b11111,0b10001, //2 Д
0b10101,0b10101,0b10101,0b01110,0b10101,0b10101,0b10101,0b00000, //3 Ж
0b01110,0b10001,0b00001,0b00110,0b00001,0b10001,0b01110,0b00000, //4 З
0b10001,0b10001,0b10001,0b10011,0b10101,0b11001,0b10001,0b00000, //5 И
0b10101,0b10001,0b10001,0b10011,0b10101,0b11001,0b10001,0b00000, //6 Й
0b00111,0b01001,0b01001,0b01001,0b01001,0b01001,0b10001,0b00000, //7 Л
0b11111,0b10001,0b10001,0b10001,0b10001,0b10001,0b10001,0b00000, //8 П
0b10001,0b10001,0b10001,0b01111,0b00001,0b10001,0b01110,0b00000, //9 У
0b00100,0b01110,0b10101,0b10101,0b10101,0b01110,0b00100,0b00000, //10 Ф
0b10010,0b10010,0b10010,0b10010,0b10010,0b10010,0b11111,0b00001, //11 Ц
0b10001,0b10001,0b10001,0b01111,0b00001,0b00001,0b00001,0b00000, //12 Ч
0b10001,0b10001,0b10001,0b10101,0b10101,0b10101,0b11111,0b00000, //13 Ш
0b10001,0b10001,0b10001,0b10101,0b10101,0b10101,0b11111,0b00001, //14 Щ
0b11000,0b01000,0b01000,0b01110,0b01001,0b01001,0b01110,0b00000, //15 Ъ
0b10001,0b10001,0b10001,0b11101,0b10011,0b10011,0b11101,0b00000, //16 Ы
0b10000,0b10000,0b10000,0b11110,0b10001,0b10001,0b11110,0b00000, //17 Ь
0b01110,0b10001,0b00001,0b00111,0b00001,0b10001,0b01110,0b00000, //18 Э
0b10010,0b10101,0b10101,0b11101,0b10101,0b10101,0b10010,0b00000, //19 Ю
0b01111,0b10001,0b10001,0b01111,0b00101,0b01001,0b10001,0b00000, //20 Я

0b00011,0b01100,0b10000,0b11110,0b10001,0b10001,0b01110,0b00000, //21 б
0b00000,0b00000,0b11110,0b10001,0b11110,0b10001,0b11110,0b00000, //22 в
0b00000,0b00000,0b11110,0b10000,0b10000,0b10000,0b10000,0b00000, //23 г
0b00000,0b00000,0b00110,0b01010,0b01010,0b01010,0b11111,0b10001, //24 д
0b00000,0b00000,0b10101,0b10101,0b01110,0b10101,0b10101,0b00000, //25 ж
0b00000,0b00000,0b01110,0b10001,0b00110,0b10001,0b01110,0b00000, //26 з
0b00000,0b00000,0b10001,0b10011,0b10101,0b11001,0b10001,0b00000, //27 и
0b01010,0b00100,0b10001,0b10011,0b10101,0b11001,0b10001,0b00000, //28 й
0b00000,0b00000,0b10010,0b10100,0b11000,0b10100,0b10010,0b00000, //29 к
0b00000,0b00000,0b00111,0b01001,0b01001,0b01001,0b10001,0b00000, //30 л
0b00000,0b00000,0b10001,0b11011,0b10101,0b10001,0b10001,0b00000, //31 м
0b00000,0b00000,0b10001,0b10001,0b11111,0b10001,0b10001,0b00000, //32 н
0b00000,0b00000,0b11111,0b10001,0b10001,0b10001,0b10001,0b00000, //33 п
0b00000,0b00000,0b11111,0b00100,0b00100,0b00100,0b00100,0b00000, //34 т
0b00000,0b00000,0b00100,0b01110,0b10101,0b01110,0b00100,0b00000, //35 ф
0b00000,0b00000,0b10010,0b10010,0b10010,0b10010,0b11111,0b00001, //36 ц
0b00000,0b00000,0b10001,0b10001,0b01111,0b00001,0b00001,0b00000, //37 ч
0b00000,0b00000,0b10101,0b10101,0b10101,0b10101,0b11111,0b00000, //38 ш
0b00000,0b00000,0b10101,0b10101,0b10101,0b10101,0b11111,0b00001, //39 щ
0b00000,0b00000,0b11000,0b01000,0b01110,0b01001,0b01110,0b00000, //40 ъ
0b00000,0b00000,0b10001,0b10001,0b11101,0b10011,0b11101,0b00000, //41 ы
0b00000,0b00000,0b10000,0b10000,0b11110,0b10001,0b11110,0b00000, //42 ь
0b00000,0b00000,0b01110,0b10001,0b00111,0b10001,0b01110,0b00000, //43 э
0b00000,0b00000,0b10010,0b10101,0b11101,0b10101,0b10010,0b00000, //44 ю
0b00000,0b00000,0b01111,0b10001,0b01111,0b00101,0b01001,0b00000  //45 я
};

// Ё на E lat, ё на e lat
static const uint16_t _charRecode[] = {
//А        Б      В        Г          Д      Е        Ж           З          И          Й
 'A', _RUS_B_BG, 'B', _RUS_G_BG, _RUS_D_BG, 'E', _RUS_ZH_BG, _RUS_Z_BG, _RUS_I_BG, _RUS_IJ_BG,
// 'A', 'Y', 'B', _RUS_G_BG, _RUS_D_BG, 'E', _RUS_ZH_BG, _RUS_Z_BG, _RUS_I_BG, _RUS_IJ_BG,
//К        Л      М    Н    О       П       Р    С    Т        У          Ф
 'K', _RUS_L_BG, 'M', 'H', 'O', _RUS_P_BG, 'P', 'C', 'T', _RUS_U_BG, _RUS_F_BG,
//Х        Ц           Ч           Ш           Щ            Ъ             Ы           Ь
 'X', _RUS_TS_BG, _RUS_CH_BG, _RUS_SH_BG, _RUS_SCH_BG, _RUS_TVRD_BG, _RUS_YY_BG, _RUS_MYAG_BG,
 //   Э           Ю           Я
 _RUS_JE_BG, _RUS_YU_BG, _RUS_YA_BG,

//а        б         в         г         д      е       ж          з         и         й
 'a', _RUS_B_SM,_RUS_V_SM,_RUS_G_SM,_RUS_D_SM, 'e',_RUS_ZH_SM,_RUS_Z_SM,_RUS_I_SM,_RUS_IJ_SM,
//   к          л          м          н      о        п      р    с        т      у        ф
_RUS_K_SM, _RUS_L_SM, _RUS_M_SM, _RUS_N_SM, 'o', _RUS_P_SM, 'p', 'c', _RUS_T_SM, 'y', _RUS_F_SM,
//х        ц           ч           ш           щ            ъ             ы           ь
 'x', _RUS_TS_SM, _RUS_CH_SM, _RUS_SH_SM, _RUS_SCH_SM, _RUS_TVRD_SM, _RUS_YY_SM, _RUS_MYAG_SM,
//   э           ю           я
_RUS_JE_SM, _RUS_YU_SM, _RUS_YA_SM
};

static uint8_t _createCharTable[8]; // Таблица созданных символов
static uint8_t _createCharTableIndex; // Индекс в таблице созданных символов
static uint8_t _x, _y;

static void _lcdDelOneUs(void) {
    volatile uint8_t i = _LCD_DEL_ONE_US;
    while(i--) { __NOP(); }
}

static void _lcdDelUs(uint16_t us) {
    while(us--) { _lcdDelOneUs(); }
}

#ifdef I2C_MODE
//------------------------------- i2c mode -------------------------------
	void _lcdInitIo(void) {
		I2C_Init();
		delay(1);
	}


	void _lcdWrite4bits(uint8_t value) {
		uint8_t data[2];
		data[0] = value | LCD_I2C_EN;
		data[1] = value;
		I2C_Write(LCD_I2C_ADR, data, 2);
	}

	void _lcdSendI2C(uint8_t cd, uint8_t rs) {
		uint8_t data[4];
		data[0] = (cd & 0xF0) | LCD_I2C_BL | LCD_I2C_EN | rs;
		data[1] = (cd & 0xF0) | LCD_I2C_BL;
		data[2] = (cd << 4) | LCD_I2C_BL | LCD_I2C_EN | rs;
		data[3] = (cd << 4) | LCD_I2C_BL;
		I2C_Write(LCD_I2C_ADR, data, 4);
	}

	void _lcdSendCmd(uint8_t cd) {
		_lcdSendI2C(cd, 0);
	}

	void _lcdSendData(uint8_t ch) {
		_lcdSendI2C(ch, LCD_I2C_RS);
	}

//----------------------------- end i2c mode -----------------------------
#else // !I2C_MODE
//------------------------------ 4-bit mode ------------------------------
	void _lcdInitIo(void) {
		ABC_PORT_CLOCK_ON();
		PIN_MODE(LCD_E, MODE_OUTPUT); // SET_SPEED(LCD_E, SPEED_LOW);
		PIN_MODE(LCD_RS, MODE_OUTPUT); // SET_SPEED(LCD_RS, SPEED_LOW);
		PIN_MODE(LCD_D4, MODE_OUTPUT); // SET_SPEED(LCD_D4, SPEED_LOW);
		PIN_MODE(LCD_D5, MODE_OUTPUT); // SET_SPEED(LCD_D5, SPEED_LOW);
		PIN_MODE(LCD_D6, MODE_OUTPUT); // SET_SPEED(LCD_D6, SPEED_LOW);
		PIN_MODE(LCD_D7, MODE_OUTPUT); // SET_SPEED(LCD_D7, SPEED_LOW);
	}

	void _lcdWrite4bits(uint8_t value) {
		PIN_WRITE(LCD_D4, (value & 0x10));
		PIN_WRITE(LCD_D5, (value & 0x20));
		PIN_WRITE(LCD_D6, (value & 0x40));
		PIN_WRITE(LCD_D7, (value & 0x80));

		PIN_SET(LCD_E); // pulse
		_lcdDelOneUs();
		PIN_RESET(LCD_E);
		_lcdDelOneUs();
	}
	
	void _lcdSendCmd(uint8_t cd) {
		_lcdWrite4bits(cd & 0xF0);
		_lcdWrite4bits(cd << 4);
		_lcdDelUs(_LCD_DEL_COMMAND);
	}

	void _lcdSendData(uint8_t ch) {
		PIN_SET(LCD_RS);
		_lcdDelOneUs();
		_lcdSendCmd(ch);
		_lcdDelOneUs();
		PIN_RESET(LCD_RS);
	}
//---------------------------- end 4-bit mode ----------------------------
#endif // I2C_MODE


void lcdInit(void) {
	_lcdInitIo();
	delay(15);
	_lcdWrite4bits(0x30); // 8 bit
	delay(5);
	_lcdWrite4bits(0x30); // 8 bit
	_lcdDelUs(120);
	_lcdWrite4bits(0x30); // 8 bit
	_lcdDelUs(120);
	_lcdWrite4bits(0x20); // 4 bit
	_lcdSendCmd(0x28); // 2 string
	_lcdSendCmd(0x0C); // display on, cursor off
	lcdClear();
}

void lcdGotoXy(uint8_t x_col, uint8_t y_row) {
	uint8_t offset = (y_row & 1) * 0x40 + (y_row & 2) * LCD_X_SIZE;
	_lcdSendCmd(0x80 | (x_col + offset));
	_x = x_col; _y = y_row;
}

void lcdCreateChar(uint8_t char_code, const uint8_t *map) {
	_lcdSendCmd(0x40 | (char_code << 3));
	uint8_t n = 8;
	while (n--) { _lcdSendData(*map++); }
	lcdGotoXy(_x, _y);
}

void lcdClear(void) {
	uint8_t i = 8;
	while(i--) { _createCharTable[i] = 0; }
	_x = _y =_createCharTableIndex = 0;
	_lcdSendCmd(0x01);
	_lcdDelUs(_LCD_DEL_CLEAR);
}

uint8_t _lcdRusRecode(uint8_t ch) {
	if (ch == 13) { return 10; }

	if (ch == 168) { return 'E'; }

	if (ch == 184) { return 'e'; }

	if (ch <  192) { return ch; } // А русская

	uint16_t recode = _charRecode[ch-192];
	if (recode < _RUS_DELTA) { return (uint8_t)recode; } // символ соответствует английскому

	// create symbol
	recode -= _RUS_DELTA;
	uint8_t i = 0;
	for( i = 0; i < 8; i++ ) {
		if( _createCharTable[i] == ch ) { return i; }
	}
	// Нет символа в таблице -> создаем его
	_createCharTable[_createCharTableIndex] = ch;
	lcdCreateChar(_createCharTableIndex, &_charGen[recode<<3]);
	//lcdCreateChar(0, testmap);
	ch = _createCharTableIndex++;
	_createCharTableIndex &= 7;
	return ch;
	//return '0';
}

void lcdWrite(uint8_t ch) {
	if (ch != 10) {
		_lcdSendData(_lcdRusRecode(ch));
		if (++_x < LCD_X_SIZE) { return; }
	} else { _x = 0; _y++;}

	if (_x == LCD_X_SIZE) { _x = 0; _y++; }

	if (_y == LCD_Y_SIZE) { _y = 0; }

	lcdGotoXy(_x, _y);
}

void lcdString(uint8_t *str) {
	while(*str) { lcdWrite(*str++); }
}

void lcdInt(uint32_t value, uint8_t ndigits) {
	uint8_t buf[16];
	uint8_t *pbuf = &buf[15];

	*pbuf-- = 0;
	while (ndigits--) {
		pbuf--;
		*pbuf = value%10 + '0';
		value /= 10;
	}

	lcdString(pbuf);
}

void lcdTime(uint32_t seconds) {
	if (seconds > 3599) {
		lcdInt(seconds/3600, 2);
		lcdWrite(':');
	}
	lcdInt((seconds/60)%60, 2);
	lcdWrite(':');
	lcdInt(seconds%60, 2);
}
