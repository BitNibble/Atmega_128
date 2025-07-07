/************************************************************************
	PCF8575LCD
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: all
Date:     11042024
Comment:
	Tested Atemga128 16Mhz and Atmega328 8Mhz
************************************************************************/
#ifndef _PCF8575LCD_H_
	#define _PCF8575LCD_H_

/***Compiler***/

/***Working Frequency***/
#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

/***Global Library***/
#include "pcf8575.h"
#include <inttypes.h>

/***Global Constant & Macro***/
// ASIGN PORT PINS TO LCD (can be setup in any way)
#define PCF_DB0 0
#define PCF_DB1 1
#define PCF_DB2 2
#define PCF_DB3 3
#define PCF_DB4 4
#define PCF_DB5 5
#define PCF_DB6 6
#define PCF_DB7 7
#define PCF_RS 8
#define PCF_RW 9
#define PCF_EN 10
#define PCF_NC 11
/***************/
// CMD RS
#define PCF_INST 0
#define PCF_DATA 1
// ticks depends on CPU frequency 16Mhz -> 0 8Mhz -> 0
#define PCF_LCD_W_N_TICKS 0
#define PCF_LCD_R_N_TICKS 0

/***Global Variable***/
typedef struct{
	void (*write)(char c, unsigned short D_I);
	char (*read)(unsigned short D_I);
	void (*BF)(void);
	void (*putch)(char c);
	char (*getch)(void);
	void (*string)(const char *s); // RAW
	void (*string_size)(const char* s, uint8_t size); // RAW
	void (*hspace)(uint8_t n);
	void (*clear)(void);
	void (*gotoxy)(unsigned int y, unsigned int x);
	void (*reboot)(void);
}PCF8575_LCD0;

/***Global Header***/
PCF8575_LCD0* pcf8575_lcd0(void);
PCF8575_LCD0 pcf8575_lcd0_enable( uint8_t pcf8575_id, uint8_t twi_prescaler );

#endif

/***EOF***/

