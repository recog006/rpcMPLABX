/* 
 *  FILE:    LCD_20x4_drvrs.c
 *  AUTHOR:  RGF 
 *  
 */

//
//
#include <xc.h>              // Everywhere .... 
#include <pic18f46k22.h>

#include "Initialize_MCU/configMCU.h"
#include "Initialize_MCU/initialization.h"
#include "Initialize_MCU/myDELAYS.h"

#include "LCD_20x4_drvrs.h"

extern byte bdum;
extern word idum;

char dispSTR1[];
char dispSTR2[];
char dispSTR3[];
char dispSTR4[];
char dispSTR5[];
char dispSTR6[];
char dispSTR7[];
char dispSTR8[];


// LCD op BIT notes  .................

// D=0 (disp off)
// C=0 (cursor off)  B=0 (blink off)
// DL=1 (8 bit interface)
// N=0  ( 1 line disp? )
// F=0  (5x8 dot char font)
// I/D=1 (incr by 1)
// S=0  (no shift)


// WRITE (LCD)
// 1. SET RS (high or low), CLR R/W (write mode)
// 2. SET E (high)
// 3. LOAD DB0-DB7
//    (DATA valid for Tdsw > 40 nsec )
// 4. CLR E (low)
// 5. Return to step 1 ...
//
//
// READ (LCD)
// 1. SET RS (high or low), SET R/W (read mode)
// 2. SET E (high)
// 3. LOAD DB0-DB7
//    (DATA valid for Tdsw > 40 nsec )
// 4. CLR E (low)
// 5. Return to step 1 ...
//

// FUNCTION: writeLCDcmd
// INPUT:    cmd 

void writeLCDcmd(char cmd){
    LCD_RS = 0;
    LCD_RW = 0;               // WR mode
    Nop();  Nop();  Nop();    // to be in WR mode ...
//
    TRISD = 0;                // SET PORTD to out ...
    Nop();  Nop();  Nop();
    LCD_PORT = cmd;           // SET latch bits to "cmd" ...  
    Nop();  Nop();  Nop();
//    
    LCD_EN = 1;     Nop();    // RAISE E signal 
    Delay_msec(5);            // TIME for LCD 
    Nop();  Nop();  Nop();    // to see cmd .......
    LCD_EN = 0;               // LOWER E signal
    Nop();  Nop();  Nop(); 
//    
    TRISD = 0xFF;             // RETURN LCD bus to input ...
    Nop();  Nop();  Nop();
}

void writeLCDdata(char btemp){
    LCD_RS = 1;
    LCD_RW = 0;               // WR mode
    Nop();  Nop();  Nop();    // to be in WR mode ...
//
    TRISD = 0;                // SET PORTD to out ...
    Nop();  Nop();  Nop();
    LCD_PORT = btemp;         // SET latch bits  ...  
    Nop();  Nop();  Nop();
//    
    LCD_EN = 1;     Nop();    // RAISE E signal 
    Delay_msec(5);            // TIME for LCD 
    Nop();  Nop();  Nop();    // to see cmd .......
    LCD_EN = 0;               // LOWER E signal
    Nop();  Nop();  Nop(); 
//    
    TRISD = 0xFF;             // RETURN LCD bus to input ...
    Nop();  Nop();  Nop();
}

void wait4BFclr(void){
    byte btemp;
    
    TRISD = 0xFF;             // Set PORTD to input ...   
    LCD_RS = 0; 
    LCD_RW = 1;  Nop();  Nop();

    BFchk: Nop();
    LCD_EN = 1;  Nop();  Nop();
    btemp = PORTD;
    btemp &= 0x80;
    LCD_EN = 0;
    if (btemp == 0x80) goto BFchk;
    Nop();
}

void clearLCD(void){
    writeLCDcmd(1);
    Nop();
}

void homeLCD(void){
    writeLCDcmd(2);
    Nop();   
}

void dispLCD(void){
     writeLCDcmd(0x0F);  
     Nop();   
}

void entrymodeLCD(void){
    writeLCDcmd(0x07);
    Nop();
}

void shiftcursorLCDright(void){
    writeLCDcmd(0x14);           // 0001 S/C R/L x x 
}

void shiftcursorLCDleft(void){
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
    word itemp;
    
    TRISD = 0xFF;                   // Set PORTD to INPUT ...
//    
    LCD_EN = 0;    LCD_RW = 0;     LCD_RS = 0;
    LCD_PWR = 1;   BACKLIGHT = 1;   // POWER on .....
    LCD_PORT = 0b00110000;              // INIT state, PORTD latch bits ....
//
    for (btemp=0; btemp<6; btemp++)  Delay_msec(20);   
    Nop();  Nop();  Nop(); 
    Nop();  Nop();  Nop();          // WAKE up time ...
 
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
    Nop();
    Nop();
    
    wait4BFclr();
    homeLCD();
    wait4BFclr();
    dispLCD();
    wait4BFclr();
 //
    Nop();    
 }

void LCDdisplaySTRING(char *dispSTR){
    byte btemp;
    
    btemp = 0;
    while(dispSTR[btemp]!=0){
        wait4BFclr();                  // NOT busy, proceed ....
        writeLCDdata(dispSTR[btemp]);
        btemp++;
    }
    Nop();
}

//
// FUNCTION: LCDterm
// INPUT:    dispSTR
//
// PURPOSE:  As each string is entered, advance
//           prior string to new line.
//           When all four lines filled, then
//           shift lines 4 thru 2 into 3 thru 1.
//           (The new string entered appears on line 4.)
// 
// FYI: LCD addr for start of each line:
//          0x00  
//          0x40 
//          0x14  
//          0x54
//

void LCDterm(char LCDlineCNTR){
    
//   switch(expression){
//      case constant-expression :
//         statement(s);
//         break;
//
//      default : statement(s);  // OPTIONAL ...
//   }
//
    Nop();
    Nop();
    Nop();
//    
    switch (LCDlineCNTR){
        case 1 :
        writeLCDcmd(0x80);
        break;
        
        case 2 :
        writeLCDcmd(0xC0);
        break;
        
        case 3 :
        writeLCDcmd(0x94);
        break;          
        
        case 4 :
        writeLCDcmd(0xD4);
        break; 
        
        default :
        writeLCDcmd(0x80);
        break;
    };
    
    Nop();
    Nop();    
}

//
//  The following create a pseudo screen "UP" effect
//  to the displayed lines .............................
//
void LCDshift5to2(void){
        LCDterm(1);  LCDdisplaySTRING(dispSTR2);
        LCDterm(2);  LCDdisplaySTRING(dispSTR3);     
        LCDterm(3);  LCDdisplaySTRING(dispSTR4);
        LCDterm(4);  LCDdisplaySTRING(dispSTR5);
}

void LCDshift6to3(void){
        LCDterm(1);  LCDdisplaySTRING(dispSTR3);
        LCDterm(2);  LCDdisplaySTRING(dispSTR4);     
        LCDterm(3);  LCDdisplaySTRING(dispSTR5);
        LCDterm(4);  LCDdisplaySTRING(dispSTR6);    
//       
}

void LCDshift7to4(void){
        LCDterm(1);  LCDdisplaySTRING(dispSTR4);
        LCDterm(2);  LCDdisplaySTRING(dispSTR5);     
        LCDterm(3);  LCDdisplaySTRING(dispSTR6);
        LCDterm(4);  LCDdisplaySTRING(dispSTR7);    
//       
}

void LCDshift8to5(void){
        LCDterm(1);  LCDdisplaySTRING(dispSTR5);
        LCDterm(2);  LCDdisplaySTRING(dispSTR6);     
        LCDterm(3);  LCDdisplaySTRING(dispSTR7);
        LCDterm(4);  LCDdisplaySTRING(dispSTR8);    
//       
}