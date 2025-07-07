/************************************************************************
	KEYPAD
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: all
Date:     27/11/2022
Update    09/01/2024
Comment:
	Stable
************************************************************************/
#ifndef _KEYPAD_H_
	#define _KEYPAD_H_

/*** Global Library ***/
#include <inttypes.h>

/*** Global Constant & Macro ***/
#define KEYPADLINES 4
#define KEYPADCOLUMNS 4
// pin number
#define KEYPADLINE_1 7
#define KEYPADLINE_2 0
#define KEYPADLINE_3 1
#define KEYPADLINE_4 3
#define KEYPADDATA_1 2
#define KEYPADDATA_2 4
#define KEYPADDATA_3 5
#define KEYPADDATA_4 6
#define KEYPADSTRINGSIZE 6
#define KEYPADENTERKEY 'D'

/*** Global Variable ***/
typedef struct {
	char character;
	char* print;
	char* string;
}keypadata;

typedef struct{
	keypadata* data;
	char (*getkey)(void);
	void (*read)(void);
	void (*flush)(void);
}KEYPAD;

/*** Global Header ***/
KEYPAD* keypad(void);
KEYPAD keypad_enable(volatile uint8_t *ddr, volatile uint8_t *pin, volatile uint8_t *port);

#endif

/***EOF***/

