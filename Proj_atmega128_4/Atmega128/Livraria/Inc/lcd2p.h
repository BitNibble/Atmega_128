/************************************************************************
	LCD2P
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: all
Date:     12072025
************************************************************************/
#ifndef _LCD2P_H_
	#define _LCD2P_H_

/*** Working Frequency ***/
#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

/*** Global Library ***/
#include <stdint.h>
#include <inttypes.h>

/*** Global Constant & Macro ***/
// ASIGN PORT PINS TO LCD (can be setup in any way)
#define LCD02P_RS 0
#define LCD02P_RW 1
#define LCD02P_EN 2
#define LCD02P_NC 3
#define LCD02P_DB4 0 // 4
#define LCD02P_DB5 1 // 5
#define LCD02P_DB6 2 // 6
#define LCD02P_DB7 3 // 7
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
	int (*printf)(const char *fmt, ...);
}LCD02P;

void lcd02p_enable(volatile uint8_t *cmdddr, volatile uint8_t *cmdpin, volatile uint8_t *cmdport, volatile uint8_t *dataddr, volatile uint8_t *datapin, volatile uint8_t *dataport);
LCD02P* lcd02p(void);

#endif
/*** EOF ***/

