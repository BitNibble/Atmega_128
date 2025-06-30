/*************************************************************************
Title:    watch.c
Author:   Sergio Manuel Santos <sergio.salazar.santos@gmail.com>
File:     $Id: watch.c,v 0.2 2017/07/01 17:00:00 Sergio Exp $
Software: AVR-GCC 4.1, AVR Libc 1.4.6 or higher
Hardware:
License:  GNU General Public License
Comment:
	
*************************************************************************/
/*** File Library ***/
#include "watch.h"

/*** File Constant & Macro***/
#define N_DELAY 16
#define N_DELAY_MASK 0X0F
#define W12HOUR_CYCLE 12
#define W24HOUR_CYCLE 24
#define W12_HOUR_SECONDS 43199
#define W24_HOUR_SECONDS 84399
/*** File Variable ***/
static WATCH_TIME time;
static char WATCH_vector[9];
static uint32_t WATCH_trigger[N_DELAY] = {0};
static uint8_t WATCH_delay_flag[N_DELAY] = {0};

/*** File Header ***/
void WATCH_set_hour(uint8_t hour);
void WATCH_set_minute(uint8_t min);
void WATCH_preset(uint8_t hour, uint8_t minute, uint8_t second);
uint8_t WATCH_start_delay(uint8_t n_delay, uint32_t seconds);
void WATCH_result(void);
char* WATCH_show(void);

/*** Procedure & Function ***/
WATCH WATCH_enable(void)
{
	time.hour = 0;
	time.minute = 0;
	time.second = 0;
	time.seconds = 0;
	
	WATCH watch;
	watch.preset = WATCH_preset;
	watch.start_delay = WATCH_start_delay;
	watch.increment = WATCH_increment;
	watch.show = WATCH_show;
	return watch;
}

uint8_t WATCH_start_delay(uint8_t delay_n, uint32_t seconds){
	uint32_t segundos = 0;
	uint8_t ret = 0; // one shot repeat
	delay_n &= N_DELAY_MASK;
	if(WATCH_delay_flag[delay_n]) {
		segundos = time.seconds;
		if( segundos >= WATCH_trigger[delay_n] ) {
			WATCH_delay_flag[delay_n] = 0;
			ret = 1;
		}
	}else {
		segundos = time.seconds + seconds;
		if(segundos > W24_HOUR_SECONDS)
			WATCH_trigger[delay_n] = segundos - (W24_HOUR_SECONDS + 1);
		else
			WATCH_trigger[delay_n] = segundos;
		WATCH_delay_flag[delay_n] = 1;
	}
	return ret;
}

void WATCH_preset(uint8_t hour, uint8_t minute, uint8_t second)
{
	if(hour < W24HOUR_CYCLE)
		time.hour = hour;
	else
		time.hour = 0;
		
	if( minute < 60 )
		time.minute = minute;
	else
		time.minute = 0;
		
	if( second < 60 )
		;
	else
		time.second = 0;
	time.seconds = (uint32_t) (hour * 3600. + minute * 60. + second);
}

void WATCH_set_minute(uint8_t min){
	if( min < 60 )
		time.seconds = time.hour * 3600. + min * 60. + time.second;
}

void WATCH_set_hour(uint8_t hour){
	if( hour < W24HOUR_CYCLE )
		time.seconds = hour * 3600. + time.minute * 60. + time.second;
}

void WATCH_increment(void)
{
	if(time.seconds < W24_HOUR_SECONDS) {
		time.seconds++;
	}else {
		time.seconds = 0;
	}
}

void WATCH_decrement(void)
{
	if(time.seconds)
		time.seconds--;
	else
		time.seconds = W24_HOUR_SECONDS;
}

void WATCH_result(void)
{
	uint16_t remainder = time.seconds % 3600;
	time.hour = time.seconds / 3600;
	time.minute = remainder / 60;
	time.second = remainder % 60;
}

char* WATCH_show(void)
{
	uint8_t tmp;
	WATCH_result();
	WATCH_vector[8] = '\0';
	WATCH_vector[7] = time.second % 10 + '0';
	tmp = time.second / 10;
	WATCH_vector[6] = tmp % 10 + '0';
	WATCH_vector[5] = ':';
	WATCH_vector[4] = time.minute % 10 + '0';
	tmp = time.minute / 10;
	WATCH_vector[3] = tmp % 10 + '0';
	WATCH_vector[2] = ':';
	WATCH_vector[1] = time.hour % 10 + '0';
	tmp = time.hour / 10;
	WATCH_vector[0] = tmp % 10 + '0';
	return WATCH_vector;
}

/***EOF***/

