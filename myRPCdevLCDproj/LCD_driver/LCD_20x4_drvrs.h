/* 
 * File:    LCD_20x4_drvrs.h 
 * Author:  R G Fischer, RPC ELECTRONICS
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LCD_20x4_drvrs_H
#define	LCD_20x4_drvrs_H

//
// FUNCTION DECLARATIONS: writeLCDcmd, etc ....
// 

extern void writeLCDcmd(unsigned char cmdBYTE);

extern void writeLCDdata(unsigned char dBYTE);

extern void wait4BFclr(void);

extern unsigned char readLCDdata(unsigned char addrSELECT);

extern void clearLCD(void);

extern void homeLCD(void);

extern void dispLCD(void);

extern void entrymodeLCD(void);

extern void shiftcursorLCDright(void);

extern void shiftcursorLCDleft(void);

extern void initLCD(void);

extern void LCDdisplaySTRING(unsigned char LCDlineCNTR, char *dispSTR);

//
//
extern void LCDshift5to2(void);
extern void LCDshift6to3(void);
extern void LCDshift7to4(void);
extern void LCDshift8to5(void);
//
//
#endif	/* LCD_20x4_drvrs_h */

