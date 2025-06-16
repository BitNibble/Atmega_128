/***************************************************************************************************
	ATMEGA128TWI
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: ATmega128
Update:   07/01/2024
Comment:
	Stable
***************************************************************************************************/
/*** File Library ***/
#include "atmega128twi.h"
#include <util/delay.h>

/*** File Variable ***/
static TWI0 atmega128_twi;

/*** File Header ***/
void TWI_init(uint8_t device_id, uint8_t prescaler);
void TWI_start(void);
void TWI_connect(uint8_t address, uint8_t rw);
void TWI_master_write(uint8_t var_twiData_u8);
uint8_t TWI_master_read(uint8_t ack_nack);
void TWI_stop(void);
// auxiliary
uint8_t TWI_status(void);
void TWI_wait_twint( uint16_t nticks );

/*** Procedure & Function ***/
// TWI TWIenable(uint8_t atmega_ID,  uint8_t prescaler)
TWI0 twi_enable(uint8_t atmega_ID,  uint8_t prescaler)
{
	// ATMEGA128enable();
	
	// Vtable
	atmega128_twi.instance = twi_instance();
	atmega128_twi.start = TWI_start;
	atmega128_twi.connect = TWI_connect;
	atmega128_twi.stop = TWI_stop;
	atmega128_twi.master_write = TWI_master_write;
	atmega128_twi.master_read = TWI_master_read;
	atmega128_twi.status = TWI_status;
	
	TWI_init(atmega_ID, prescaler);
	
	return atmega128_twi;
}

TWI0* twi(void){ return &atmega128_twi; }

// void TWI_Init(uint8_t device_id, uint8_t prescaler)
void TWI_init(uint8_t device_id, uint8_t prescaler)
{
	if(device_id > 0 && device_id < 128){
		twi_instance()->twar.par.twa = device_id;
		twi_instance()->twar.par.twgce = 1;
	}else{
		twi_instance()->twar.par.twgce = 1;
	}
	portd_instance()->ddr.reg |= TWI_IO_MASK;
	portd_instance()->port.reg |= TWI_IO_MASK;
	switch(prescaler){
		case 1:
			twi_instance()->twsr.par.twps = 0;
		break;
		case 4:
			twi_instance()->twsr.par.twps = 1;
		break;
		case 16:
			twi_instance()->twsr.par.twps = 2;
		break;
		case 64:
			twi_instance()->twsr.par.twps = 3;
		break;
		default:
			prescaler = 1;
			twi_instance()->twsr.par.twps = 0;
		break;
	}
	twi_instance()->twbr.reg = ((F_CPU / TWI_SCL_CLOCK) - 16) / (2 * prescaler);
	// Standard Config begin
	// atmega128()->twi->twsr = 0x00; //set presca1er bits to zero
	// atmega128()->twi->twbr = 0x46; //SCL frequency is 50K for 16Mhz
	// atmega128()->twi->twcr = 0x04; //enab1e TWI module
	// Standard Config end
}
// void TWI_Start(void)
void TWI_start(void) // $08
{	
	uint8_t cmd = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	twi_instance()->twcr.reg = cmd;
	
	TWI_wait_twint( Nticks );
	
	switch( TWI_status( ) ){
		case TWI_T_START:
			// Do nothing
		break;
		default:
			TWI_stop( ); // error
		break;
	}
}

// void TWI_Connect(uint8_t address, uint8_t rw)
void TWI_connect( uint8_t address, uint8_t rw )
{
	uint8_t cmd = 0;
	if( rw ){ cmd = (address << 1) | (1 << 0); }
	else{ cmd = (address << 1) | (0 << 0); }
	twi_instance()->twdr.reg = cmd;
	
	cmd = (1 << TWINT) | (1 << TWEN);
	twi_instance()->twcr.reg = cmd;
	
	TWI_wait_twint( Nticks );
	
	switch( TWI_status( ) ){
		case TWI_M_SLAW_R_ACK:
			// Do nothing
		break;
		case TWI_M_SLAR_R_ACK:
			// Do nothing
		break;
		default:
			TWI_stop( ); // error
		break;
	}
}

// void TWI_Write(uint8_t var_twiData_u8)
void TWI_master_write( uint8_t var_twiData_u8 )
{
	uint8_t cmd = var_twiData_u8;
	twi_instance()->twdr.reg = cmd;
	
	cmd = (1 << TWINT) | (1 << TWEN);
	twi_instance()->twcr.reg = cmd;
	
	TWI_wait_twint( Nticks );
	
	switch( TWI_status( ) ){
		case TWI_M_DATABYTE_R_ACK:
			// Do nothing
		break;
		default:
			TWI_stop( ); // error
		break;
	}
}

// uint8_t TWI_Read(uint8_t ack_nack)
uint8_t TWI_master_read( uint8_t ack_nack )
{
	uint8_t cmd = 0x00;
	if( ack_nack ){ cmd |= ( 1 << TWEA ); }
	cmd |= ( 1 << TWINT ) | ( 1 << TWEN );
	twi_instance()->twcr.reg = cmd;
	
	TWI_wait_twint( Nticks );
	
	switch( TWI_status( ) ){
		case TWI_ARBLSLARNACK:
			TWI_stop( );
		break;
		default:
		break;
	}
	
	cmd = twi_instance()->twdr.reg;
	return cmd;
}

// void TWI_Stop(void)
void TWI_stop( void )
{
	uint8_t cmd = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	twi_instance()->twcr.reg = cmd; 
	
	_delay_us(100); // wait for a short time
}

// auxiliary
uint8_t TWI_status( void )
{
	uint8_t cmd = twi_instance()->twsr.reg & TWI_STATUS_MASK;
	return cmd;
}

void TWI_wait_twint( uint16_t nticks ) // hardware triggered
{
	unsigned int i;
	for(i = 0; !( twi_instance()->twcr.reg & (1 << TWINT)); i++ ){ // wait for acknowledgment confirmation bit.
		if( i > nticks ) // timeout
			break;
	}
}

/*** File Interrupt ***/

/***EOF***/

