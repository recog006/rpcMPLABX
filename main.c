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
 *           ( APPLIANCE or test system foundation/ref design ... )
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
 * 4. Keyboard? Other features?
 * 
 * 5. RASP PI interface ... UNDER CONSTRUCTION ....
 *    
 * 
 * 
 * 
 * 
 */

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <plib/usart.h>

#include "Initialize_MCU/configMCU.h"
#include "Initialize_MCU/initialization.h"
#include "Initialize_MCU/myDELAYS.h"

#include "DS3231/DS3231.h"
#include "I2C_driver/myI2C.h"
#include "LCD_driver/LCD_20x4_drvrs.h"
#include "USART_driver/msgserials.h"

const char dispSTR1[] = "1 RPC ELEC RGF 2016 ";
const char dispSTR2[] = "2 DS3231 ********** ";
const char dispSTR3[] = "3 AT24C32 ********* ";
const char dispSTR4[] = "4 This is line FOUR";

const char dispSTR5[] = "5 EMAIL: rfischer@  ";
const char dispSTR6[] = "6 rpcelectronics.com";
const char dispSTR7[] = "7 RTC DAY MONTH YEAR";
const char dispSTR8[] = "8 RTC H:M:S ";

const char hbuf[] = "0123456789ABCDEF";

/* *** GLOBAL VAR declarations .................................... */

unsigned char bdum @ 0x0100;     /* Gen purpose "byte" var ........ */
unsigned int idum;               /* Gen purpose "word" var ........ */
unsigned int mloopCNTR;

unsigned char rbyte1;            
unsigned char cstatus;

unsigned char RTC_seconds;   
unsigned char RTC_minutes;
unsigned char RTC_hour;         // Use 24 hour mode ....

char sbuf[21] @ 0x0200;
char RTCbuf[21] @ 0x0300;

/* *** END of GLOBAL VAR declarations .................... */

/* *** INTERRUPTS ........................................
 *             
 *   Check the source of the interrupt :
 *
 *   PIR1bits.RC1IF = USART 1 receive reg int flag bit....
 *
 *   CCP2 ... also high pri ...
 *   Timer 0
 *   Timer 2
 *
 *   .............    */

void interrupt low_priority low_isr(void){
    Nop();
    Nop();   
}

void interrupt high_isr(void){
    if (PIR1bits.RC1IF == 1) {
        rbyte1 = RCREG1;
        cstatus = cstatus | 0x80;  /* Set the comm status bit */
		Nop();
		Nop();
        PIR1bits.RC1IF = 0;    /* Clear the flag bit to receive another */
	};   
    Nop();
    Nop();    
}

// *******************************************************
// *** LOCAL ("main.c") subroutines here .................

void MENUsetRTChour(void){
  putrs1USART("Enter hour [00-23]: ");    
    
}

void MENUsetRTCminute(void){
  putrs1USART("Enter minute [00-59]: ");    
    
}


void MENUsetRTCmonth(void){
  putrs1USART("Enter month [01-12]: ");    
    
}

void MENUsetRTCday(void){
  putrs1USART("Enter minute [00-30]: ");    
    
}






void MENUtbd(void){
   putrs1USART(" .... TBD .... \n\r");   
    
}

// *** END of LOCAL subroutines ..................
// ...............................................

// ************************************************************
// **************   MAIN / START   ****************************
// ************************************************************

void main(void) {
    init();             // INIT the MCU and USARTs ............
    id_print();         // ID msg OUT on serial comm  
    Delay_msec(1000); 
    Nop();
    Nop();
// 
    menu(); 
    Nop();
    Nop();
    intrENAB();
    Nop();
    Nop();
    
    cstatus = 0; 
    select_loop: bdum = cstatus & 0x80;
    if (bdum == 0x80){
        Nop();
        Nop();
        Nop();
        Write1USART(rbyte1);   /* ECHO the received char .... */
        crlf_print();
        
        switch(rbyte1){
            case '1' : MENUsetRTC();
            break;
            
            case '2' : MENUtbd();
            break;
            
            default : MENUtbd();
            break;
        };
        
        Nop();
        Nop();
        cstatus = 0;
        } else {
            Nop();
            Nop();
            Nop();
            goto select_loop;     
        };
    
    Nop();
    Nop();
    Nop();

 //   putrs1USART("ENTER five chars:"); 
 //   gets1USART(sbuf, 5);
 //   bdum = Read1USART(); // ?????? 
    
    crlf_print();
    crlf_print();    
//
//
//   
    initLCD();          // LCD power ON and back light ON ....
    Nop();
    Nop();
    
// *** The following provides a test display to the LCD --- FOUR LINES ....    
    strcpy(sbuf, dispSTR1);
//      crlf_print();
//      putrs1USART(sbuf);
//      crlf_print();
      
    LCDdisplaySTRING(1, sbuf);
    Nop();
    Nop();
    
    strcpy(sbuf, dispSTR2);
    LCDdisplaySTRING(2, sbuf);
    strcpy(sbuf, dispSTR3);
    LCDdisplaySTRING(3, sbuf);
    strcpy(sbuf, dispSTR4);
    LCDdisplaySTRING(4, sbuf);
// *** END of LCD test display .............................................
    
    Nop();
    Nop();
    Nop();
    
    initRTCmodule(0xD0);
   
    crlf_print();
    DS3231_GetCalendar(RTCbuf);
    putrs1USART(RTCbuf);
    crlf_print();
    
    bdum = DS3231_GetTime(RTCbuf);
    putrs1USART("DS3231_GetTime return byte: 0x"); 
    byteTOascHEX(bdum);
    putrs1USART(sbuf);
    crlf_print();
    
    putrs1USART("DS3231_GetTime: ");
    putrs1USART(RTCbuf);
    crlf_print();

    homeLCD();
    clearLCD();
    Nop();
    
    bdum = DS3231_GetTime(RTCbuf);
    LCDdisplaySTRING(4, RTCbuf); 
    Delay_msec(2000);  
    
//    DS3231_SetInfo(HOURS, 23);
//    zeroRTCtime(); 
//
//    bdum =  RTC_seconds;
//    bytetohex();
//    if (USB_RTS == 0) putrs1USART(sbuf); 
//    crlf_print();   
//
//    bdum =  RTC_minutes;
//    bytetohex();
//    if (USB_RTS == 0) putrs1USART(sbuf); 
//    crlf_print();   
//
//    bdum =  RTC_hour;
//    bytetohex();
//    if (USB_RTS == 0) putrs1USART(sbuf); 
//    crlf_print();   
//
//    crlf_print(); 
    

    
//    putrs1USART(sbuf);  crlf_print();
//    crlf_print();
    
    mloopCNTR = 0;
    while(1){
       putrs1USART(" mloopCNTR: 0x");
       idum = mloopCNTR;
       wordTOascHEX(idum);       /* Converts idum into sbuf */
       putrs1USART(sbuf); 
       crlf_print();
//
//       bdum = DS3231_GetTime(RTCbuf);
//       bytetohex();
//       if (USB_RTS == 0) putrs1USART(sbuf);  
//       crlf_print();
       
//        LCDdisplaySTRING(4, RTCbuf);
        
//       getRTCtime();
//       LCDdisplayRTC();
//       Nop();
//    
       bdum = DS3231_GetTime(RTCbuf);
       LCDdisplaySTRING(4, RTCbuf); 
       
       mloopCNTR++;   
       Delay_msec(1000);
     };
//
//    
    return;
}
