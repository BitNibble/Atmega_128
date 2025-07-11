/************************************************************************
	LCD
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: all
Date:     04072025
************************************************************************/
#ifndef _LCD_H_
	#define _LCD_H_

/*** Working Frequency ***/
#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

/*** Global Library ***/
#include <stdint.h>
#include <inttypes.h>

/*** Global Constant & Macro ***/
// ASIGN PORT PINS TO LCD (can be setup in any way)
//#define RS 0
//#define RW 1
//#define EN 2
//#define NC 3
//#define DB4 4
//#define DB5 5
//#define DB6 6
//#define DB7 7
/***************/
#define RS 7
#define RW 6
#define EN 5
#define NC 4
#define DB4 3
#define DB5 2
#define DB6 1
#define DB7 0
/***************/

/*** Handler ***/
typedef struct{
	void (*write)(char c, unsigned short D_I);
	char (*read)(unsigned short D_I);
	uint8_t (*BF)(void);
	void (*putch)(char c);
	char (*getch)(void);
	void (*string)(const char *s); // RAW
	void (*string_size)(const char* s, uint8_t size); // RAW
	void (*hspace)(uint8_t n);
	void (*clear)(void);
	void (*gotoxy)(unsigned int y, unsigned int x);
	void (*reboot)(void);
}LCD0, LCD1;

void lcd0_enable(volatile uint8_t *ddr, volatile uint8_t *pin, volatile uint8_t *port);
LCD0* lcd0(void);
void lcd1_enable(volatile uint8_t *ddr, volatile uint8_t *pin, volatile uint8_t *port);
LCD1* lcd1(void);

#endif
/***EOF***/

