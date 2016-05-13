/* 
 *  FILE:    LCD_20x4_drvrs.c
 *  AUTHOR:  RGF 
 *  
 */

#include <xc.h>
// #include <pic18f46k22.h>

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

// LOCAL function for setting data byte to LCD .................
// ( FYI: LCD uses "db" label for data lines ...... )

void dataLCDout(unsigned char dbBYTE){
    unsigned char btemp;
    
    btemp = dbBYTE;
    btemp &= 0xFC;   // ZERO out low bits ......................
    LATD = btemp;    // SET out latch for data bits 7 thru 3 ...
    
    btemp = dbBYTE & 0x02;
    if ( btemp == 0x02 ) LATBbits.LATB2 = 1; else LATBbits.LATB2 = 0;
    btemp = dbBYTE & 0x01;
    if ( btemp == 0x01 ) LATBbits.LATB1 = 1; else LATBbits.LATB1 = 0;  
    Nop();
    Nop();    
}

void writeLCDcmd(unsigned char cmdBYTE){
    LCD_RS = 0;               // "Instruction" reg
    LCD_RW = 0;               // WR mode
    Nop();  Nop();  Nop();    // to be in WR mode ...
//
    TRISD = 0x03;             // SET PORTD to out ...
    TRISBbits.RB2 = 0;
    TRISBbits.RB1 = 0;
    Nop();  Nop();  Nop();
    dataLCDout(cmdBYTE);      // SET latch bits to LCD with "cmdBYTE" ...  
    Nop();  Nop();  Nop();
    Nop();  Nop();  Nop();  
//    
    LCD_EN = 1;     Nop();    // RAISE E signal 
    Delay_msec(1);            // TIME for LCD 
    Nop();  Nop();  Nop();    // to see cmd BYTE ..........
    LCD_EN = 0;               // LOWER E signal
    Nop();  Nop();  Nop(); 
//    
    TRISD = 0xFF;             // RETURN LCD bus to input ...
    TRISBbits.RB2 = 1;
    TRISBbits.RB1 = 1;    
    Nop();  Nop();  Nop();
}

void writeLCDdata(unsigned char dBYTE){
    LCD_RS = 1;               // "Data" reg
    LCD_RW = 0;               // WR mode
    Nop();  Nop();  Nop();    // to be in WR mode ...
//
    TRISD = 0x03;             // SET PORTD to out ...
    TRISBbits.RB2 = 0;
    TRISBbits.RB1 = 0;
    Nop();  Nop();  Nop();
    dataLCDout(dBYTE);        // SET latch bits  ...  
    Nop();  Nop();  Nop();
    Nop();  Nop();  Nop();  
//    
    LCD_EN = 1;     Nop();    // RAISE E signal 
    Delay_msec(1);            // TIME for LCD 
    Nop();  Nop();  Nop();    // to see data byte ..........
    LCD_EN = 0;               // LOWER E signal
    Nop();  Nop();  Nop(); 
//    
    TRISD = 0xFF;             // RETURN LCD bus to input ...
    TRISBbits.RB2 = 1;
    TRISBbits.RB1 = 1;   
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
    LCD_RW = 0;               // LCD no output ........
    TRISD = 0xFF;             // Set PORTD to input ...    
    TRISBbits.RB2 = 1;
    TRISBbits.RB1 = 1;
    Nop();   Nop();   Nop();  // Set lines ............
    LCD_RW = 1;               // LCD output ...........
    Nop();   Nop();   Nop();
    LCD_EN = 1;
    Nop();   Nop();   Nop();   
    btemp = PORTD;            // Read PORTD, db lines from LCD ...
    if (LCDdb1 == 1) btemp |= 0x02; else btemp &= 0xFD;
    if (LCDdb0 == 1) btemp |= 0x01; else btemp &= 0xFE;
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
    LCD_RS = 1;               // Read data req'd ......
    LCD_RW = 0;  
    LCD_EN = 0;  
    TRISD = 0xFF;             // Set PORTD to input ...    
    TRISBbits.RB2 = 1;
    TRISBbits.RB1 = 1;
    Nop();   Nop();   Nop();  // Set lines ............   
    LCD_RW = 1;               // Allow LCD to output to LCD "data bus" ....
    Nop();   Nop();   Nop(); 
    LCD_EN = 1;               // Start a data read ....
    Nop();   Nop();   Nop();   
    btemp = PORTD;            // Read PORTD, db lines from LCD ...
    if (LCDdb1 == 1) btemp |= 0x02; else btemp &= 0xFD;
    if (LCDdb0 == 1) btemp |= 0x01; else btemp &= 0xFE;
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

// void setDDRAMaddr(void){
//    writeLCDcmd(0x80);
//    Nop();
// }

//
// READ BUSY FLAG & ADDR ...
//
//  RS  R/W  DB7 DB6 DB5 DB4  DB3 DB2 DB1 DB0
//  0   1    BF  AC  AC  AC   AC  AC  AC  AC
//


//  RS  R/W  DB7 DB6 DB5 DB4  DB3 DB2 DB1 DB0
//  0   0    1   ADD ADD ADD  ADD ADD ADD ADD


//  FUNCTION initLCD  --- EIGHT bit interface .................
//
//  RS  R/W  DB7 DB6 DB5 DB4  DB3 DB2 DB1 DB0
//  0   0    0   0   1   1    x   x   x   x     (0x30)
//
//  0   0    0   0   1   1    x   x   x   x
//
//  0   0    0   0   1   1    N   F   x   x     (0x30)  FUNCTION SET
//  0   0    0   0   0   0    1   0   0   0     (0x08)  DISPLAY OFF 
//  0   0    0   0   0   0    0   0   0   1     (0x01)  DISPLAY CLEAR
//  0   0    0   0   0   0    0   1   I/D S     (0x06)  FUNCTION SET
//
//  END of initLCD 

void initLCD(void){
    byte btemp;
    
    TRISD = 0xFF;      // Set PORTD to INPUT ...
    LCD_EN = 0;    LCD_RW = 0;     LCD_RS = 0; 
    BACKLIGHT = 0;
//
    LCD_PWR = 1;     // POWER on ... (briefly?) ......
    Delay_usec(20); 
    LCD_PWR = 0;       
    Delay_usec(100); 
//    
    LCD_PWR = 1;     
    Delay_usec(100); 
    LCD_PWR = 0;       
    Delay_usec(100);     
//
    LCD_PWR = 1;     
    Delay_msec(10); 
    LCD_PWR = 0;
    Delay_msec(5);  
    LCD_PWR = 1;     // POWER on ..... 
    Delay_msec(50);   
 //    
    BACKLIGHT = 1;   // Power stable ? ... No false reset ? .....   
    Delay_msec(10); 
    Nop();
    Nop();    
//    
    dataLCDout(0x30);              // INIT state, PORTD latch bits ....
//
    for (btemp=0; btemp<6; btemp++)  Delay_msec(20);   
    Nop();  Nop();  Nop(); 
    Nop();  Nop();  Nop();         // WAKE up time ...
 
    writeLCDcmd(0x38);             // OR 0x30 ????
    Delay_msec(20);    
    Nop();   Nop();   Nop();     
 //
    writeLCDcmd(0x38);             // DO again ...
    Delay_msec(20);    
    Nop();   Nop();   Nop();   
//
    writeLCDcmd(0x38);             // DO again ...
    Delay_msec(20);   
    Nop();   Nop();   Nop();   
//    
    writeLCDcmd(0x08);             // 
    Delay_msec(20);     
    Nop();   Nop();   Nop();   
//      
    writeLCDcmd(0x01);             // 
    Delay_msec(20);    
    Nop();   Nop();   Nop();   
//    
    writeLCDcmd(0x06);             // 
    Delay_msec(20);    
    Nop();   Nop();   Nop();   
//      
    homeLCD();
    dispLCD();
    clearLCD();
    Nop();
    Nop();
 }

void LCDdisplaySTRING(unsigned char LCDlineCNTR, char *dispSTR){
    byte indexCNTR;
    static byte btemp1, btemp2, btemp3;          // "Reads" of "DDRAM" ....
    static byte addrCNTR1, addrCNTR2, addrCNTR3;
    
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
    
    indexCNTR = 0;
    LCDdisplayLOOP: if (dispSTR[indexCNTR] == 0) goto exitLCDdisplayLOOP;
        btemp1 = readBFaddrCNTR();
        addrCNTR1 = 0x7F & btemp1;    // Get initial "DDRAM AC" ...
        Nop();   Nop();   Nop();
        wait4BFclr();                 // NOT busy, proceed ...
        writeLCDdata(dispSTR[indexCNTR]);
        Nop();   Nop();   Nop();
        
        btemp2 = readBFaddrCNTR();
        addrCNTR2 = 0x7F & btemp2;    // Get initial "DDRAM AC" ...
        Nop();   Nop();   Nop();
        indexCNTR++;
        wait4BFclr();                 // NOT busy, proceed ...
        writeLCDdata(dispSTR[indexCNTR]);
                    
        btemp3 = readBFaddrCNTR();
        addrCNTR3 = 0x7F & btemp3;    // Get initial "DDRAM AC" ...
        Nop();   Nop();   Nop();
        indexCNTR++;
        wait4BFclr();                 // NOT busy, proceed ...
        writeLCDdata(dispSTR[indexCNTR]);
    
    /* Note that addrCNTRx are prior to writes to LCD ....   */
        
        Nop();   
        Nop();
        Nop();
     
        btemp1 = readLCDdata(addrCNTR1);
        btemp2 = readLCDdata(addrCNTR2);
        btemp3 = readLCDdata(addrCNTR3);
        
    /* Note that btempx contains DDRAM values at addrCNTRx ....   */  
        
        Nop();
        Nop();
        errorLOOP: Nop();
            Nop();
            Nop();
            goto errorLOOP;
    
    exitLCDdisplayLOOP: Nop();    
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