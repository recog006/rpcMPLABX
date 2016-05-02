//
//  MODULE   : initialization.h
//
//
#ifndef INITIALIZATION_H
#define INITIALIZATION_H

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

// OUTPUTS
#define LCD_PWR   LATEbits.LATE0
#define BACKLIGHT LATEbits.LATE1
#define LCD_RS    LATEbits.LATE2
//
// PORTD is output to LCD ......
//
#define LCD_PORT  LATD

#define LCD_RW    LATBbits.LATB3
#define LCD_EN    LATBbits.LATB4

/* *****  EXPORTED FUNCTION PROTOTYPES **** */
// Function prototypes of non-static functions

extern void intrENAB(void);
extern void intrDISAB(void);

// extern void init_ports(void);
extern void initUSARTS(void);     // USART1 used via USB .......

extern void init(void);

#endif