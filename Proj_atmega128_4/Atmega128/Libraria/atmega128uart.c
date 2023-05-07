/*************************************************************************
	ATMEGA128UART
Author: Sergio Santos 
	<sergio.salazar.santos@gmail.com>
License: GNU General Public License
Hardware: ATmega128
Date: 24042023
Comment:
	Stable
*************************************************************************/
/*** File Library ***/
#include "atmega128mapping.h"
#include "buffer.h"
//#include <stdarg.h>
#include <util/delay.h>

/*** File Constant & Macro ***/
#if ( ( UART_RX_BUFFER_SIZE ) >= ( RAMEND-0x60 ) )
	#error "size of UART_RX_BUFFER_SIZE + UART_TX_BUFFER_SIZE larger than size of SRAM"
#endif
#define BAUD_RATE_ASYNCHRONOUS_NORMAL_MODE(baudRate, xtalCpu) ((xtalCpu) / ((baudRate) * 16l) - 1)
#define BAUD_RATE_ASYNCHRONOUS_DOUBLE_SPEED(baudRate, xtalCpu) (((xtalCpu) / ((baudRate) * 8l) -1) | 0x8000)
#define BAUD_RATE_SYNCHRONOUS_MASTER_MODE(baudRate, xtalCpu) (((xtalCpu) / ((baudRate) * 2l) -1)))
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__) 
	// ATmega with two USART
	#define ATMEGA_128_USART
	#define UART_FRAME_ERROR		0x0800              /* Framing Error by UART        */
	#define UART_OVERRUN_ERROR		0x0400              /* Overrun condition by UART    */
	#define UART_BUFFER_OVERFLOW	0x0200              /* receive ring buffer overflow */
	#define UART_NO_DATA			0x0100              /* no receive data available    */
	#define UART0_RECEIVE_INTERRUPT   USART0_RX_vect
	#define UART1_RECEIVE_INTERRUPT   USART1_RX_vect
	#define UART0_TRANSMIT_INTERRUPT  USART0_UDRE_vect
	#define UART1_TRANSMIT_INTERRUPT  USART1_UDRE_vect
#else
	#error "Not Atmega 128"
#endif

/*** File Variable ***/
ATMEGA128 uartmega128;
BUFF rx0buff;
BUFF rx1buff;

UARTvar UART0_Rx;
UARTvar UART0_RxBuf[UART0_RX_BUFFER_SIZE + 1];
uint8_t UART0_LastRxError;
UARTvar UART1_Rx;
UARTvar UART1_RxBuf[UART1_RX_BUFFER_SIZE + 1];
uint8_t UART1_LastRxError;

/*** File Header ***/
UARTvar uart0_read(void);
UARTvar uart0_getch(void);
UARTvar* uart0_gets(void);
void uart0_rxflush(void);
void uart0_write(UARTvar data);
void uart0_putch(UARTvar c);
void uart0_puts(UARTvar* s);

UARTvar uart1_read(void);
UARTvar uart1_getch(void);
UARTvar* uart1_gets(void);
void uart1_rxflush(void);
void uart1_write(UARTvar data);
void uart1_putch(UARTvar c);
void uart1_puts(UARTvar* s);

/*** Procedure & Function ***/
UART0 UART0enable(unsigned int baudrate, unsigned int FDbits, unsigned int Stopbits, unsigned int Parity )
{
	UART0 uart;
	uartmega128 = ATMEGA128enable();
	rx0buff = BUFFenable( UART0_RX_BUFFER_SIZE, UART0_RxBuf );
	uart.ubrr = baudrate;
	// Vtable
	uart.read = uart0_read;
	uart.getch = uart0_getch;
	uart.gets = uart0_gets;
	uart.rxflush = uart0_rxflush;
	uart.write = uart0_write;
	uart.putch = uart0_putch;
	uart.puts = uart0_puts;
	// Set baud rate
	if ( baudrate & 0x8000 ) 
	{
   		uartmega128.usart0.reg->ucsr0a = (1 << U2X0);  // Enable 2x speed 
   		baudrate &= ~0x8000;
   	}
	uartmega128.usart0.reg->ubrr0h = (unsigned char)(baudrate >> 8);
	uartmega128.usart0.reg->ubrr0l = (unsigned char) baudrate;
	// Enable USART receiver and transmitter and receive complete interrupt
	uartmega128.usart0.reg->ucsr0b = _BV(RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
	// Set frame format: asynchronous, 8 data, no parity, 1 stop bit
	#ifdef URSEL0
		uartmega128.usart0->ucsr0c = (1 << URSEL0) | (3 << UCSZ00);
		uart.FDbits = 8;
		uart.Stopbits = 1;
		uart.Parity = 0;
	#else
		switch(FDbits){
			case 9:
				uartmega128.usart0.reg->ucsr0b |= (1 << UCSZ02);
				uartmega128.usart0.reg->ucsr0c |= (3 << UCSZ00);
				uart.FDbits = 9;
			break;
			case 8:
				uartmega128.usart0.reg->ucsr0b &= ~(1 << UCSZ02);
				uartmega128.usart0.reg->ucsr0c |= (3 << UCSZ00);
				uart.FDbits = 8;
			break;
			case 7:
				uartmega128.usart0.reg->ucsr0b &= ~(1 << UCSZ02);
				uartmega128.usart0.reg->ucsr0c |= (1 << UCSZ01);
				uartmega128.usart0.reg->ucsr0c &= ~(1 << UCSZ00);
				uart.FDbits = 7;
			break;
			case 6:	
				uartmega128.usart0.reg->ucsr0b &= ~(1 << UCSZ02);
				uartmega128.usart0.reg->ucsr0c &= ~(1 << UCSZ01);
				uartmega128.usart0.reg->ucsr0c |= (1 << UCSZ00);
				uart.FDbits = 6;
			break;
			case 5:	
				uartmega128.usart0.reg->ucsr0b &= ~(1 << UCSZ02);
				uartmega128.usart0.reg->ucsr0c &= ~(3 << UCSZ00);
				uart.FDbits = 5;
			break;
			default:
				uartmega128.usart0.reg->ucsr0b &= ~(1 << UCSZ02);
				uartmega128.usart0.reg->ucsr0c |= (3 << UCSZ00);
				uart.FDbits = 8;
			break;
		}
		switch(Stopbits){
			case 1:
				uartmega128.usart0.reg->ucsr0c &= ~(1 << USBS0);
				uart.Stopbits = 1;
			break;
			case 2:
				uartmega128.usart0.reg->ucsr0c |= (1 << USBS0);
				uart.Stopbits = 2;
			break;	
			default:
				uartmega128.usart0.reg->ucsr0c &= ~(1 << USBS0);
				uart.Stopbits = 1;
			break;
		}
		switch(Parity){
			case 0:
				uartmega128.usart0.reg->ucsr0c &= ~(3 << UPM00);
				uart.Parity = 0;
			break;
			case 2:
				uartmega128.usart0.reg->ucsr0c |= (1 << UPM01);
				uartmega128.usart0.reg->ucsr0c &= ~(1 << UPM00);
				uart.Parity = 2;
			break;
			case 3:
				uartmega128.usart0.reg->ucsr0c |= (3 << UPM00);
				uart.Parity = 3;
			break;	
			default:
				uartmega128.usart0.reg->ucsr0c &= ~(3 << UPM00);
				uart.Parity = 0;
			break;
		}
	#endif
	uartmega128.cpu.reg->sreg |= _BV(GLOBAL_INTERRUPT_ENABLE);
	return uart;
}
UARTvar uart0_read(void)
{
	UARTvar c;
	c = UART0_Rx;
	UART0_Rx = 0;
	_delay_ms(1);
	return c;
}
UARTvar uart0_getch(void)
{
	return uart0_read();
}
UARTvar* uart0_gets(void)
{
	return rx0buff.raw(&rx0buff.pos);
}
void uart0_rxflush(void)
{
	rx0buff.flush(&rx0buff.pos);
}
void uart0_write(UARTvar data)
{
	uartmega128.usart0.reg->udr0 = data;
	uartmega128.usart0.reg->ucsr0b |= _BV(UDRIE0);
	_delay_ms(1);
}
void uart0_putch(UARTvar c)
{
	uart0_write(c);
}
void uart0_puts(UARTvar* s)
{
	char tmp;
	while(*s){
		tmp = *(s++);
		uart0_putch(tmp);
	}
}

// these functions are only for ATmegas with two USART
UART1 UART1enable(unsigned int baudrate, unsigned int FDbits, unsigned int Stopbits, unsigned int Parity )
{
	UART1 uart;
	uartmega128 = ATMEGA128enable();
	rx1buff = BUFFenable( UART1_RX_BUFFER_SIZE, UART1_RxBuf );
	uart.ubrr = baudrate;
	// FUNCTION POINTER
	uart.read = uart1_read;
	uart.getch = uart1_getch;
	uart.gets = uart1_gets;
	uart.rxflush = uart1_rxflush;
	uart.write = uart1_write;
	uart.putch = uart1_putch;
	uart.puts = uart1_puts;
	// Set baud rate
	if ( baudrate & 0x8000 ) {
		uartmega128.usart1.reg->ucsr1a = (1 << U2X1);  // Enable 2x speed 
		baudrate &= ~0x8000;
	}
	uartmega128.usart1.reg->ubrr1h = (unsigned char)(baudrate >> 8);
	uartmega128.usart1.reg->ubrr1l = (unsigned char) baudrate;
	// Enable USART receiver and transmitter and receive complete interrupt
	uartmega128.usart1.reg->ucsr1b = _BV(RXCIE1) | (1 << RXEN1)|(1 << TXEN1);
	// Set frame format: asynchronous, 8data, no parity, 1stop bit
	#ifdef URSEL1
		UCSR1C = (1 << URSEL1) | (3 << UCSZ10);
		uart.FDbits = 8;
		uart.Stopbits = 1;
		uart.Parity = 0;
	#else
		// Parameters
		switch(FDbits){
			case 9:
				uartmega128.usart1.reg->ucsr1b |= (1 << UCSZ12);
				uartmega128.usart1.reg->ucsr1c |= (3 << UCSZ10);
				uart.FDbits = 9;
			break;
			case 8:
				uartmega128.usart1.reg->ucsr1b &= ~(1 << UCSZ12);
				uartmega128.usart1.reg->ucsr1c |= (3 << UCSZ10);
				uart.FDbits = 8;
			break;
			case 7:
				uartmega128.usart1.reg->ucsr1b &= ~(1 << UCSZ12);
				uartmega128.usart1.reg->ucsr1c |= (1 << UCSZ11);
				uartmega128.usart1.reg->ucsr1c &= ~(1 << UCSZ10);
				uart.FDbits = 7;
			break;
			case 6:
				uartmega128.usart1.reg->ucsr1b &= ~(1 << UCSZ12);
				uartmega128.usart1.reg->ucsr1c &= ~(1 << UCSZ11);
				uartmega128.usart1.reg->ucsr1c |= (1 << UCSZ10);
				uart.FDbits = 6;
			break;
			case 5:
				uartmega128.usart1.reg->ucsr1b &= ~(1 << UCSZ12);
				uartmega128.usart1.reg->ucsr1c &= ~(3 << UCSZ10);
				uart.FDbits = 5;
			break;
			default:
				uartmega128.usart1.reg->ucsr1b &= ~(1 << UCSZ12);
				uartmega128.usart1.reg->ucsr1c |= (3 << UCSZ10);
				uart.FDbits = 8;
			break;
		}
		switch(Stopbits){
			case 1:
				uartmega128.usart1.reg->ucsr1c &= ~(1 << USBS1);
				uart.Stopbits = 1;
			break;
			case 2:
				uartmega128.usart1.reg->ucsr1c |= (1 << USBS1);
				uart.Stopbits = 2;
			break;
			default:
				uartmega128.usart1.reg->ucsr1c &= ~(1 << USBS1);
				uart.Stopbits = 1;
			break;
		}
		switch(Parity){
			case 0: // NONE
				uartmega128.usart1.reg->ucsr1c &= ~(3 << UPM10);
				uart.Parity = 0;
			break;
			case 2: // EVEN
				uartmega128.usart1.reg->ucsr1c |= (1 << UPM11);
				uartmega128.usart1.reg->ucsr1c &= ~(1 << UPM10);
				uart.Parity = 2;
			break;
			case 3: // ODD
				uartmega128.usart1.reg->ucsr1c |= (3 << UPM10);
				uart.Parity = 3;
			break;
			default:
				uartmega128.usart1.reg->ucsr1c &= ~(3 << UPM10);
				uart.Parity = 0;
			break;
		}
	#endif
	uartmega128.cpu.reg->sreg |= _BV(GLOBAL_INTERRUPT_ENABLE);
	return uart;
}
UARTvar uart1_read(void)
{
	UARTvar c;
	c = UART1_Rx;
	UART1_Rx = 0;
	_delay_ms(1);
	return c;
}
UARTvar uart1_getch(void)
{
	return uart1_read();
}
UARTvar* uart1_gets(void)
{
	return rx1buff.raw(&rx1buff.pos);
}
void uart1_rxflush(void)
{
	rx1buff.flush(&rx1buff.pos);
}
void uart1_write(UARTvar data)
{
	uartmega128.usart1.reg->udr1 = data;
	uartmega128.usart1.reg->ucsr1b |= _BV(UDRIE1);
	_delay_ms(1);
}
void uart1_putch(UARTvar c)
{
	uart1_write(c);
}
void uart1_puts(UARTvar* s)
{
	char tmp;
	while(*s){
		tmp = *(s++);
		uart1_putch(tmp);
	}
}

/*** File Interrupt ***/
ISR(UART0_RECEIVE_INTERRUPT)
{
	unsigned char bit9;
	unsigned char usr;
	
	usr  = uartmega128.usart0.reg->ucsr0a;
	bit9 = uartmega128.usart0.reg->ucsr0b;
	bit9 = 0x01 & (bit9 >> 1);
	
	UART0_LastRxError = (usr & (_BV(FE0) | _BV(DOR0)));
	
	UART0_Rx = uartmega128.usart0.reg->udr0;
	rx0buff.push(&rx0buff.pos, UART0_Rx);
}
ISR(UART0_TRANSMIT_INTERRUPT)
{
	uartmega128.usart0.reg->ucsr0b &= ~_BV(UDRIE0);
}

SIGNAL(UART1_RECEIVE_INTERRUPT)
{
	unsigned char bit9;
	unsigned char usr;
	
	usr  = uartmega128.usart1.reg->ucsr1a;
	bit9 = uartmega128.usart1.reg->ucsr1b;
	bit9 = 0x01 & (bit9 >> 1);
 	   
	UART1_LastRxError = (usr & (_BV(FE1) | _BV(DOR1)));
	
	UART1_Rx = uartmega128.usart1.reg->udr1;
	rx1buff.push(&rx1buff.pos, UART1_Rx);
}
SIGNAL(UART1_TRANSMIT_INTERRUPT)
{
	uartmega128.usart1.reg->ucsr1b &= ~_BV(UDRIE1);
}

/***EOF***/

