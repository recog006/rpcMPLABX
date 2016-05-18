//
//  MODULE   : msgserials.c
//  PROJECT  : 
//  VERSION  :
//  AUTHOR   : RGF
//  WORKDATE : MAY 2016
//
//
//  *****************   REFERENCE INFO      *************************
//
//
//  ***************** END OF REFERENCE INFO *************************
//

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <plib/delays.h>
#include <plib/usart.h>

#include "Initialize_MCU/configMCU.h"
#include "Initialize_MCU/initialization.h"
#include "Initialize_MCU/myDELAYS.h"

#include "msgserials.h"

extern unsigned char bdum;
extern unsigned int idum;
 
extern char hbuf[];    // const defined in main.c ...
extern char sbuf[];

/*  ************************************************************************
 *  **********   Serial communication functions   **************************
 *  ************************************************************************
 *
 *   "htoa()" is "hex to ascii" ....... INPUT: idum
 *   "crlf()" is "carriage return line feed" ...
 *
 *   .....  */

void byteTOascBINARY(unsigned char b2conv){
	unsigned char btemp;

	btemp = b2conv & 0x80;
	sbuf[0] = hbuf[0];    /* Most signif bit */
	if (btemp == 0x80) sbuf[0] = hbuf[1];

	btemp = b2conv & 0x40;
	sbuf[1] = hbuf[0];
	if (btemp == 0x40) sbuf[1] = hbuf[1];

	btemp = b2conv & 0x20;
	sbuf[2] = hbuf[0];
	if (btemp == 0x20) sbuf[2] = hbuf[1];

	btemp = b2conv & 0x10;
	sbuf[3] = hbuf[0];
	if (btemp == 0x10) sbuf[3] = hbuf[1];

	btemp = b2conv & 0x08;
	sbuf[4] = hbuf[0];
	if (btemp == 0x08) sbuf[4] = hbuf[1];

	btemp = b2conv & 0x04;
	sbuf[5] = hbuf[0];
	if (btemp == 0x04) sbuf[5] = hbuf[1];

	btemp = b2conv & 0x02;
	sbuf[6] = hbuf[0];
	if (btemp == 0x02) sbuf[6] = hbuf[1];

	btemp = b2conv & 0x01;
	sbuf[7] = hbuf[0];
	if (btemp == 0x01) sbuf[7] = hbuf[1];

	sbuf[8] = 0;
}

/* Convert a "word" into ascii HEX ......... null terminated string */

void wordTOascHEX(unsigned int w2conv){    
    unsigned char btemp;
    
	btemp = (unsigned char) (w2conv >> 12);    /* Result in "sbuf" ... */
	sbuf[0] = hbuf[btemp];
    w2conv = w2conv & 0x0FFF;
	btemp = (unsigned char) (w2conv >> 8);
	sbuf[1] = hbuf[btemp];
    w2conv = w2conv & 0x00FF;
    btemp = (unsigned char) (w2conv >> 4);
    sbuf[2] = hbuf[btemp];
    w2conv = w2conv & 0x000F;
    btemp = (unsigned char) w2conv;
    sbuf[3] = hbuf[btemp];
    sbuf[4] = 0;
}

void byteTOascHEX(unsigned char b2conv){
	char btemp;

	btemp = b2conv >> 4;         /* Result in "sbuf" ....         */
	sbuf[0] = hbuf[btemp];
    btemp = b2conv & 0x0F;
	sbuf[1] = hbuf[btemp];
	sbuf[2] = 0;
}

// ...............................................................

void crlf_print(void){
    putrs1USART("\n\r");
}

void id_print(void){    /* ****  INITIAL ID MESSAGE ****** */
    for (bdum=0; bdum<10; bdum++) crlf_print();
    Nop();
    Nop();
//   
    putrs1USART(" *************************************** \n\r");
    putrs1USART(" ********* USART1 19200 BAUD *********** \n\r");
    putrs1USART(" *************************************** \n\r");  
	crlf_print();
	Nop();
	Nop();
	Nop();
}

void menu(void){    
    crlf_print();
    putrs1USART("****** MENU ****** \n\r");
    putrs1USART(" 1. Set RTC time \n\r");
    putrs1USART(" 2. Time OK, RUN \n\r"); 
    putrs1USART(" 3. Another item \n\r");
    putrs1USART("ENTER SELECTION: "); 
    Nop();
    Nop();
} 


//  ...............................................
//  ......   END of "msgserials.c"   ..............
//  ...............................................

