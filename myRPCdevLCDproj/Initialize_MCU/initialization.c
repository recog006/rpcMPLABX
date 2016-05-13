/*
 * MODULE   : initialization.c
 * 
 *      */

#include <xc.h>

#include <plib\delays.h>
#include <plib\usart.h>
#include <plib\pconfig.h>

#include "initialization.h"

extern unsigned char bdum;
extern unsigned int idum;


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
    TRISB = 0b11000001; 
/*  ICD pins,   RB5=USB_CTS,   RB4=LCD_EN,
 *  RB3=LCD_RW, RB2=LCD_DB1,   RB1=LCD_DB0, RB0=RTC_INT .... */
//
    LATC = 0;
    TRISC = 0b11111110;
/* INPUTS/PLIB config'd ... RC5=USB_RTS ... TST_OUT ... */

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
 		         USART_ADDEN_OFF, 207);    /* ... 19200 baud ..... */
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
    
    USB_CTS = 0;   // Indicates MCU can accept data ......
}


void init(void){
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
	INTCONbits.TMR0IE = 0;  /* Enable TIMER 0 interrupt      */
	T0CONbits.TMR0ON = 0;   /* Turn TIMER 0 ON               */

	PIR1bits.TMR1IF = 0;    /* Timer 1 ON                    */
	PIE1bits.TMR1IE = 0;    /*                               */
	T1CONbits.TMR1ON = 0;   /*                               */

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
    IPR1bits.RC1IP = 1;     /* HIGH priority  .............. */
    PIE1bits.RC1IE = 1;     /* USART1 interrupt ..... */
    PIR1bits.RC1IF = 0;     /* Clear flag bit .............. */
//	
	IPR3bits.RC2IP = 0;     /* LOW priority  ............... */
	PIE3bits.RC2IE = 0;
	PIR3bits.RC2IF = 0;
//
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

