/*
 *
 *  FILE:    initialization.h
 *  AUTHOR:  RGF
 * 
 *  
 */

//
//
#ifndef XC_INITIALIZATION_H
#define XC_INITIALIZATION_H

/* ************************************************************************
 * ************************************************************************
 * ******                                         *************************
 * ******  DEFINITIONS -- CONSTANTS -- GLOBAL     *************************
 * ******                                         *************************
 * ************************************************************************ */

typedef unsigned char  byte;     //  8 bits ....
typedef unsigned int   word;     // 16 bits ....
typedef unsigned long dword;

/* ************************************************************************
 * ********
 * ********    DEFINITIONS -- HARDWARE
 * ********
 * ************************************************************************ */

// INPUTS
#define PBSW1   PORTBbits.RB0   // PB sw 1  - "SELECT"
#define PBSW2   PORTBbits.RB1   // PB sw 2  - "ADD data"
#define PBSW3   PORTBbits.RB2   // PB sw 3  - "AM / PM"

#define USB_RTS PORTCbits.RC5   // USB_RTS signal from USB adapter

// OUTPUTS ..................................
#define USB_CTS   LATBbits.LATB5

#define LCD_RW    LATBbits.LATB3
#define LCD_EN    LATBbits.LATB4

#define TST_OUT   LATCbits.LATC0

#define LCD_PWR   LATEbits.LATE0
#define BACKLIGHT LATEbits.LATE1
#define LCD_RS    LATEbits.LATE2
//
// LCD data lines are
// PORTD bits 7 thru 3
// LCD data bit 0 is RB1
// LCD data bit 1 is RB2
//
#define LCDdb0    PORTBbits.RB1
#define LCDdb1    PORTBbits.RB2
//

// I2C CONTROL ................................
#define I2C1_SCL	TRISCbits.TRISC3
#define I2C1_SDA	TRISCbits.TRISC4


/* *****  EXPORTED FUNCTION PROTOTYPES **** */
extern void intrENAB(void);
extern void intrDISAB(void);

extern void init_ports(void);
extern void initUSARTS(void);   

extern void init(void);

#endif