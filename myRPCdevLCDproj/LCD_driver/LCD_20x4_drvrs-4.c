/* 
 *  FILE:    LCD_20x4_drvrs.c
 *  AUTHOR:  RGF 
 *  
 */

#include <xc.h>

#include "Initialize_MCU/configMCU.h"
#include "Initialize_MCU/initialization.h"
#include "Initialize_MCU/myDELAYS.h"

#include "LCD_20x4_drvrs.h"

extern unsigned char bdum;
extern unsigned int idum;

extern char dispSTR1[];
extern char dispSTR2[];
extern char dispSTR3[];
extern char dispSTR4[];
extern char dispSTR5[];
extern char dispSTR6[];
extern char dispSTR7[];
extern char dispSTR8[];

/* ..... LOCAL FUNCTIONS start here .................... */

void setMCUoutLCD(void){
    TRISD = 0b00011011;   /* Set PORT D lines for OUTPUT */  
    TRISBbits.RB5 = 0;
    TRISBbits.RB2 = 0;
    TRISBbits.RB1 = 0;
    TRISCbits.RC5 = 0;   
    Nop();
    Nop();   
}

void setMCUinpLCD(void){
    TRISD = 0xFF;         /* Set PORT D lines for INPUT */  
    TRISBbits.RB2 = 1;
    TRISBbits.RB1 = 1;  
    TRISCbits.RC5 = 1;
    TRISBbits.RB5 = 1;    
    Nop();
    Nop();
}

void strobeLCDenable(void){
    LCD_EN = 1;     Nop();    // RAISE E signal 
    Delay_msec(1);            // TIME for LCD 
    Nop();  Nop();  Nop();    // to see cmd BYTE ..........
    LCD_EN = 0;               // LOWER E signal
    Nop();  Nop();  Nop();     
}

/*  ********
 *  ***  LOCAL function for setting data byte to LCD ........
 *  ( FYI: LCD uses "db" label for data lines ...... )   .... */

void dataLCDout(unsigned char dbBYTE){
    unsigned char btemp;
    
    btemp = dbBYTE;
    btemp &= 0xE4;   /* SELECT four bits 
                      * of param byte for LATD
                      * See schematic ... 
                      * ... NOT bits 4, 3, 1 and 0 -> 0xE4 .......... */
    LATD = btemp;    /* SET LATD accordingly ........................ */
    
    btemp = dbBYTE & 0x01;
    if ( btemp == 0x01 ) LATB1 = 1; else LATB1 = 0;   
    btemp = dbBYTE & 0x02;
    if ( btemp == 0x02 ) LATB2 = 1; else LATB2 = 0;
    btemp = dbBYTE & 0x08;
    if ( btemp == 0x08 ) LATB5 = 1; else LATB5 = 0;
    btemp = dbBYTE & 0x10;
    if ( btemp == 0x10 ) LATC5 = 1; else LATC5 = 0;    
    Nop();
    Nop();    
}

/* ..... END LOCAL FUNCTIONS .................... */

void writeLCDcmd(unsigned char cmdBYTE){
    LCD_RS = 0;               // "Instruction" reg select ...
    LCD_RW = 0;               // WR mode ( MCU writing to LCD )
    Nop();  Nop();  Nop();    // Time to be in WR mode ......
//
    setMCUoutLCD();
    Nop();  Nop();  Nop();
    dataLCDout(cmdBYTE);      // SET latch bits to LCD with "cmdBYTE" ...  
    Nop();  Nop();  Nop();
    Nop();  Nop();  Nop();  
//   
    strobeLCDenable();
    Nop();  Nop();  Nop(); 
//  
    setMCUinpLCD();
    Nop();  Nop();  Nop();
}

void writeLCDdata(unsigned char dBYTE){
    LCD_RS = 1;               // "Data" reg
    LCD_RW = 0;               // WR mode
    Nop();  Nop();  Nop();    // to be in WR mode ...
//
    setMCUoutLCD();
    Nop();  Nop();  Nop();
    dataLCDout(dBYTE);        // SET latch bits  ...  
    Nop();  Nop();  Nop();
    Nop();  Nop();  Nop();  
//    
    strobeLCDenable();
    Nop();  Nop();  Nop(); 
//  
    setMCUinpLCD(); 
    Nop();  Nop();  Nop();
}

/* 
 * LOCAL function to "Read Busy flag and address"
 * 
 * NOTES:
 *   1. LCD_RS is low 
 *   2. Returned byte is BF (bit 7) and addr cntr (bits 6 thru 0)
 *  .........................................................   */

unsigned char readBFaddrCNTR(void){
    byte btemp;
    
    LCD_RS = 0;               // Set low to read BF and addr cntr ...
    LCD_RW = 0;               // LCD no output ......................
    setMCUinpLCD();           // Set MCU port pins to input .........
    Nop();   Nop();   Nop();  // Time for lines to be set ...........
    LCD_RW = 1;               // Enable LCD output to MCU ...........
    Nop();   Nop();   Nop();
    LCD_EN = 1;
    Nop();   Nop();   Nop();   
    btemp = PORTD;            // Read PORTD, db lines from LCD ......
    if (LCDdb0 == 1) btemp |= 0x01; else btemp &= 0xFE; 
    if (LCDdb1 == 1) btemp |= 0x02; else btemp &= 0xFD;
    if (LCDdb3 == 1) btemp |= 0x08; else btemp &= 0xF7; 
    if (LCDdb4 == 1) btemp |= 0x10; else btemp &= 0xEF;
    LCD_EN = 0;
    LCD_RW = 0;               // LCD no output ....
    Nop();
    Nop(); 
    return btemp;
}

/* FUNCTION: wait4BFclr
 * 
 * PURPOSE:  Function exits when BF is clear ...
 *   ...........................................   */

void wait4BFclr(void){
    byte bchk;
    
    BFchk:  bchk = readBFaddrCNTR(); 
        Nop(); Nop();  Nop();
        bchk &= 0x80;
        if (bchk == 0x80) goto BFchk;
    Nop();
    Nop();
}

/* 
 * LOCAL function to "set DDRAM addr" ...
 * ...................................... */

void setDDRAMaddr(unsigned char addrBYTE){
    addrBYTE |= 0x80;   
    writeLCDcmd(addrBYTE);
    Nop();
    Nop();    
}

/* LOCAL function to "Read data from RAM" ................
 * NOTES:    ...  */

unsigned char readLCDdata(unsigned char addrSELECT){
    byte btemp;
 
    setDDRAMaddr(addrSELECT);
    LCD_RS = 1;               // Read data req'd ......................
    LCD_RW = 0;  
    LCD_EN = 0;  
    setMCUinpLCD();           // Set MCU port pins to input ...........
    Nop();   Nop();   Nop();  // Time (and debug) allowance ...........   
    LCD_RW = 1;               // Allow LCD to output to "data bus" ....
    Nop();   Nop();   Nop(); 
    LCD_EN = 1;               // Start a data read ....................
    Nop();   Nop();   Nop();   
    btemp = PORTD;            // Read PORTD, db lines from LCD ......
    if (LCDdb0 == 1) btemp |= 0x01; else btemp &= 0xFE; 
    if (LCDdb1 == 1) btemp |= 0x02; else btemp &= 0xFD;
    if (LCDdb3 == 1) btemp |= 0x08; else btemp &= 0xF7; 
    if (LCDdb4 == 1) btemp |= 0x10; else btemp &= 0xEF;
    LCD_EN = 0;               // End of data read .....
    LCD_RW = 0;               // Disable LCD output ...
    Nop();
    Nop();   
    return btemp;    
}

void clearLCD(void){
    LCD_RS = 0;   LCD_RW = 0;
    wait4BFclr();
    writeLCDcmd(1);
    Nop();
    Nop();
}

void homeLCD(void){
    wait4BFclr(); 
    writeLCDcmd(2);
    Nop(); 
    Nop();
}

void dispLCD(void){
     wait4BFclr();
     writeLCDcmd(0x0F);   // Entire display ON, cursor ON, cursor position ON ....
     Nop();
     Nop();   
}

void entrymodeLCD(void){
    wait4BFclr(); 
    writeLCDcmd(0x07);
    Nop();
}

void shiftcursorLCDright(void){
    wait4BFclr();  
    writeLCDcmd(0x14);           // 0001 S/C R/L x x 
}

void shiftcursorLCDleft(void){
     wait4BFclr(); 
     writeLCDcmd(0x10);           // 0001 S/C R/L x x    
}

/*  ***********************************************************
 *  ***********************************************************
 * 
 *  FUNCTION: initLCD
 * 
 *  NOTES:
 *  [ LCD PWR observed in 50 usec ... ]
 * 
 *  BIT FORMAT:  READ BUSY FLAG & ADDR ...
 *
 *  RS  R/W  DB7 DB6 DB5 DB4  DB3 DB2 DB1 DB0
 *   0   1    BF  AC  AC  AC   AC  AC  AC  AC
 * 
 *  RS  R/W  DB7 DB6 DB5 DB4  DB3 DB2 DB1 DB0
 *   0   0    1  ADR ADR ADR  ADR ADR ADR ADR
 * 
 *  FUNCTION initLCD  --- EIGHT bit interface .................
 * 
 *  RS  R/W  DB7 DB6 DB5 DB4  DB3 DB2 DB1 DB0
 *   0   0    0   0   1   1    x   x   x   x     (0x30)
 * 
 *   0   0    0   0   1   1    x   x   x   x
 * 
 *   0   0    0   0   1   1    N   F   x   x     (0x30)  FUNCTION SET
 *   0   0    0   0   0   0    1   0   0   0     (0x08)  DISPLAY OFF 
 *   0   0    0   0   0   0    0   0   0   1     (0x01)  DISPLAY CLEAR
 *   0   0    0   0   0   0    0   1   I/D S     (0x06)  FUNCTION SET
 * 
 *  .........  END of initLCD
 * 
 *  ***********************************************   */

void initLCD(void){
    unsigned char btemp;
    
    LCD_EN = 0;    LCD_RW = 0;     LCD_RS = 0; 
    
    LCD_PWR = 1;     /* LCD POWER ON ............................... */
    Nop();  Nop();  Nop();  
    BACKLIGHT = 1;   /* Is Power stable ? ... No false reset ? ..... */ 
   
    setMCUoutLCD();       /* Set MCU port pins for OUTPUT    .... */ 
/*  
 *  SET DB7 thru DB0 as noted above ( 0x30 ) ....................
 *  NOTE that this is not "dataLCDout" function ................. 
 *  ............................................................. */ 
    
    LATB1 = 0;            /* INITIAL set of LCD "db" inputs ..... */
    LATB2 = 0;
    LATD2 = 0;
    LATB5 = 0;
    
    LATC5 = 1;
    LATD5 = 1;
    LATD6 = 0;
    LATD7 = 0;

    Nop();                    /* DEBUG */
    Nop();
    Nop();    
  
 /* ***
  * *** HARDWARE DEBUG -- MCU to LCD wiring .......
  * ***       
    dataLCDout(0xAA);
    Nop();
    Nop();
    Nop();
    dataLCDout(0x55);
    Nop();
    Nop();
    Nop();
  ***    END of DEBUG .............................  */
   
    Delay_msec(100);          /* .... tbd ... NEEDED? .... */
    Nop();  
    Nop();
    Nop();    
     
    for (btemp=0; btemp<4; btemp++){
            strobeLCDenable();
    Delay_msec(20);           /* Data sheet "wait time > 4.1 msec" ... */
    Nop();  Nop();  Nop();  
        
        
    }
    LATB5 = 1;                /* CHANGE MCU output to 0x38 ... */
    Nop();  Nop();  Nop();   
/*
 *  "STROBE" the enable ... MCU pins in OUTPUT ...............
 */
    strobeLCDenable();
    Delay_msec(20);           /* Data sheet "wait time > 4.1 msec" ... */
    Nop();  Nop();  Nop();  
    
    strobeLCDenable();
    Delay_msec(20);           /* Data sheet "wait time > 100 usec" ... */
    Nop();  Nop();  Nop(); 
    
    strobeLCDenable();
    Delay_msec(20);           /* NO WAIT time specified here ....... */
    Nop();  Nop();  Nop(); 
 
 /* Effectively, the CMD 0x38 has been strobed out THREE times ..... 
  * Now change to 0x08 ...
  *                                                                  */
    LATB5 = 1;   LATC5 = 0;   LATD5 = 0;
    Nop();   Nop();   Nop();    

    strobeLCDenable();             /*  Write CMD 0x08   */
    Delay_msec(20);     
    Nop();   Nop();   Nop();   
//   
    LATB5 = 0;   LATB1 = 1; 
    Nop();   Nop();   Nop();   
   
    strobeLCDenable();             /*  Write CMD 0x01   */ 
    Delay_msec(20);    
    Nop();   Nop();   Nop();   
//    
    LATB1 = 0;   LATB2 = 1;    LATD2 = 1; 
    Nop();   Nop();   Nop();
    
    strobeLCDenable();             /*  Write CMD 0x06   */  
    Delay_msec(20);    
    Nop();   Nop();   Nop();   
//      
    homeLCD();
    dispLCD();
    clearLCD();
    Nop();
    Nop();
    Nop();
  
 /* ***
  * *** HARDWARE/SOFTWARE DEBUG HERE ............
  * *** IS the LCD properly initialized? ........
  * *** IS written data actually in "DDRAM" ? ...
  *                                                 */  
//    btemp = readLCDdata(0);
//    Nop();
//    Nop();
//    Nop();
    
    wait4BFclr();
    writeLCDdata(0x52);
    Nop();
    Nop();   
    Nop();
    
    btemp = readLCDdata(0);
    Nop();
    Nop();
    Nop();   
    
    
 }

/*  *** END of initLCD function ..............................   */

void LCDdisplaySTRING(unsigned char LCDlineCNTR, char *dispSTR){
    byte btemp;  
    
    wait4BFclr();                  // NOT busy, proceed ....
    switch (LCDlineCNTR){
        case 1 : writeLCDcmd(0x80);
        break;
        
        case 2 : writeLCDcmd(0xC0);
        break;
        
        case 3 : writeLCDcmd(0x94);
        break;          
        
        case 4 : writeLCDcmd(0xD4);
        break; 
        
        default : writeLCDcmd(0x80);
        break;
    }; 
 //
    Nop();
    Nop();
    
    btemp = 0;
    while(dispSTR[btemp]!=0){
        wait4BFclr();                  // NOT busy, proceed ....
        writeLCDdata(dispSTR[btemp]);
        btemp++;
        Nop();
        Nop();
        Nop();
    }
    Nop();
    Nop();
    Nop();
}

//
//  The following create a pseudo screen "UP" effect
//  to the displayed lines .............................
//
void LCDshift5to2(void){
        LCDdisplaySTRING(1, dispSTR2);
        LCDdisplaySTRING(2, dispSTR3);     
        LCDdisplaySTRING(3, dispSTR4);
        LCDdisplaySTRING(4, dispSTR5);
}

void LCDshift6to3(void){
        LCDdisplaySTRING(1, dispSTR3);
        LCDdisplaySTRING(2, dispSTR4);     
        LCDdisplaySTRING(3, dispSTR5);
        LCDdisplaySTRING(4, dispSTR6);    
//       
}

void LCDshift7to4(void){
        LCDdisplaySTRING(1, dispSTR4);
        LCDdisplaySTRING(2, dispSTR5);     
        LCDdisplaySTRING(3, dispSTR6);
        LCDdisplaySTRING(4, dispSTR7);    
//       
}

void LCDshift8to5(void){
        LCDdisplaySTRING(1, dispSTR5);
        LCDdisplaySTRING(2, dispSTR6);     
        LCDdisplaySTRING(3, dispSTR7);
        LCDdisplaySTRING(4, dispSTR8);    
//       
}