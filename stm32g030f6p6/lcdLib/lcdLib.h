#pragma once
#include "stm32_io.h"

#define I2C_MODE

#define LCD_X_SIZE			16
#define LCD_Y_SIZE			2

#ifdef I2C_MODE
//------------------------------- i2c mode -------------------------------
	#include "hard_i2c_2.h"
	//#include "soft_i2c.h"

	#define LCD_I2C_ADR			(0x27 << 1)
	#define LCD_I2C_BL			0b00001000 // back light
	#define LCD_I2C_EN			0b00000100
	#define LCD_I2C_RW			0b00000010
	#define LCD_I2C_RS			0b00000001
//----------------------------- end i2c mode -----------------------------
#else // !I2C_MODE
//------------------------------ 4-bit mode ------------------------------
	#ifndef LCD_E
		#define LCD_E				C15
		#define LCD_RS				C14
		#define LCD_D4				A0
		#define LCD_D5				A1
		#define LCD_D6				A2
		#define LCD_D7				A3
	#endif

//---------------------------- end 4-bit mode ----------------------------
#endif // I2C_MODE

#define _LCD_DEL_ONE_US		(64/6)
#define _LCD_DEL_COMMAND	50
#define _LCD_DEL_CLEAR		1000

void lcdInit(void);
void lcdClear(void);

void lcdGotoXy(uint8_t x_col, uint8_t y_row);

void lcdWrite(uint8_t ch);
void lcdString(uint8_t *str);

void lcdInt(uint32_t value, uint8_t ndigits);
void lcdTime(uint32_t seconds);

void lcdCreateChar(uint8_t char_code, const uint8_t *map);

//----------------------------------------- RUS CHAR GEN ----------------------------------------
#define _RUS_DELTA			500
#define _RUS_B_BG			0 + _RUS_DELTA //Б
#define _RUS_G_BG			1 + _RUS_DELTA //Г
#define _RUS_D_BG			2 + _RUS_DELTA //Д
#define _RUS_ZH_BG			3 + _RUS_DELTA //Ж
#define _RUS_Z_BG			4 + _RUS_DELTA //З
#define _RUS_I_BG			5 + _RUS_DELTA //И
#define _RUS_IJ_BG			6 + _RUS_DELTA //Й
#define _RUS_L_BG			7 + _RUS_DELTA //Л
#define _RUS_P_BG			8 + _RUS_DELTA //П
#define _RUS_U_BG			9 + _RUS_DELTA //У
#define _RUS_F_BG			10 + _RUS_DELTA //Ф
#define _RUS_TS_BG			11 + _RUS_DELTA //Ц
#define _RUS_CH_BG			12 + _RUS_DELTA //Ч
#define _RUS_SH_BG			13 + _RUS_DELTA //Ш
#define _RUS_SCH_BG			14 + _RUS_DELTA //Щ
#define _RUS_TVRD_BG		15 + _RUS_DELTA //Ъ
#define _RUS_YY_BG			16 + _RUS_DELTA //Ы
#define _RUS_MYAG_BG		17 + _RUS_DELTA //Ь
#define _RUS_JE_BG			18 + _RUS_DELTA //Э
#define _RUS_YU_BG			19 + _RUS_DELTA //Ю
#define _RUS_YA_BG			20 + _RUS_DELTA //Я
#define _RUS_B_SM			21 + _RUS_DELTA //б
#define _RUS_V_SM			22 + _RUS_DELTA //в
#define _RUS_G_SM			23 + _RUS_DELTA //г
#define _RUS_D_SM			24 + _RUS_DELTA //д
#define _RUS_ZH_SM			25 + _RUS_DELTA //ж
#define _RUS_Z_SM			26 + _RUS_DELTA //з
#define _RUS_I_SM			27 + _RUS_DELTA //и
#define _RUS_IJ_SM			28 + _RUS_DELTA //й
#define _RUS_K_SM			29 + _RUS_DELTA //к
#define _RUS_L_SM			30 + _RUS_DELTA //л
#define _RUS_M_SM			31 + _RUS_DELTA //м
#define _RUS_N_SM			32 + _RUS_DELTA //н
#define _RUS_P_SM			33 + _RUS_DELTA //п
#define _RUS_T_SM			34 + _RUS_DELTA //т
#define _RUS_F_SM			35 + _RUS_DELTA //ф
#define _RUS_TS_SM			36 + _RUS_DELTA //ц
#define _RUS_CH_SM			37 + _RUS_DELTA //ч
#define _RUS_SH_SM			38 + _RUS_DELTA //ш
#define _RUS_SCH_SM			39 + _RUS_DELTA //щ
#define _RUS_TVRD_SM		40 + _RUS_DELTA //ъ
#define _RUS_YY_SM			41 + _RUS_DELTA //ы
#define _RUS_MYAG_SM		42 + _RUS_DELTA //ь
#define _RUS_JE_SM			43 + _RUS_DELTA //э
#define _RUS_YU_SM			44 + _RUS_DELTA //ю
#define _RUS_YA_SM			45 + _RUS_DELTA //я
//--------------------------------------- END RUS CHAR GEN --------------------------------------
