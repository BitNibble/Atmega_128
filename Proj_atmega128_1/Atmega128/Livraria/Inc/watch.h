/************************************************************************
Title:    watch.h
Author:   Sergio Manuel Santos <sergio.salazar.santos@gmail.com>
File:     $Id: watch.h,v 0.2 2017/07/01 17:00:00 Sergio Exp $
Software: AVR-GCC 4.1, AVR Libc 1.4
Hardware:
License:  GNU General Public License
Comment:
	
************************************************************************/
#ifndef _WATCH_H_
	#define _WATCH_H_

/*** Global library ***/
#include <inttypes.h>

/*** Global Constant & Macro ***/

/*** Global Variable ***/
typedef struct {
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	volatile uint32_t seconds;
} WATCH_TIME;

typedef struct {
	void (*preset)(uint8_t hour, uint8_t minute, uint8_t second);
	uint8_t (*start_delay)(uint8_t n_delay, uint32_t seconds);
	void (*increment)(void);
	char* (*show)(void);
} WATCH;

/*** Global Header ***/
WATCH WATCH_enable(void);
extern void WATCH_increment(void);
extern void WATCH_decrement(void);

#endif

/***EOF***/

