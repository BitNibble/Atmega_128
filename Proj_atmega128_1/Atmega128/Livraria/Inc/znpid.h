/************************************************************************
	ZNPID
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega 128
Date:     17022021_start
Comment:

************************************************************************/
#ifndef _ZNPID_H_
	#define _ZNPID_H_

/*** Global Library ***/
#include <inttypes.h>

/*** Constant & Macro ***/
#define ZNPID_outMAX 1023
#define ZNPID_outMIN -1023

/*** Global Variable ***/
typedef struct {
	double kc; // constant p
	double ki; // constant i
	double kd; // constant d
	double SetPoint; // desired output
	double Err_past; // Last Error reading
	double dy; // difference error y axis points.
	double dx; // difference time x axis points.
	double integral; // progression
	double derivative; // rate of growth (tangent), or derivative
	double PV; // output feedback
	double OP; // output signal
}znpid_parameter;

typedef struct{
	znpid_parameter par;
	/******/
	void (*set_kc)(znpid_parameter* par, double kc);
	void (*set_ki)(znpid_parameter* par, double ki);
	void (*set_kd)(znpid_parameter* par, double kd);
	void (*set_SP)(znpid_parameter* par, double setpoint);
	double (*output)(znpid_parameter* par, double PV, double timelapse);
}ZNPID;

/*** Global Header ***/
ZNPID znpid_enable(void);

#endif

/***EOF***/

