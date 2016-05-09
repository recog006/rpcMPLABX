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
 *      RC3 (SCLK1) = pin 18
 *      RC4 (SDAT1) = pin 23
 * 
 *    I2C interface:
 *    DS3231 (SLAVE) ADDR: 0xD0 (fixed)
 *    AT24C32        ADDR: 0x57 (open SMD pads)
 *  
 * 2. LCD uses (see "initialization.h")
 * 
 * 3. FLOW CONTROL to USB
 *      RC5 (INPUT) monitors USB_RTS from USB interface
 *      RB5 (OUTPUT) signals USB_CTS to USB interface
 * 
 * 4. 
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
#include <string.h>

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

byte dispSTR1[21] = "1   RPC RGF 2016   1";
byte dispSTR2[21] = "2 DS3231";
byte dispSTR3[21] = "3 AT24C32";
byte dispSTR4[21] = "4 tbd RTC";

byte dispSTR5[21] = "5 EMAIL: rfischer@  ";
byte dispSTR6[21] = "6 rpcelectronics.com";
byte dispSTR7[21] = "7 RTC DAY HR MONTH  ";
byte dispSTR8[21] = "RTC H:M:S:";

byte RTC_seconds @ 0x0200;   
byte RTC_minutes;
byte RTC_hour;         // Use 24 hour mode ....

// *** END of GLOBAL VAR declarations ....................

//
// INTERRUPTS .....
//


// ***********************************************
// *** LOCAL ("main.c") subroutines here .........



// Set the RTC variables by reading DS3231 .......

void getRTCtime(void){
    RTC_seconds = DS3231_GetInfo(0);   // Get "SECONDS" ... 
    RTC_minutes = DS3231_GetInfo(1);   // 
    RTC_hour = DS3231_GetInfo(2);      //  
    Nop();
}

// Send to USART the RTC variables ...
// Display on line 4 of LCD ..........

void displayRTCtime(void){
    crlf_print();
 //    
    bdum = RTC_hour;
    btoa();
    strcat(dispSTR8, sbuf);
    strcpy(sbuf, ":");
    strcat(dispSTR8, sbuf);
//    
    bdum = RTC_minutes;
    btoa();
    strcat(dispSTR8, sbuf);
    strcpy(sbuf, ":");
    strcat(dispSTR8, sbuf);
//
    bdum = RTC_seconds;
    btoa();
    strcat(dispSTR8, sbuf);   
    putrs1USART(sbuf);
    crlf_print();  
    Nop();  
    Nop();
//
    LCDterm(4);         // Set LCD line
    LCDdisplaySTRING(dispSTR8); 
    Nop();
    Nop();
}


// *** END of LOCAL subroutines ..................
// ...............................................

// ************************************************************
// **************   MAIN / START   ****************************
// ************************************************************

void main(void) {
    init();             // INIT the MCU and USARTs ............
    Nop();
    Nop();
//
    id_print();         // ID msg OUT on serial comm   
    Nop();
    Nop();  
//
    initLCD();
    Nop();
    Nop();
//    
    LCDterm(1);            // Set LCD line
    LCDdisplaySTRING(dispSTR1);  
    Delay_msec(4000);   
//    
    bdum = 0;
    bdum = initRTCmodule(0xD0);   // Set device addr ...
    Nop();
    Nop();
    Nop();
    Delay_msec(3000);    
//
    while(1){
       putrs1USART("*** ROGER *** \n\r\a"); 
       Delay_msec(5000);
       Nop();  
    }
//
//    
    return;
}
