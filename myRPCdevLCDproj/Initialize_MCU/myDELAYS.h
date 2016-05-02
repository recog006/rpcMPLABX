/* 
 * File:     myDELAYS.h 
 * Author:   RGF
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_myDELAYS_H
#define	XC_myDELAYS_H

#include <xc.h> // include processor files - each processor file is guarded.  

extern void Delay_msec(unsigned int count);   // Keyword "extern" = "declare without defining"
extern void Delay_usec(unsigned int count);

#endif	/* XC_myDELAYS_H */

