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

/*** Constant & Macro ***/
#define N_DELAY_MASK 0X0F
#define N_DELAY 16

/*** Global Variable ***/
struct WATCHTIME{
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint16_t seconds;
};
typedef struct{
	struct WATCHTIME* time;
	uint8_t (*start_delay)(uint8_t n_delay, uint16_t seconds);
	void (*increment)(void);
	void (*decrement)(void);
	char* (*show)(void);
}WATCH;

/*** Global Header ***/
WATCH watch_enable(void);

#endif

/***EOF***/

