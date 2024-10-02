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
void TWI_wait_twint(uint16_t nticks);

/*** Procedure & Function ***/
TWI0 twi_enable(uint8_t atmega_ID, uint8_t prescaler)
{
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

TWI0* twi(void) {
	return &atmega128_twi;
}

void TWI_init(uint8_t device_id, uint8_t prescaler)
{
	if (device_id > 0 && device_id < 128) {
		twi_instance()->twar.par.twa = device_id;
		twi_instance()->twar.par.twgce = 1;
		} else {
		twi_instance()->twar.par.twgce = 1; // Enable General Call
	}
	
	portd_instance()->ddr.reg |= TWI_IO_MASK;
	portd_instance()->port.reg |= TWI_IO_MASK;

	// Set prescaler
	twi_instance()->twsr.par.twps = (prescaler == 1) ? 0 :
	(prescaler == 4) ? 1 :
	(prescaler == 16) ? 2 :
	(prescaler == 64) ? 3 : 0; // Default to 1

	twi_instance()->twbr.reg = ((F_CPU / TWI_SCL_CLOCK) - 16) / (2 * (1 << twi_instance()->twsr.par.twps));
}

void TWI_start(void) {
	twi_instance()->twcr.reg = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	TWI_wait_twint(Nticks);
	
	if (TWI_status() != TWI_T_START) {
		TWI_stop(); // Handle error
	}
}

void TWI_connect(uint8_t address, uint8_t rw) {
	uint8_t cmd = (address << 1) | (rw ? 1 : 0);
	twi_instance()->twdr.reg = cmd;
	twi_instance()->twcr.reg = (1 << TWINT) | (1 << TWEN);
	TWI_wait_twint(Nticks);
	
	if (TWI_status() != TWI_M_SLAW_R_ACK && TWI_status() != TWI_M_SLAR_R_ACK) {
		TWI_stop(); // Handle error
	}
}

void TWI_master_write(uint8_t var_twiData_u8) {
	twi_instance()->twdr.reg = var_twiData_u8;
	twi_instance()->twcr.reg = (1 << TWINT) | (1 << TWEN);
	TWI_wait_twint(Nticks);
	
	if (TWI_status() != TWI_M_DATABYTE_R_ACK) {
		TWI_stop(); // Handle error
	}
}

uint8_t TWI_master_read(uint8_t ack_nack) {
	twi_instance()->twcr.reg = (1 << TWINT) | (1 << TWEN) | (ack_nack ? (1 << TWEA) : 0);
	TWI_wait_twint(Nticks);
	
	if (TWI_status() == TWI_ARBLSLARNACK) {
		TWI_stop(); // Handle error
	}
	
	return twi_instance()->twdr.reg;
}

void TWI_stop(void) {
	twi_instance()->twcr.reg = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	_delay_us(100); // wait for a short time
}

uint8_t TWI_status(void) {
	return twi_instance()->twsr.reg & TWI_STATUS_MASK;
}

void TWI_wait_twint(uint16_t nticks) {
	for (unsigned int i = 0; !(twi_instance()->twcr.reg & (1 << TWINT)); i++) {
		if (i > nticks) break; // Timeout
	}
}
