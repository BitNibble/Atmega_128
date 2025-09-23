/**********************************************************************
	ATMEGA128INTERRUPT
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: ATmega128
Date:     07/01/2024
**********************************************************************/
#ifndef _ATMEGA128INTERRUPT_H_
	#define _ATMEGA128INTERRUPT_H_

/*** Library ***/
#include "atmega128.h"

/*** Constant & Macro ***/
#if !defined(__AVR_ATmega64__) && !defined(__AVR_ATmega128__)
	#error "Not Atmega 128"
#endif

typedef struct {
	void (*int0_vect)(void);
	void (*int1_vect)(void);
	void (*int2_vect)(void);
	void (*int3_vect)(void);
	void (*int4_vect)(void);
	void (*int5_vect)(void);
	void (*int6_vect)(void);
	void (*int7_vect)(void);
}EXINT0_Callback;

/*** Handler ***/
typedef struct {
	// Callback
	EXINT0_Callback callback;
	// V-table
	void (*set)(uint8_t channel, uint8_t sense);
	void (*off)(uint8_t channel);
	void (*on)(uint8_t channel);
	uint8_t (*reset_status)(void);
}EXINT0_Handler;

EXINT0_Handler* exint(void);

#endif
/*** EOF ***/

