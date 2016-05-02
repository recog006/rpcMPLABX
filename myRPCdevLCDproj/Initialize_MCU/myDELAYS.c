/* 
 * File:   myDELAYS.c
 * Author: RGF
 * 
 */

#include <xc.h>
#include "myDELAYS.h"

#define _XTAL_FREQ 16000000  // Select your XTAL

void Delay_msec(unsigned int count){
unsigned int i;
for(i = 0; i < count; i++) __delay_ms(1);
}

void Delay_us(unsigned int count){
unsigned int i;
for(i = 0; i < count; i++) __delay_us(1);
}