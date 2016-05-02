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
 * 
 * NOTES:
 * 1. DS3231 uses I2C1:
 *    RC3 (SCLK1) = pin 18
 *    RC4 (SDAT1) = pin 23
 * 2. LCD uses:
 *    
 * 
 * 
 * 
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

byte hbuf[17] = "0123456789ABCDEF";
byte sbuf[17] = "0123456789ABCDEF";

byte dispSTR1[21] = "1 0123456789ABCDEF 1";
byte dispSTR2[21] = "2 FEDCBA9876543210 2";
byte dispSTR3[21] = "3 RPC ELECTRONIC INC";
byte dispSTR4[21] = "4 RPC ENGINEERING ..";

byte dispSTR5[21] = "5 EMAIL: rfischer@  ";
byte dispSTR6[21] = "6 rpcelectronics.com";
byte dispSTR7[21] = "7 RTC DAY HR MONTH  ";
byte dispSTR8[21] = "8 Have a nice day!  ";

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
    init();             // INIT the MCU etc
    initLCD();
    Nop();
    Nop();
    
    id_print();         // ID msg OUT on serial comm
    Delay_msec(2000);
    Nop();
    Nop();
//
    LCDterm(1);         // Set LCD line
    LCDdisplaySTRING(dispSTR7);
    Delay_msec(2000);      
//
    while(1){
       putrs1USART("*** ROGER *** \n\r\a"); 
       Delay_msec(3000);
       Nop();  
    }
//
//    
    return;
}
