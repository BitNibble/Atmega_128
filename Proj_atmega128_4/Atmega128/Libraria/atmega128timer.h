/************************************************************************
	ATMEGA128TIMER
Author: Sergio Santos 
	<sergio.salazar.santos@gmail.com>
License: GNU General Public License
Hardware: ATmega128
Date: 24042023
Comment:
	Stable
************************************************************************/
/*** Preamble Inic ***/
#ifndef _ATMEGA128TIMER_H_
	#define _ATMEGA128TIMER_H_

/*** Global Library ***/
#include <inttypes.h>

/*** Constant & Macro ***/
#ifndef GLOBAL_INTERRUPT_ENABLE
	#define GLOBAL_INTERRUPT_ENABLE 7
#endif

/*** Global Variable ***/
struct tmr_cntr0{
	// prototype pointers
	void (*compoutmode)(unsigned char compoutmode);
	void (*compoutmodeA)(unsigned char compoutmode);
	void (*compoutmodeB)(unsigned char compoutmode);
	void (*compare)(unsigned char compare);
	void (*compareA)(unsigned char compare);
	void (*compareB)(unsigned char compare);
	uint8_t (*start)(unsigned int prescaler);
	uint8_t (*stop)(void);
};
typedef struct tmr_cntr0 TIMER_COUNTER0;

struct tmr_cntr1{
	// prototype pointers
	void (*compoutmodeA)(unsigned char compoutmode);
	void (*compoutmodeB)(unsigned char compoutmode);
	void (*compoutmodeC)(unsigned char compoutmode);
	void (*compareA)(uint16_t compareA);
	void (*compareB)(uint16_t compareB);
	void (*compareC)(uint16_t compareC);
	uint8_t (*start)(unsigned int prescaler);
	uint8_t (*stop)(void);
};
typedef struct tmr_cntr1 TIMER_COUNTER1;

struct tmr_cntr2{
	// prototype pointers
	void (*compoutmode)(unsigned char compoutmode);
	void (*compoutmodeA)(unsigned char compoutmode);
	void (*compoutmodeB)(unsigned char compoutmode);
	void (*compare)(unsigned char compare);
	void (*compareA)(unsigned char compare);
	void (*compareB)(unsigned char compare);
	uint8_t (*start)(unsigned int prescaler);
	uint8_t (*stop)(void);
};
typedef struct tmr_cntr2 TIMER_COUNTER2;

struct tmr_cntr3{
	// prototype pointers
	void (*compoutmodeA)(unsigned char compoutmode);
	void (*compoutmodeB)(unsigned char compoutmode);
	void (*compoutmodeC)(unsigned char compoutmode);
	void (*compareA)(uint16_t compareA);
	void (*compareB)(uint16_t compareB);
	void (*compareC)(uint16_t compareC);
	uint8_t (*start)(unsigned int prescaler);
	uint8_t (*stop)(void);
};
typedef struct tmr_cntr3 TIMER_COUNTER3;

/*** Global Header ***/
TIMER_COUNTER0 TIMER_COUNTER0enable(unsigned char wavegenmode, unsigned char interrupt);
TIMER_COUNTER1 TIMER_COUNTER1enable(unsigned char wavegenmode, unsigned char interrupt);
TIMER_COUNTER2 TIMER_COUNTER2enable(unsigned char wavegenmode, unsigned char interrupt);
TIMER_COUNTER3 TIMER_COUNTER3enable(unsigned char wavegenmode, unsigned char interrupt);

#endif
/***EOF***/

