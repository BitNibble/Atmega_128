/*************************************************************************
	eeprom wrapper
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: ATmega
Update:   07/01/2024
Comment:
   Very Stable
*************************************************************************/
/*** File Library ***/
#include "atmegaeeprom.h"

/*** File Variable ***/
static EEPROM0 atmega128_eeprom;

/*** File Header ***/

/*** Procedure & Function ***/
EEPROM0 eeprom_enable(void){
	atmega128_eeprom.read_byte = eeprom_read_byte;
	atmega128_eeprom.write_byte = eeprom_write_byte;
	atmega128_eeprom.update_byte = eeprom_update_byte;
	atmega128_eeprom.read_word = eeprom_read_word;
	atmega128_eeprom.write_word = eeprom_write_word;
	atmega128_eeprom.update_word = eeprom_update_word;
	//atmega128_eeprom.read_dword = eeprom_read_dword;
	atmega128_eeprom.write_dword = eeprom_write_dword;
	atmega128_eeprom.write_dword = eeprom_write_dword;
	//atmega128_eeprom.read_float = eeprom_read_float;
	atmega128_eeprom.write_float = eeprom_write_float;
	atmega128_eeprom.update_float = eeprom_update_float;
	atmega128_eeprom.read_block = eeprom_read_block;
	atmega128_eeprom.write_block = eeprom_write_block;
	atmega128_eeprom.update_block = eeprom_update_block;
	
	return atmega128_eeprom;
}

EEPROM0* eeprom(void){ return &atmega128_eeprom; }

/***EOF***/

