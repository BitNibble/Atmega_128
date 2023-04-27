/************************************************************************
	ATMEGA128INTERRUPT
Author: Sergio Santos 
	<sergio.salazar.santos@gmail.com>
License: GNU General Public License
Hardware: ATmega128
Date: 24042023
Comment:
	Stable
************************************************************************/
#ifndef _ATMEGA128INTERRUPT_H_
	#define _ATMEGA128INTERRUPT_H_

/*** Global Library ***/
#include <inttypes.h>

/*** Global Constant & Macro ***/
#ifndef GLOBAL_INTERRUPT_ENABLE
	#define GLOBAL_INTERRUPT_ENABLE 7
#endif

/*** Global Variable ***/
struct ntrrpt{
	void (*set)(uint8_t channel, uint8_t sense);
	void (*off)(uint8_t channel);
	void (*on)(uint8_t channel);
	uint8_t (*reset_status)(void);
};
typedef struct ntrrpt INTERRUPT;

/*** Global Header ***/
INTERRUPT INTERRUPTenable(void);

#endif
/***EOF***/

