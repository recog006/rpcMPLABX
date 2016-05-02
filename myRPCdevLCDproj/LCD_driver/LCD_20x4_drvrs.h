/* 
 * File:    LCD_20x4_drvrs.h 
 * Author:  R G Fischer, RPC ELECTRONICS
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LCD_20x4_drvrs_h
#define	LCD_20x4_drvrs_h

// FUNCTION: writeLCDcmd
// INPUT:    cmd 

void writeLCDcmd(char cmd);

void writeLCDdata(char btemp);

void wait4BFclr(void);

void clearLCD(void);

void homeLCD(void);

void dispLCD(void);

void entrymodeLCD(void);

void shiftcursorLCDright(void);

void shiftcursorLCDleft(void);

void initLCD(void);

void LCDdisplaySTRING(char *dispSTR);

void LCDterm(char LCDlineCNTR);
//
//
//
void LCDshift5to2(void);
void LCDshift6to3(void);
void LCDshift7to4(void);
void LCDshift8to5(void);
//
//
#endif	/* LCD_20x4_drvrs_h */

