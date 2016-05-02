/*
 * File:   main.c
 * 
 * PLATFORM:  MicroChip MPLAB X IDE v3.26
 * 
 * Author: R G Fischer
 * EMAIL:  rfischer@rpcelectronics.com
 *
 * WORKDATE:  MAY 2016
 * 
 * PURPOSE:  CONTROL and demo LCD ( 20 x 4 )
 *           RTC module and serial comm via USB interface.
 *           ( APPLIANCE or test system foundation ... )
 * 
 */

#include <xc.h>
#include <pic18f46k22.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <plib/delays.h>
#include <plib/usart.h>

#include "Initialize_MCU/configMCU.h"
#include "Initialize_MCU/initialization.h"
#include "Initialize_MCU/myDELAYS.h"

#include "DS3231/DS3231.h"
#include "I2C_driver/myI2C.h"
#include "LCD_driver/LCD_20x4_drvrs.h"
#include "USART_driver/msgserials.h"


// *** GLOBAL VAR declarations ...........................

byte bdum @ 0x0100;              /* Gen purpose "byte" var ................... */
word idum;                       /* Gen purpose "word" var ... */

byte hbuf[17] = { "0123456789ABCDEF" };
byte sbuf[17] = { "0123456789ABCDEF" };

// *** END of GLOBAL VAR declarations ....................

//
// INTERRUPTS .....
//


// ***********************************************
// *** LOCAL ("main.c") subroutines here .........



// *** END of LOCAL subroutines ..................
// ...............................................

// ************************************************************
// **************   MAIN / START   ****************************
// ************************************************************

void main(void) {
    init();
    id_print();
    Delay_msec(2000);
    Nop();
    Nop();
//
    while(1){
       putrs1USART("*** ROGER *** \n\r\a"); 
       Nop();
       Nop();  
    }
//
//    
    return;
}
