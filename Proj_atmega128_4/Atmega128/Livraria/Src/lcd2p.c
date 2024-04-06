/*************************************************************************
	LCD2P
Author: Sergio Santos 
	<sergio.salazar.santos@gmail.com>
License: GNU General Public License
Hardware: all
Date: 20042023
Comment:
	Tested Atemga128 16Mhz and Atmega328 8Mhz                    
************************************************************************/
/*** File Library ***/
#include "lcd2p.h"
#include <util/delay.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <stdarg.h>
//#include <math.h>

/*** File Constant & Macro ***/
// CMD RS
#define LCD02P_INST 0
#define LCD02P_DATA 1
// ticks depends on CPU frequency this case 16Mhz
#define LCD02P_N_TICKS 0
#define LCD02P_BN_TICKS 0

/*** File Variable ***/
volatile uint8_t *lcd02pcmd_DDR;
volatile uint8_t *lcd02pcmd_PIN;
volatile uint8_t *lcd02pcmd_PORT;
volatile uint8_t *lcd02pdata_DDR;
volatile uint8_t *lcd02pdata_PIN;
volatile uint8_t *lcd02pdata_PORT;
static uint8_t DDR_DATA_MASK;
uint8_t lcd02p_detect;

/*** File Header ***/
void LCD02P_inic(void);
void LCD02P_write(char c, unsigned short D_I);
char LCD02P_read(unsigned short D_I);
void LCD02P_BF(void);
void LCD02P_putch(char c);
char LCD02P_getch(void);
void LCD02P_string(const char* s); // RAW
void LCD02P_string_size(const char* s, uint8_t size); // RAW
void LCD02P_hspace(uint8_t n);
void LCD02P_clear(void);
void LCD02P_gotoxy(unsigned int y, unsigned int x);
void LCD02P_strobe(uint16_t num);
void LCD02P_reboot(void);
void LCD02P_ticks(uint16_t num);

/*** Procedure & Function ***/
LCD02P lcd02p_enable(volatile uint8_t *cmdddr, volatile uint8_t *cmdpin, volatile uint8_t *cmdport, volatile uint8_t *dataddr, volatile uint8_t *datapin, volatile uint8_t *dataport)
{
	// LOCAL VARIABLES
	// ALLOCAÇÂO MEMORIA PARA Estrutura
	LCD02P setup_lcd02p;
	// import parameters
	lcd02pcmd_DDR = cmdddr;
	lcd02pcmd_PIN = cmdpin;
	lcd02pcmd_PORT = cmdport;
	lcd02pdata_DDR = dataddr;
	lcd02pdata_PIN = datapin;
	lcd02pdata_PORT = dataport;
	DDR_DATA_MASK = (1 << LCD02P_DB4) | (1 << LCD02P_DB5) | (1 << LCD02P_DB6) | (1 << LCD02P_DB7);
	// initialize variables
	*lcd02pcmd_DDR &= ~((1 << LCD02P_RS) | (1 << LCD02P_RW) | (1 << LCD02P_EN) | (1 << LCD02P_NC));
	*lcd02pcmd_PORT |= (1 << LCD02P_RS) | (1 << LCD02P_RW) | (1 << LCD02P_EN) | (1 << LCD02P_NC);
	*lcd02pdata_DDR &= ~DDR_DATA_MASK;
	*lcd02pdata_PORT |= DDR_DATA_MASK;
	lcd02p_detect = *lcd02pcmd_PIN & (1 << LCD02P_NC);
	// Direccionar apontadores para PROTOTIPOS
	setup_lcd02p.write = LCD02P_write;
	setup_lcd02p.read = LCD02P_read;
	setup_lcd02p.BF = LCD02P_BF;
	setup_lcd02p.putch = LCD02P_putch;
	setup_lcd02p.getch = LCD02P_getch;
	setup_lcd02p.string = LCD02P_string; // RAW
	setup_lcd02p.string_size = LCD02P_string_size; // RAW
	setup_lcd02p.hspace = LCD02P_hspace;
	setup_lcd02p.clear = LCD02P_clear;
	setup_lcd02p.gotoxy = LCD02P_gotoxy;
	setup_lcd02p.reboot = LCD02P_reboot;
	// LCD INIC
	LCD02P_inic();
	
	return setup_lcd02p;
}
void LCD02P_inic(void)
{
	// LCD INIC
	*lcd02pcmd_DDR |= (1 << LCD02P_RS) | (1 << LCD02P_RW) | (1 << LCD02P_EN) | (0 << LCD02P_NC);
	*lcd02pcmd_PORT |= (1 << LCD02P_NC);
	*lcd02pdata_DDR &= ~DDR_DATA_MASK;
	*lcd02pdata_PORT |= DDR_DATA_MASK;
	// INICIALIZACAO LCD datasheet/
	_delay_ms(40); // using clock at 16Mhz
	LCD02P_write(0x38, LCD02P_INST); // function set
	_delay_us(39);
	LCD02P_write(0x38, LCD02P_INST); // function set
	_delay_us(39);
	LCD02P_write(0x38, LCD02P_INST); // function set
	_delay_us(39);
	LCD02P_write(0x28, LCD02P_INST); // function set 2B
	_delay_us(37);
	LCD02P_write(0x28, LCD02P_INST); // function set 2B
	_delay_us(37);
	/**************************************/
	// for(repeat = 2 ; repeat ; repeat--){
	// repeat twice in 4 bit length
	LCD02P_write(0x28, LCD02P_INST); // function set 2B
	LCD02P_BF();
	LCD02P_write(0x28, LCD02P_INST); // function set 2B
	LCD02P_BF();

	LCD02P_write(0x0C, LCD02P_INST); // display on/off control
	LCD02P_BF();
	LCD02P_write(0x0C, LCD02P_INST); // display on/off control
	LCD02P_BF();

	LCD02P_write(0x01, LCD02P_INST); // clear display
	LCD02P_BF();
	LCD02P_write(0x01, LCD02P_INST); // clear display
	LCD02P_BF();

	LCD02P_write(0x06, LCD02P_INST); // entry mode set (crazy settings)
	LCD02P_BF();
	LCD02P_write(0x06, LCD02P_INST); // entry mode set (crazy settings)
	LCD02P_BF();
	//}
	// INICIALIZATION END
	// LCD02P_write(0x1F, LCD02P_INST); // cursor or display shift
	// _delay_us(39);
	// LCD02P_write(0x03, LCD02P_INST); // return home
	// _delay_ms(1.53);
	LCD02P_gotoxy(0,0);
}
void LCD02P_write(char c, unsigned short D_I)
{
	*lcd02pcmd_PORT &= ~(1 << LCD02P_RW); // lcd as input
	*lcd02pdata_DDR |= DDR_DATA_MASK; // mcu as output
	
	if(D_I) *lcd02pcmd_PORT |= (1 << LCD02P_RS); else *lcd02pcmd_PORT &= ~(1 << LCD02P_RS);
	LCD02P_strobe(LCD02P_N_TICKS); LCD02P_ticks(LCD02P_BN_TICKS);
	
	if(c & 0x80) *lcd02pdata_PORT |= 1 << LCD02P_DB7; else *lcd02pdata_PORT &= ~(1 << LCD02P_DB7); LCD02P_ticks(LCD02P_BN_TICKS);
	if(c & 0x40) *lcd02pdata_PORT |= 1 << LCD02P_DB6; else *lcd02pdata_PORT &= ~(1 << LCD02P_DB6); LCD02P_ticks(LCD02P_BN_TICKS);
	if(c & 0x20) *lcd02pdata_PORT |= 1 << LCD02P_DB5; else *lcd02pdata_PORT &= ~(1 << LCD02P_DB5); LCD02P_ticks(LCD02P_BN_TICKS);
	if(c & 0x10) *lcd02pdata_PORT |= 1 << LCD02P_DB4; else *lcd02pdata_PORT &= ~(1 << LCD02P_DB4); LCD02P_ticks(LCD02P_BN_TICKS);
	
	if(D_I) *lcd02pcmd_PORT |= (1 << LCD02P_RS); else *lcd02pcmd_PORT &= ~(1 << LCD02P_RS);
	LCD02P_strobe(LCD02P_N_TICKS); LCD02P_ticks(LCD02P_BN_TICKS);
	
	if(c & 0x08) *lcd02pdata_PORT |= 1 << LCD02P_DB7; else *lcd02pdata_PORT &= ~(1 << LCD02P_DB7); LCD02P_ticks(LCD02P_BN_TICKS);
	if(c & 0x04) *lcd02pdata_PORT |= 1 << LCD02P_DB6; else *lcd02pdata_PORT &= ~(1 << LCD02P_DB6); LCD02P_ticks(LCD02P_BN_TICKS);
	if(c & 0x02) *lcd02pdata_PORT |= 1 << LCD02P_DB5; else *lcd02pdata_PORT &= ~(1 << LCD02P_DB5); LCD02P_ticks(LCD02P_BN_TICKS);
	if(c & 0x01) *lcd02pdata_PORT |= 1 << LCD02P_DB4; else *lcd02pdata_PORT &= ~(1 << LCD02P_DB4); LCD02P_ticks(LCD02P_BN_TICKS);
	
	*lcd02pcmd_PORT &= ~(1 << LCD02P_EN); LCD02P_ticks(LCD02P_N_TICKS);
}
char LCD02P_read(unsigned short D_I)
{
	char c = 0x00;
	*lcd02pdata_DDR &= ~DDR_DATA_MASK; // mcu as input
	*lcd02pdata_PORT |= DDR_DATA_MASK; // pull up resistors
	*lcd02pcmd_PORT |= (1 << LCD02P_RW); // lcd as output
	
	if(D_I) *lcd02pcmd_PORT |= (1 << LCD02P_RS); else *lcd02pcmd_PORT &= ~(1 << LCD02P_RS);
	LCD02P_strobe(LCD02P_N_TICKS); LCD02P_ticks(LCD02P_BN_TICKS);
	
	if(*lcd02pdata_PIN & (1 << LCD02P_DB7)) c |= 1 << 7; else c &= ~(1 << 7); LCD02P_ticks(LCD02P_BN_TICKS);
	if(*lcd02pdata_PIN & (1 << LCD02P_DB6)) c |= 1 << 6; else c &= ~(1 << 6); LCD02P_ticks(LCD02P_BN_TICKS);
	if(*lcd02pdata_PIN & (1 << LCD02P_DB5)) c |= 1 << 5; else c &= ~(1 << 5); LCD02P_ticks(LCD02P_BN_TICKS);
	if(*lcd02pdata_PIN & (1 << LCD02P_DB4)) c |= 1 << 4; else c &= ~(1 << 4); LCD02P_ticks(LCD02P_BN_TICKS);
	
	if(D_I) *lcd02pcmd_PORT |= (1 << LCD02P_RS); else *lcd02pcmd_PORT &= ~(1 << LCD02P_RS);
	LCD02P_strobe(LCD02P_N_TICKS); LCD02P_ticks(LCD02P_BN_TICKS);
	
	if(*lcd02pdata_PIN & (1 << LCD02P_DB7)) c |= 1 << 3; else c &= ~(1 << 3); LCD02P_ticks(LCD02P_BN_TICKS);
	if(*lcd02pdata_PIN & (1 << LCD02P_DB6)) c |= 1 << 2; else c &= ~(1 << 2); LCD02P_ticks(LCD02P_BN_TICKS);
	if(*lcd02pdata_PIN & (1 << LCD02P_DB5)) c |= 1 << 1; else c &= ~(1 << 1); LCD02P_ticks(LCD02P_BN_TICKS);
	if(*lcd02pdata_PIN & (1 << LCD02P_DB4)) c |= 1 << 0; else c &= ~(1 << 0); LCD02P_ticks(LCD02P_BN_TICKS);
	
	*lcd02pcmd_PORT &= ~(1 << LCD02P_EN); LCD02P_ticks(LCD02P_N_TICKS);
	
	return c;
}
void LCD02P_BF(void)
// it has to read at minimum one equal and exit immediately if not equal, weird property.
{
	uint8_t i;
	char inst = 0x80;
	for(i=0; 0x80 & inst; i++){
		inst = LCD02P_read(LCD02P_INST);
		if(i > 1)
			break;
	}
}
char LCD02P_getch(void)
{
	char c;
	c = LCD02P_read(LCD02P_DATA);
	LCD02P_BF();
	return c;
}
void LCD02P_putch(char c)
{
	LCD02P_write(c, LCD02P_DATA);
	LCD02P_BF();
}
void LCD02P_string(const char* s)
{
	char tmp;
	while(*s){
		tmp = *(s++);
		LCD02P_putch(tmp);
	}
}
void LCD02P_string_size(const char* s, uint8_t size)
{
	char tmp;
	uint8_t pos = 0;
	while(*s){
		tmp=*(s++);
		pos++;
		if(pos > size) // 1 TO SIZE+1
			break;
		LCD02P_putch(tmp);
	}
	while(pos<size){ // TO SIZE
		LCD02P_putch(' ');
		pos++;
	}
}
void LCD02P_hspace(uint8_t n)
{
	for(; n; n--){
		LCD02P_putch(' ');
	}
}
void LCD02P_clear(void)
{
	LCD02P_write(0x01, LCD02P_INST);
    _delay_ms(1.53);
}
void LCD02P_gotoxy(unsigned int y, unsigned int x)
{
	switch(y){
		case 0:
			LCD02P_write((0x80 + x), LCD02P_INST);
			LCD02P_BF();
			break;
		case 1:
			LCD02P_write((0xC0 + x), LCD02P_INST);
			LCD02P_BF();
			break;
		case 2:
			LCD02P_write((0x94 + x), LCD02P_INST); // 0x94
			LCD02P_BF();
			break;
		case 3:
			LCD02P_write((0xD4 + x), LCD02P_INST); // 0xD4
			LCD02P_BF();
			break;
		default:
			break;
	}
}
void LCD02P_strobe(uint16_t num)
{
	*lcd02pcmd_PORT &= ~(1 << LCD02P_EN);
	LCD02P_ticks(num);
	*lcd02pcmd_PORT |= (1 << LCD02P_EN);
}
void LCD02P_reboot(void)
{
	// low high detect pin NC
	uint8_t i;
	uint8_t tmp;
	tmp = *lcd02pcmd_PIN & (1 << LCD02P_NC);
	i = tmp ^ lcd02p_detect;
	i &= tmp;
	if(i)
		LCD02P_inic();
	lcd02p_detect = tmp;
}
void LCD02P_ticks(uint16_t num)
{
	uint16_t count;
	for(count = 0; count < num; count++) ;
}

/***EOF***/

