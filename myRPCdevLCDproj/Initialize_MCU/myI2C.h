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

#define I2C1_SCL	TRISCbits.TRISC3
#define I2C1_SDA	TRISCbits.TRISC4

void I2C_Init(unsigned char clock_output);
void I2C_Start(void);
void I2C_RepeatedStart(void);
void I2C_Stop(void);

void I2C_Idle(void);
void I2C_Close(void);

unsigned char I2C_ReadByte(unsigned char acknowledge);
unsigned char I2C_WriteByte(unsigned char data_out);

#endif	/* XC_myI2C_H */

