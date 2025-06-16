/************************************************************************
	ROTENC
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Rotary encoder Potentiometer
Date:     25102020
Comment:
	Stable
************************************************************************/
/*** File Library ***/
#include "rotenc.h"

/*** File Variable ***/

/*** File Header ***/
rotaryencoder_parameter RotEnc_rte(rotaryencoder_parameter* par, uint8_t data);
uint8_t ROTENClh(uint8_t xp, uint8_t xi);
uint8_t ROTENChl(uint8_t xp, uint8_t xi);

/*** Procedure & Function ***/
ROTENC rotenc_enable( uint8_t ChnApin, uint8_t ChnBpin )
{
	// struct object
	ROTENC setup_rtnc;
	
	// Initialize variables
	setup_rtnc.par.PinChnA = ChnApin;
	setup_rtnc.par.PinChnB = ChnBpin;
	setup_rtnc.par.pchn = setup_rtnc.par.chn = (1 << ChnBpin) | (1 << ChnApin);
	setup_rtnc.par.num = 0;
	// function pointers
	setup_rtnc.rte = RotEnc_rte;
	
	return setup_rtnc;
}

rotaryencoder_parameter RotEnc_rte(rotaryencoder_parameter* par, uint8_t data)
{
	uint8_t hl;
	par->chn = data & ((1 << par->PinChnB) | (1 << par->PinChnA));
	hl = ROTENChl(par->pchn, par->chn);
	if(par->pchn != par->chn){
		if((par->pchn == ((1 << par->PinChnB) | (1 << par->PinChnA))) && (hl & (1 << par->PinChnA)))
		par->num++;
		if((par->pchn == ((1 << par->PinChnB) | (1 << par->PinChnA))) && (hl & (1 << par->PinChnB)))
		par->num--;
	}
	par->pchn = par->chn;
	return *par;
}

// auxiliary
uint8_t ROTENClh(uint8_t xp, uint8_t xi)
{
	uint8_t i;
	i = xp ^ xi;
	i &= xi;
	return i;
}
uint8_t ROTENChl(uint8_t xp, uint8_t xi)
{
	uint8_t i;
	i = xi ^ xp;
	i &= xp;
	return i;
}

/***EOF***/

