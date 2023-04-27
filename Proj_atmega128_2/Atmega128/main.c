/************************************************************************
Title: SUNTRACK.c
Author: Sergio Manuel Santos 
	<sergio.salazar.santos@gmail.com>
License: GNU General Public License
File: $Id: MAIN,v 1.8.2.1 21/10/2020 Exp $
Software: Atmel Studio 7 (ver 7.0.129)
Hardware: Atmega128 by ETT ET-BASE
	-PORTA LCD 16X2
	-PORTE Keyboard 4X4
	-PF0 Sensor LDR
	-PB6 Servo Motor
	-PORTD RTC and UART1->FTDI chip->PC Putty or HC-05 115200kb 8 1 n.
	-PORTG HC595
Date: 17/11/2022
Comment:
	Nice
************************************************************************/
/*** Working Frequency ***/
#define F_CPU 16000000UL

/*** Compiler ***/
#if (__GNUC__ * 100 + __GNUC_MINOR__) < 304
#error "This library requires AVR-GCC 3.4 or later, update to newer AVR-GCC compiler !"
#endif

/*** File library ***/
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <inttypes.h>
//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdarg.h>
//#include <math.h>
#include "atmega128mapping.h"
#include "function.h"
#include "lcd.h"
#include "pcf8563rtc.h"
#include "keypad.h"
#include "74hc595.h"
#include "atcommands.h"

/*** File Constant & Macro ***/
#define TRUE 1
#define ZERO 0
#define Min 500     // 450 PWM servo motor
#define Max 2350    // 2450 PWM servo motor
#define SMIN -200
#define SMAX +200

/*** File variable ***/
struct time tm; // time struct RTC
struct date dt; // date struct RTC
ATMEGA128 m128;
HC595 shift;
UART1 uart;
FUNC function;
PCF8563RTC rtc;
uint8_t count=0; // 1Hz
uint8_t increment=0; // 1Hz
char* ptr=NULL; // pointing to analog reading string
char* uartreceive=NULL; // pointing to Rx Buffer
unsigned int signal;
uint16_t counter1;

/*** File Header ***/
void PORTINIT(void);
ISR(TIMER0_COMP_vect);
ISR(TIMER2_COMP_vect);

/**** Procedure & Function ****/
int main(void)
{
// Inic File mcu var
m128 = ATMEGA128enable(); // Image enable

// Inic procedure & function
PORTINIT(); // Inic Ports
// Inic File var
function= FUNCenable(); // Function Library
LCD0 lcd0 = LCD0enable(&DDRA,&PINA,&PORTA); // LCD Display 4X20
KEYPAD keypad = KEYPADenable(&DDRE,&PINE,&PORTE); // Keyboard
ANALOG analog = m128.adc.enable(1, 128, 1, 0); // Channel 0 for Position
TIMER_COUNTER0 timer0 = m128.tc0.enable(2,2); // 1Hz to HC595
TIMER_COUNTER1 timer1 = m128.tc1.enable(9,0); // PWM Positioning
TIMER_COUNTER2 timer2 = m128.tc2.enable(2,2);
rtc = PCF8563RTCenable(16); // RTC with I2C
shift = HC595enable(&DDRG,&PORTG,2,0,1);
uart = m128.usart1.enable(25,8,1,NONE); // UART 103 para 9600, 68 para 14400, 25 para 38400, 8 para 115200



// local var
char Menu = '1'; // Main menu selector
int AT = 0;
uint16_t adcvalue; // analog reading
char str[6]="0"; // analog vector
int16_t mvalue=0; // manual position reading
int16_t m_value; // manual positioning
char mstr[6]="0"; // manual position vector
char tstr[6]; // time vector
char cal='0'; // Sub Menu for setting up date and time
uint16_t set;
ptr=str;
uint16_t positionhour = 12;
signal = 0;
counter1 = 0;
char* ATmsg = NULL;
uint8_t uartoneshot = 0;
char uartmsg[UART1_RX_BUFFER_SIZE];

//uint16_t var = RAMEND;
// RAMSTART

// Parameters timers
timer0.compare(249);
timer0.start(64);
timer1.compoutmodeB(2);
timer1.compareA(20000);
timer1.start(8);
timer2.start(0);



// rtc setup pin
rtc.SetClkOut(1, 2); // oscillate pin at 1 sec

// TODO:: Please write your application code
while(TRUE){
	// Preamble [INPUT]
	lcd0.reboot();
	keypad.read();
	if(uartoneshot){ uartoneshot = 0; uart.rxflush();} // the matrix
	uartreceive=uart.gets(); //UART1
	if(uart.getch() == '\n'){ strcpy(uartmsg,uartreceive); uartoneshot = 1;}
	// RTC
	tm=rtc.GetTime();
	dt=rtc.GetDate();
	// Reading analog
	adcvalue=analog.read(0);
	strcpy(str,function.i16toa(adcvalue));
	// Reading input
	lcd0.gotoxy(1,0);
	lcd0.putch(':');
	lcd0.string_size(keypad.data().print, 6);
	// ENTRY END
	
	// catch message
	if(!strcmp(uartreceive,"Connect\r\n")){Menu='6';lcd0.clear();}
	if(!strcmp(uartreceive,"Connected\r\n")){Menu='6';lcd0.clear();}
	if(!strcmp(uartreceive,"ERROR\r\nConnected\r\n")){Menu='6';lcd0.clear();}
	
	// MENU SELECTOR	
	switch(Menu){
		// MENU 1
		case '1': // Main Program Menu
			if(!strcmp(keypad.data().string,"A")){Menu='7';keypad.flush();lcd0.clear();break;}
			if(!strcmp(keypad.data().string,"B")){Menu='2';keypad.flush();lcd0.clear();break;}
			// Set Position
			if(positionhour>5 && positionhour<21)
				timer1.compareB(function.trimmer(positionhour,2,23,Min,Max)+function.trimmer(adcvalue,0,1023,SMIN,SMAX));
			else
				timer1.compareB(function.trimmer(6,2,23,Min,Max));
			lcd0.gotoxy(0,0);
			lcd0.string_size("S:",3);
			lcd0.string_size(str,5);
			lcd0.gotoxy(0,8);
			lcd0.string_size(function.ui16toa(rtc.bcd2dec(dt.days)),2);
			lcd0.putch(':');
			lcd0.string_size(function.ui16toa(rtc.bcd2dec(dt.century_months)),2);
			lcd0.putch(':');
			lcd0.string_size(function.ui16toa(rtc.bcd2dec(dt.years)),2);
			lcd0.gotoxy(1,8);
			// set hour for positioning
			positionhour=rtc.bcd2dec(tm.hours);
			lcd0.string_size(function.ui16toa(positionhour),2);
			lcd0.putch(':');
			lcd0.string_size(function.ui16toa(rtc.bcd2dec(tm.minutes)),2);
			lcd0.putch(':');
			lcd0.string_size(function.ui16toa(rtc.bcd2dec(tm.VL_seconds)),2);
		break;
		// MENU 2
		case '2': // Manual position override 
			if(!strcmp(keypad.data().string,"A")){Menu='1';keypad.flush();lcd0.clear();break;}
			if(!strcmp(keypad.data().string,"B")){Menu='3';keypad.flush();lcd0.clear();break;}
			if(!strcmp(keypad.data().string,"C")){Menu='1';keypad.flush();lcd0.clear();uart.puts("Manual exit\r\n");break;}
			// Title
			lcd0.gotoxy(0,0);
			lcd0.string_size("Ang:",5);
			lcd0.string_size(mstr,3);
			
			if(keypad.data().character == KEYPADENTERKEY){
				strncpy(mstr,keypad.data().string,6);
				mvalue=function.strToInt(mstr);
				if(mvalue >=0 && mvalue <181){
					m_value=mvalue;
					timer1.compareB(function.trimmer(m_value,0,180,Min,Max));
					lcd0.gotoxy(0,12);
					lcd0.hspace(4);
				}else{
					lcd0.gotoxy(0,12);
					lcd0.string_size("err",4);
				}
				keypad.flush();
			}
			// else
			// timer1.compareB(function.trimmer(m_value,0,180,Min,Max));
			lcd0.gotoxy(1,12);
			lcd0.string_size("C-Ex",4);
		break;
		// MENU 3
		case '3': //Set Date
			if(!strcmp(keypad.data().string,"A")){Menu='2';keypad.flush();lcd0.clear();break;}
			if(!strcmp(keypad.data().string,"B")){Menu='4';keypad.flush();lcd0.clear();break;}
			if(!strcmp(keypad.data().string,"C")){Menu='1';cal='0';keypad.flush();lcd0.clear();uart.puts("Date exit\r\n");break;}
			// Menu to set RTC Time and Date
			// Calibrate Menu
			switch(cal){
				case '0': // choice
					lcd0.gotoxy(0,0);
					lcd0.string_size("1-Yr",5);
					lcd0.string_size("2-Mh",5);
					lcd0.string_size("3-Dy",5);
					lcd0.gotoxy(1,12);
					lcd0.string_size("C-Ex",4);
					if(!strcmp(keypad.data().string,"1")){cal='1';keypad.flush();lcd0.clear();}
					if(!strcmp(keypad.data().string,"2")){cal='2';keypad.flush();lcd0.clear();}
					if(!strcmp(keypad.data().string,"3")){cal='3';keypad.flush();lcd0.clear();}
					// if(keypad.data().character=='1'){cal='1';keypad.flush();lcd0.clear();}
				break;
				
				case '1': // YEAR
					lcd0.gotoxy(1,12);
					lcd0.string_size("C-Ex",4);
					// YEAR
					if(keypad.data().character == KEYPADENTERKEY){
						strcpy(tstr,keypad.data().string);
						set = function.strToInt(tstr);
						if(set >=0 && set <100){
							rtc.SetYear(rtc.bintobcd(set));
							cal='0';
						}else{
							strcpy(tstr,"err");
						}
						keypad.flush();
					}
				break;
				
				case '2': // MONTH
					lcd0.gotoxy(1,12);
					lcd0.string_size("C-Ex",4);
					// MONTH
					if(keypad.data().character == KEYPADENTERKEY){
						strcpy(tstr,keypad.data().string);
						set = function.strToInt(tstr);
						if(set >=0 && set <13){
							rtc.SetMonth(rtc.bintobcd(set));
							cal='0';
						}else{
							strcpy(tstr,"err");
						}
						keypad.flush();
					}
				break;
				
				case '3': // DAY
					lcd0.gotoxy(1,12);
					lcd0.string_size("C-Ex",4);
					// DAY
					if(keypad.data().character == KEYPADENTERKEY){
						strcpy(tstr,keypad.data().string);
						set = function.strToInt(tstr);
						if(set >=0 && set <32){
							rtc.SetDay(rtc.bintobcd(set));
							cal='0';
						}else{
							strcpy(tstr,"err");
						}
						keypad.flush();
					}
				break;
				
				default:
					cal='0';
				break;
			};
		break;
		// MENU 4
		case '4': //Set Time
			if(!strcmp(keypad.data().string,"A")){Menu='3';keypad.flush();lcd0.clear();break;}
			if(!strcmp(keypad.data().string,"B")){Menu='5';keypad.flush();lcd0.clear();break;}
			if(!strcmp(keypad.data().string,"C")){Menu='1';cal='0';keypad.flush();lcd0.clear();uart.puts("Time exit\r\n");break;}
			// Menu to set RTC Time and Date
			// Calibrate Menu
			switch(cal){
				case '0': // choice
					lcd0.gotoxy(0,0);
					lcd0.string_size("1-Hr",5);
					lcd0.string_size("2-Mn",5);
					lcd0.string_size("3-Sc",5);
					lcd0.gotoxy(1,12);
					lcd0.string_size("C-Ex",4);
					if(!strcmp(keypad.data().string,"1")){cal='1';keypad.flush();lcd0.clear();}
					if(!strcmp(keypad.data().string,"2")){cal='2';keypad.flush();lcd0.clear();}
					if(!strcmp(keypad.data().string,"3")){cal='3';keypad.flush();lcd0.clear();}
					// if(keypad.data().character=='1'){cal='1';keypad.flush();lcd0.clear();}
				break;
				
				case '1': // HOUR
					lcd0.gotoxy(1,12);
					lcd0.string_size("C-Ex",4);
					// HOUR
					if(keypad.data().character == KEYPADENTERKEY){
						strcpy(tstr,keypad.data().string);
						set = function.strToInt(tstr);
						if(set >=0 && set <24){
							rtc.SetHour(rtc.bintobcd(set));
							cal='0';
						}else{
							strcpy(tstr,"err");
						}
						keypad.flush();
					}
				break;
				
				case '2': // MINUTE
					lcd0.gotoxy(1,12);
					lcd0.string_size("C-Ex",4);
					// MINUTE
					if(keypad.data().character == KEYPADENTERKEY){
						strcpy(tstr,keypad.data().string);
						set = function.strToInt(tstr);
						if(set >=0 && set <60){
							rtc.SetMinute(rtc.bintobcd(set));
							cal='0';
						}else{
							strcpy(tstr,"err");
						}
						keypad.flush();
					}
				break;
				
				case '3': // SECOND
					lcd0.gotoxy(1,12);
					lcd0.string_size("C-Ex",4);
					// SECOND
					if(keypad.data().character == KEYPADENTERKEY){
						strcpy(tstr,keypad.data().string);
						set = function.strToInt(tstr);
						if(set >=0 && set <60){
							rtc.SetSecond(rtc.bintobcd(set));
							cal='0';
						}else{
							strcpy(tstr,"err");
						}
						keypad.flush();
					}
				break;
				default:
					cal='0';
				break;
			};
		break;
		
		// MENU 5
		case '5': // Output
			if(!strcmp(keypad.data().string,"A")){Menu='4';keypad.flush();lcd0.clear();break;}
			if(!strcmp(keypad.data().string,"B")){Menu='6';keypad.flush();lcd0.clear();break;}
			if(!strcmp(keypad.data().string,"C")){Menu='1';cal='0';keypad.flush();lcd0.clear();uart.puts("Output exit\r\n");break;}
			// Title
			lcd0.gotoxy(0,0);
			lcd0.string_size("Out:",5);
			
			lcd0.string_size(mstr,3);
			if(keypad.data().character == KEYPADENTERKEY){
				strncpy(mstr,keypad.data().string,6);
				mvalue=function.strToInt(mstr);
				if(mvalue >=0 && mvalue <16){
					// PORTC = mvalue;
					m128.portc.reg->port = mvalue;
					lcd0.gotoxy(0,12);
					lcd0.hspace(4);
				}else{
					lcd0.gotoxy(0,12);
					lcd0.string_size("err",4);
				}
				keypad.flush();
			}
			lcd0.gotoxy(1,12);
			lcd0.string_size("C-Ex",4);
		break;
		// MENU 6
		case '6': // HC-05 Communication
			if(!strcmp(keypad.data().string,"A")){Menu='5';keypad.flush();lcd0.clear();break;}
			if(!strcmp(keypad.data().string,"B")){Menu='7';keypad.flush();lcd0.clear();break;}
			// if(!strcmp(keypad.data().string,"C")){Menu='1';cal='0';keypad.flush();lcd0.clear();break;}
			if(!strcmp(keypad.data().string,"C")){Menu='1';cal='0';keypad.flush();lcd0.clear();uart.puts("Communication exit\r\n");break;}
			// Title
			lcd0.gotoxy(0,0);
			lcd0.string_size("HC:",3);
			
			if(!strcmp(uartreceive,"position\r\n")){
				uart.puts("> ");
				uart.puts("analog Reading: ");
				uart.puts(ptr);
				uart.puts("\r\n");
				uart.rxflush();
			}
			if(!strcmp(uartreceive,"time\r\n")){
				// uart.putc('>');uart.puts("analog Reading: ");uart.puts(ptr);uart.puts("\r\n");
				uart.puts(function.ui16toa(rtc.bcd2dec(tm.hours)));
				uart.putch(':');
				uart.puts(function.ui16toa(rtc.bcd2dec(tm.minutes)));
				uart.putch(':');
				uart.puts(function.ui16toa(rtc.bcd2dec(tm.VL_seconds)));
				uart.puts("\r\n");
				uart.rxflush();
			}
			if(!strcmp(uartreceive,"led 1 on\r\n")){
				m128.portc.reg->port |= 1;
			}
			if(!strcmp(uartreceive,"led 1 off\r\n")){
				m128.portc.reg->port &= ~1;
			}
			if(!strcmp(uartreceive,"led 2 on\r\n")){
				m128.portc.reg->port |= 2;
			}
			if(!strcmp(uartreceive,"led 2 off\r\n")){
				m128.portc.reg->port &= ~2;
			}
			if(!strcmp(uartreceive,"led 3 on\r\n")){
				m128.portc.reg->port |= 4;
			}
			if(!strcmp(uartreceive,"led 3 off\r\n")){
				m128.portc.reg->port &= ~4;
			}
			if(!strcmp(uartreceive,"led 4 on\r\n")){
				m128.portc.reg->port |= 8;
			}
			if(!strcmp(uartreceive,"led 4 off\r\n")){
				m128.portc.reg->port &= ~8;
			}
			if(!strcmp(uartreceive,"all on\r\n")){
				m128.portc.reg->port |= 15;
			}
			if(!strcmp(uartreceive,"all off\r\n")){
				m128.portc.reg->port &= ~15;
			}
			if(!strcmp(uartreceive,"Disconnect\r\n")){
				Menu = '1';
			}
				
			lcd0.gotoxy(0,3);
			lcd0.string_size(uartmsg,13);

			lcd0.gotoxy(1,12);
			lcd0.string_size("C-Ex",4);
		break;			
		// MENU 7
		case '7': // HC-05 AT
			if(!strcmp(keypad.data().string,"A")){Menu='6';keypad.flush();lcd0.clear();break;}
			if(!strcmp(keypad.data().string,"B")){Menu='1';keypad.flush();lcd0.clear();break;}
			// if(!strcmp(keypad.data().string,"C")){Menu='1';cal='0';keypad.flush();lcd0.clear();break;}
			if(!strcmp(keypad.data().string,"C")){Menu='1';cal='0';keypad.flush();lcd0.clear();uart.puts("AT exit\r\n");break;}
			// Title
			lcd0.gotoxy(0,0);
			lcd0.string_size("AT:",3);
			// COMMANDS
			if(keypad.data().character == KEYPADENTERKEY){
				AT = function.strToInt(keypad.data().string);
				ATmsg = BT05ATcmd(AT);
				if(ATmsg)
					uart.puts(BT05ATcmd(AT));
				keypad.flush();
			}
			AT = 0;
			
			lcd0.gotoxy(0,3);
			lcd0.string_size(uartmsg,13);
				
			lcd0.gotoxy(1,12);
			lcd0.string_size("C-Ex",4);
		break;
		
		default:
			Menu='1';
		break;
	};
	
	switch(signal)
	{
		case 1:
			lcd0.gotoxy(1,7);
			lcd0.string_size("T",1);
			signal = 0;
		break;
		default:
		break;	
	};
}// WHILE
}// MAIN

void PORTINIT(void)
{
	// INPUT
	m128.portf.reg->ddr = 0x00;
	m128.portf.reg->port = 0x0F;
	// OUTPUT
	m128.portb.reg->ddr |= (1<<5) | (1<<6) | (1<<7);
	// OUTPUT PULLUP
	m128.portc.reg->ddr = 0xFF;
	m128.portc.reg->port = 0x00;
}

/*** File Interrupt ***/
ISR(TIMER0_COMP_vect) // 1Hz and usart Tx
{
	uint8_t Sreg;
	Sreg = m128.cpu.reg->sreg;
	m128.cpu.reg->sreg &= ~(1<<7);
	if(count>59){ //59 -> 1Hz
		increment++;
		if((increment & 0x0F) < 8){
			shift.bit(0);
			shift.out();
		}else{
			shift.bit(1);
			shift.out();
		}
		count=0;
	}else
		count++;
	m128.cpu.reg->sreg = Sreg;
}

ISR(TIMER2_COMP_vect)
{
	uint8_t Sreg;
	Sreg = m128.cpu.reg->sreg;
	m128.cpu.reg->sreg &= ~(1<<7);
	
	if(counter1 > 1000){
		// signal = 1;
		counter1=0;
	}
	counter1++;
	
	m128.cpu.reg->sreg = Sreg;
}

/***EOF***/

/**************************** Comment: ******************************
1º Sequence
2º Scope
3º Pointers and Variables
4º Casting
********************************************************************/

