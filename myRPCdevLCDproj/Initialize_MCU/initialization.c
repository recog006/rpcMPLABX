//
//  PIC18F46K22 MCU ........................
//
//  MODULE   : initialization.c
//

#include <xc.h>
#include <p18f46k22.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <plib\delays.h>
#include <plib\usart.h>
#include <plib\pconfig.h>

#include "initialization.h"

extern byte bdum;
extern word idum;


/* ****************   FUNCTIONS BEGIN HERE    ******************** */

// ENABLE interrupts .....

void intrENAB(void){
	INTCONbits.GIEH = 1;    /* Enable all interrupts ......  */
    INTCONbits.GIEL = 1;
	Nop();
	Nop();
}

void intrDISAB(void){
	INTCONbits.GIEH = 0;    /* DISABLE all interrupts ......  */
    INTCONbits.GIEL = 0;
	Nop();
	Nop();
}

// ******* PORT PIN INIT's ..................

void init_ports(void){
    LATA = 0;     
    TRISA = 0xFF; /* INPUTS */
//
    LATB = 0;     /* Configure PORT B pi.................... */
    TRISB = 0b11000111; 
/* ICD pins, RB5=USB_CTS, RB4=LCD_EN, RB3=LCD_RW, RB2=PBSW3 ..... */
//
    LATC = 0;
    TRISC = 0b11111110;
/* INPUTS/PLIB config'd ... TST_OUT ... */

	LATD = 0;
    TRISD = 0xFF;   /* ALL INPUTS ... */
//
	LATE = 0;
	TRISE = 0b11111000;
//
// Set "analog select bits" ....
//   1 = Dig inp bufr disabled
//   0 = Dig inp bufr enabled
//
	ANSELA = 0x01;    ANSELB = 0; 
    ANSELC = 0;       ANSELD = 0;   ANSELE = 0;
	Nop();
	Nop();
	Nop();
}

/* *** END of PORT INIT ....... */

/* *** TIMER INITIALIZATION *** */

//
//  TIMER 0 is 16 bit ...
//  TIMER 1 is 8 bit ....
//

// *** INIT USART1 *************

void initUSARTS(void){
/*  USART config ...
 *  ( See datasheet of PIC18F26K22 ...... )
 *
 *  BAUD RATE : 19200 
 *
 *  Parameters:
 *
 *  ............................ */
//
    BAUDCON1bits.BRG16 = 1;
    Open1USART( USART_TX_INT_OFF &
                 USART_RX_INT_ON &
               USART_ASYNCH_MODE &
                 USART_EIGHT_BIT &
                 USART_SINGLE_RX &
                 USART_BRGH_HIGH &
 		         USART_ADDEN_OFF, 207);    /* 19200 baud */
// 				  USART_ADDEN_OFF, 259);  
//
//    BAUDCON1bits.DTRXP = 1;  // Rx data is "inverted" ... Default is zero ....
    Nop();
    Nop();
//
//  Initialize character reception...................
//
    RCSTA1bits.CREN = 1;    // Enable reception .....
    RCSTA2bits.CREN = 1;    // Enable reception .....
	Nop();
	Nop();
	Nop();
}


void init(void){
    Nop();
	Nop();
// *********** OSCCON and OSCTUNE at default of reset ......................
//    OSCCON = 0b01100111;   // 8 MHZ int osc ... (Same as 18F2321 original) 
//    OSCCON = 0b00110111;   // 1 MHZ int osc .... 
	Nop();
	Nop();
//
// OSCCON2 and OSCTUNE at default of reset ....
//
    init_ports();
//
    initUSARTS();
    Nop();
    Nop();
//

/* ********************************************************* */

/* ****     Enable/disable global interrupts   ************* */

/* ****   INTO and INT1 are external hardware  ************* */

/* ****       TIMER 0 and TIMER 1              ************* */

/* ********************************************************* */

	INTCON2bits.TMR0IP = 0; /* TIMER 0 is low priority int   */
    IPR1bits.TMR1IP = 1;    /* TIMER 1 is high priority int  */
/* *** NOTE: Fixed in hardware, INT 0 is high priority int   */
    INTCON3bits.INT1IP = 1; /* INT 1 is high priority int    */

	INTCONbits.TMR0IF = 0;  /* Clear TIMER 0 int flag bit    */
	INTCONbits.TMR0IE = 1;  /* Enable TIMER 0 interrupt      */
	T0CONbits.TMR0ON = 1;   /* Turn TIMER 0 ON               */

	PIR1bits.TMR1IF = 0;    /* Timer 1 ON                    */
	PIE1bits.TMR1IE = 1;    /*                               */
	T1CONbits.TMR1ON = 1;   /*                               */

//	  INTCONbits.INT0IF = 0;
//	  INTCONbits.INT0IE = 1;  /* SSW int                     */
 
    INTCON3bits.INT1IF = 0; // Clear flag bit
    INTCON3bits.INT1IE = 0; /* Cut motor over current int    */
//
//	  IPR1bits.ADIP = 0;      /* A/D conv is low priority int  */
//    PIE1bits.ADIE = 0;      /* A/D int is OFF .............  */	
//
	RCONbits.IPEN = 1;      /* Int priority levels enable    */
//
//  Serial comm int config bits ............................
//
    IPR1bits.RC1IP = 0;     /* LOW priority  ............... */
    PIE1bits.RC1IE = 1;     /* Enable USART1 interrupt ..... */
    PIR1bits.RC1IF = 0;     /* Clear flag bit .............. */
//	
	IPR3bits.RC2IP = 0;     /* LOW priority  ............... */
	PIE3bits.RC2IE = 1;
	PIR3bits.RC2IF = 0;
//
    intrENAB();             /* Enable all interrupts ......  */
	Nop();
	Nop();

/* **********************************************************************
 * *****************      MASTER OUTPUT ENABLE         ******************
 * ********************************************************************** */

	Nop();
    Nop();
	Nop();
	Nop();
	Nop();
}

/*
 *
 * ************************************************************************
 * ****************                           *****************************
 * ****************    END OF INIT FUNCTION   *****************************
 * ****************                           *****************************
 * ************************************************************************ */

