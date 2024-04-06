/************************************************************************
Title: PID_TEST.c
Author: Sergio Manuel Santos
	<sergio.salazar.santos@gmail.com>
License: GNU General Public License
File: $Id: MAIN,v 1.8.2.1 21/02/2021 Exp $
Software: Atmel Studio 7 (ver 7.0.129)
Hardware: Atmega128 by ETT ET-BASE
	-PORTA LCD
	-PORTE Keyboard
	-PF0 Sensor LDR
	-PORTG HC595
Comment:

************************************************************************/
/*** Working Frequency ***/
#define F_CPU 16000000UL
/*
** library
*/
#include "atmega128mapping.h"
#include "function.h"
#include "lcd.h"
#include "pcf8563rtc.h"
#include "keypad.h"
#include "74hc595.h"
#include "znpid.h"
//#include "atmega128uart.h"
#include <util/delay.h>
#include <stdio.h> //sprintf
#include <string.h>

/*
** Constant and Macro
*/
#define TRUE 1
#define ZERO 0
#define Min 500     // 450 PWM servo motor
#define Max 2350    // 2450 PWM servo motor
#define repeat 59
/*
** Global File variable
*/
HC595 shift;
FUNC function;
ZNPID pid_1;
ZNPID pid_2;
uint8_t count=0; // 1Hz
uint8_t increment=0; // 1Hz
char* ptr=NULL; // pointing to analog reading string
uint16_t adcvalue; // analog reading
float pid_out_1;
float pid_out_2;
int32_t tmp; 
/*
** Header
*/
void PORTINIT();
/****MAIN****/
int main(void)
{
	atmega128_enable();
	PORTINIT(); // Inic Ports
	/***INICIALIZE OBJECTS***/
	function= func_enable(); // Function Library
	LCD0 lcd0 = lcd0_enable(&DDRA,&PINA,&PORTA); // LCD Display 4X20
	KEYPAD keypad = keypad_enable(&DDRE,&PINE,&PORTE); // Keyboard
	ADC0 analog = adc_enable(1, 128, 1, 0); // Channel 0 for Position
	TC0 timer0 = tc0_enable(2,2); // 1Hz to HC595
	TC1 timer1 = tc1_enable(9,0); // PWM Positioning
	shift = hc595_enable(&DDRG,&PORTG,2,0,1);
	pid_1 = znpid_enable();
	pid_2 = znpid_enable();
	/******/
	char Menu='1'; // Main menu selector
	char str[6]="0"; // analog vector
	int16_t mvalue=90; // manual position reading
	int16_t m_value; // manual positioning
	char mstr[6]="90"; // manual position vector
	ptr=str;
	/***Parameters timers***/
	timer0.compare(249);
	timer0.start(64);
	timer1.compoutmodeB(2);
	timer1.compareA(20000);
	timer1.start(8);
	pid_1.set_kc(&pid_1.par, 1);
	pid_1.set_kd(&pid_1.par, 0.2); //
	pid_1.set_ki(&pid_1.par, 0.5); // will provoke overshoot, to much acceleration limit max value and minimum value.
	pid_1.set_SP(&pid_1.par, 650);
	/***Another one woopy ti dooo***/
	pid_2.set_kc(&pid_2.par, 0.5);
	pid_2.set_kd(&pid_2.par, 1);
	pid_2.set_ki(&pid_2.par, 0.01);
	pid_2.set_SP(&pid_2.par, 125);
	/**********/
	//TODO:: Please write your application code
	while(TRUE){
		/***PREAMBLE***/
		lcd0.reboot();
		keypad.read();
		//uartreceive=uart.read();
		/***Reading input***/
		lcd0.gotoxy(3,13);
		lcd0.putch(':');
		lcd0.string_size(keypad.data->print,6);
		/***ENTRY END***/
		switch(Menu){
			/***MENU 1***/
			case '1': // Main Program Menu
				if(!strcmp(keypad.data->string,"A")){Menu='2';keypad.flush();lcd0.clear();break;}
				if(!strcmp(keypad.data->string,"B")){Menu='3';keypad.flush();lcd0.clear();break;}
					/***Reading analog***/
					adcvalue=analog.read(0);
					lcd0.gotoxy(0,0);
					lcd0.string_size("Output: ",7);
					strcpy(str,function.i16toa(adcvalue));
					lcd0.string_size(str,4);
					
					if(pid_out_1 >-1024 && pid_out_1 <1024){
						tmp=function.trimmer(pid_out_1,-1023,1023,Min,Max);
						timer1.compareB(tmp);
						lcd0.gotoxy(1,0);
						strcpy(str,function.i16toa(tmp));
						lcd0.string_size(str,4);
					}
					
					function.ftoa(35.00567, 6);
					lcd0.gotoxy(2,0);
					lcd0.string_size(str,12);
					//lcd0.gotoxy(2,0);
					//strcpy(str,function.i32toa(pid_1.PV));
					//lcd0.string_size(str,6);
					//if(pid_1.derivative>0){
						lcd0.gotoxy(3,0);
						strcpy(str,function.i32toa(pid_1.par.derivative));
						lcd0.string_size(str,6);
					//}
					
					
					// PID_1 output
					lcd0.gotoxy(0,13);
					strcpy(str,function.i32toa(pid_out_1));
					lcd0.string_size(str,6);
					
					// PID_2 output
					lcd0.gotoxy(1,13);
					strcpy(str,function.i32toa(pid_out_2));
					lcd0.string_size(str,6);
					
				break;
			/***MENU 2***/
			case '2': // Manual position override 
				if(!strcmp(keypad.data->string,"A")){Menu='3';keypad.flush();lcd0.clear();break;}
				if(!strcmp(keypad.data->string,"B")){Menu='1';keypad.flush();lcd0.clear();break;}
				if(!strcmp(keypad.data->string,"C")){Menu='1';keypad.flush();lcd0.clear();break;}
					lcd0.gotoxy(0,0);
					lcd0.string_size("Manual: ",8);
					lcd0.string_size(mstr,3);
					lcd0.gotoxy(1,0);
					lcd0.string_size("Enter Angle",11);
					if(keypad.data->character==KEYPADENTERKEY){
						strncpy(mstr,keypad.data->string,6);
						mvalue=function.strToInt(mstr);
						if(mvalue >=0 && mvalue <181){
							m_value=mvalue;
							timer1.compareB(function.trimmer(m_value,0,180,Min,Max));
							lcd0.hspace(5);
						}else{
							lcd0.string_size("  err",5);
						}
						keypad.flush();
					}
					//else
					//	timer1.compareB(function.trimmer(m_value,0,180,Min,Max));
					lcd0.gotoxy(3,0);
					lcd0.string_size("C - exit",8);
				break;
			/***MENU 3***/
			case '3': //Set Time and Date
				if(!strcmp(keypad.data->string,"A")){Menu='1';keypad.flush();lcd0.clear();break;}
				if(!strcmp(keypad.data->string,"B")){Menu='2';keypad.flush();lcd0.clear();break;}
				if(!strcmp(keypad.data->string,"C")){Menu='1';keypad.flush();lcd0.clear();break;}
					lcd0.gotoxy(0,0);
					lcd0.string_size("Not being used",19);
					
					/***Play around***/
				
				
				
				
				break;
				/********************************************************************/
			default:
				Menu='1';
				break;
		};
	}
}
/*
** procedure and function
*/
void PORTINIT(void)
{
	//INPUT
	DDRF=0x00;
	PORTF=0x0F;
	//OUTPUT
	DDRB|=(1<<5) | (1<<6) | (1<<7);
}
/*
** interrupt
*/
ISR(TIMER0_COMP_vect) // 1Hz and usart Tx
{
	uint8_t Sreg;
	Sreg = SREG;
	SREG &= ~(1<<7);
	if(count>repeat){ //59 -> 1Hz
		pid_out_1=pid_1.output(&pid_1.par,adcvalue,0.5);
		pid_out_2=pid_2.output(&pid_2.par,adcvalue,0.5);
		increment++;
		if((increment & 0x0F) < 8){
			shift.bit(&shift.par, 0);
			shift.out(&shift.par);
		}else{
			shift.bit(&shift.par, 1);
			shift.out(&shift.par);
		}
		
		count=0;
	}else
		count++;
	SREG = Sreg;
}
/***EOF***/
/**** Comment:
note: not defining interrupt function, or initializing an object will block the program.
****/

