/**********************************************************************
	ATMEGA128USART0
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date:     04/09/2025
**********************************************************************/
#ifndef _ATMEGA128USART0_H_
	#define _ATMEGA128USART0_H_

/*** Library ***/
#include "atmega128.h"

/*** Constant & Macro ***/
#ifndef UART0_RX_BUFFER_SIZE
	#define UART0_RX_BUFFER_SIZE 32
#endif

#define UARTvar char

#if ( ( UART0_RX_BUFFER_SIZE ) >= ( RAMEND - 0x60 ) )
	#error "size of UART_RX_BUFFER_SIZE larger than size of SRAM"
#endif

#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)
	#define UART0_RECEIVE_COMPLETE   USART0_RX_vect
	#define UART0_DATA_REGISTER_EMPTY  USART0_UDRE_vect
	#define	UART0_TRANSMIT_COMPLETE	USART0_TX_vect
#else
	#error "Not Atmega 128"
#endif

#define NONE 0
#define EVEN 2
#define ODD 3
#define UART_FRAME_ERROR		0x0800              /* Framing Error by UART        */
#define UART_OVERRUN_ERROR		0x0400              /* Overrun condition by UART    */
#define UART_BUFFER_OVERFLOW	0x0200              /* receive ring buffer overflow */
#define UART_NO_DATA			0x0100              /* no receive data available    */

/*** Callback ***/
typedef struct {
	void (*rx)(void);
	void (*udre)(void);
	void (*tx)(void);
}USART0_Callback;

/*** Handler ***/
typedef struct {
	USART0_Callback callback;
	
	// V-table
	UARTvar (*read)(void);
	UARTvar (*getch)(void);
	UARTvar* (*gets)(void);
	void (*rxflush)(void);
	void (*write)(UARTvar data);
	void (*putch)(UARTvar c);
	void (*puts)(UARTvar* s);
}USART0_Handler;

void usart0_enable(uint32_t baud, unsigned int FDbits, unsigned int Stopbits, unsigned int Parity );
USART0_Handler* usart0(void);

/*** Procedure and Function declaration ***/
char* usart0_messageprint(USART0_Handler* uart, char* oneshot, char* msg, const char* endl);

#endif
/*** EOF ***/

