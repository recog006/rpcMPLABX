/* 
 * File:     DS3231.h 
 * Author:   RGF 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_DS3231_H
#define	XC_DS3231_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define MODE_12HOUR_AM  0x40
#define MODE_12HOUR_PM  0X60
#define SECONDS         0x00
#define MINUTES         0x01
#define HOURS           0x02
#define DAY             0x03
#define DATE            0x04
#define MONTH           0x05
#define YEAR            0x06

//
// Function Prototypes declarations .....
// 
extern void DS3231_AMPM(void);
extern void DS3231_HourMode(void);
extern void DS3231_AddData(unsigned char byte0);
extern void DS3231_GetCalendar(unsigned char *p1);
extern void DS3231_SetInfo(unsigned char byte0, unsigned char byte1);

extern unsigned char DS3231_GetTime(unsigned char *byte0);
extern unsigned char DS3231_GetInfo(unsigned char byte0);
extern unsigned char BCDToDecimal (unsigned char byte0);
extern unsigned char DecimalToBCD (unsigned char byte0);

#endif	/* XC_DS3231_H */

