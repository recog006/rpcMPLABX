/* 
 * File:     DS3231.h 
 * Author:   RGF 
 * Comments:
 * Revision history:
 * 
 * NOTES:
 *   1. See MaximSemi DS3231 data sheet
 *   2. The DS3231 has a fixed slave addr of 0xD0
 *   3. The digital temp sensor (+- 3 deg C accuracy)
 *      and INT/SQW output (two time-of-day alarms)
 *   4. Can use "fast" 400 KHZ I2C interface (here 
 *   5. ... tbd ....
 *  
 */

#ifndef XC_DS3231_H
#define	XC_DS3231_H

// ( SEE the MaximSemi DS3231 data sheets p. 11 ) 
// DS3231 REG ADDR MAPPING definitions ..........................

#define SECONDS         0x00   // "BCD" value 0 to 59 only
#define MINUTES         0x01   // "BCD" value 0 to 59 only
#define HOURS           0x02   // Depends on 12 or 24 hour mode
#define DAY             0x03   // 1 to 7 only
//
#define DATE            0x04   // "BCD" value 0 to 31 (rules?)  ?
#define MONTH           0x05   // 1 to 12 with "century bit"    ?
#define YEAR            0x06   // 0 to 99                       ?             

#define MODE_12HOUR_AM  0x40
#define MODE_12HOUR_PM  0X60

#define ALARM1_SECONDS  0x07   // Alarm 1 seconds reg addr
#define ALARM1_MINUTES  0x08   // Alarm 1 minutes reg addr
// Additional alarm regs ... HERE ? ..............................

#define RTC_CNTRL       0x0E   // Control reg
#define MSB_TEMP        0x11   // Most sig byte of temp reg
#define LSB_TEMP        0x12   // Least sig byte of temp reg

//
// Function Prototypes declarations .....
// 
extern unsigned char initRTCmodule(unsigned char devADDR);

extern void DS3231_SetInfo(unsigned char byte0, unsigned char byte1);
extern unsigned char DS3231_GetInfo(unsigned char byte0);

extern void DS3231_IncrData(unsigned char selectBYTE);

extern void DS3231_AMPM(void);
extern void DS3231_HourMode(void);

extern unsigned char DS3231_GetTime(unsigned char *RTCbuf);
extern void DS3231_GetCalendar(unsigned char *RTCbuf);

extern unsigned char BCDToDecimal (unsigned char byte0);
extern unsigned char DecimalToBCD (unsigned char byte0);

#endif	/* XC_DS3231_H */

