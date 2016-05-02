/* 
 * File:    myI2C.h 
 * Author:  RGF 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_myI2C_H
#define	XC_myI2C_H

#include <xc.h>             // .......  
#include <pic18f46k22.h>

// #define I2C1_SCL	TRISCbits.TRISC3  // SEE initialization.h 
// #define I2C1_SDA	TRISCbits.TRISC4

extern void I2C_Init(unsigned char clock_output);
extern void I2C_Start(void);
extern void I2C_RepeatedStart(void);
extern void I2C_Stop(void);

extern void I2C_Idle(void);
extern void I2C_Close(void);

extern unsigned char I2C_ReadByte(unsigned char ackBIT);
extern unsigned char I2C_WriteByte(unsigned char data_out);

#endif	/* XC_myI2C_H */

