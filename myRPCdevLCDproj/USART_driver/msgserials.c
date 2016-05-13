//
//  MODULE   : msgserials.c
//  PROJECT  : 
//  VERSION  :
//  AUTHOR   : RGF
//  WORKDATE : APRIL 2016
//
//
//  *****************   REFERENCE INFO      *************************
//
//
//  ***************** END OF REFERENCE INFO *************************
//

#include <xc.h>
// #include <p18f46k22.h>

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
 
extern char hbuf[];
extern char sbuf[];

/*  ************************************************************************
 *  **********   Serial communication functions   **************************
 *  ************************************************************************
 *
 *   "btoa()" is "binary to ascii" .... INPUT: bdum
 *   "htoa()" is "hex to ascii" ....... INPUT: idum
 *   "crlf()" is "carriage return line feed" ...
 *
 *   .....  */

void btoa(void){
	unsigned char btemp;

	btemp = bdum & 0x80;
	sbuf[0] = hbuf[0];    /* Most signif bit */
	if (btemp == 0x80) sbuf[0] = hbuf[1];

	btemp = bdum & 0x40;
	sbuf[1] = hbuf[0];
	if (btemp == 0x40) sbuf[1] = hbuf[1];

	btemp = bdum & 0x20;
	sbuf[2] = hbuf[0];
	if (btemp == 0x20) sbuf[2] = hbuf[1];

	btemp = bdum & 0x10;
	sbuf[3] = hbuf[0];
	if (btemp == 0x10) sbuf[3] = hbuf[1];

	btemp = bdum & 0x08;
	sbuf[4] = hbuf[0];
	if (btemp == 0x08) sbuf[4] = hbuf[1];

	btemp = bdum & 0x04;
	sbuf[5] = hbuf[0];
	if (btemp == 0x04) sbuf[5] = hbuf[1];

	btemp = bdum & 0x02;
	sbuf[6] = hbuf[0];
	if (btemp == 0x02) sbuf[6] = hbuf[1];

	btemp = bdum & 0x01;
	sbuf[7] = hbuf[0];
	if (btemp == 0x01) sbuf[7] = hbuf[1];

	sbuf[8] = 0;
}

void htoa(void){                  /* Hex to ascii ... null terminated string */
	bdum = (unsigned char) (idum >> 12);    /* Result in "sbuf" ....         */
	sbuf[0] = hbuf[bdum];
    idum = idum & 0x0FFF;
	bdum = (unsigned char) (idum >> 8);
	sbuf[1] = hbuf[bdum];
    idum = idum & 0x00FF;
    bdum = (unsigned char) (idum >> 4);
    sbuf[2] = hbuf[bdum];
    idum = idum & 0x000F;
    bdum = (unsigned char) idum;
    sbuf[3] = hbuf[bdum];
    sbuf[4] = 0;
}

void bytetohex(void){
	char btemp;

	btemp = bdum >> 4;         /* Result in "sbuf" ....         */
	sbuf[0] = hbuf[btemp];
    btemp = bdum & 0x0F;
	sbuf[1] = hbuf[btemp];
	sbuf[2] = 0;
}

// ...............................................................

void crlf_print(void){
    if (USB_RTS == 0) putrs1USART("\n\r");
    Nop();
    Nop();
}

void id_print(void){    /* ****  INITIAL ID MESSAGE ****** */
    for (bdum=0; bdum<10; bdum++) crlf_print();
    Nop();
    Nop();
//   
    outCHK: if (USB_RTS != 0) goto outCHK;
    
    putrs1USART(" *************************************** \n\r");
    putrs1USART(" ********* USART1 19200 BAUD *********** \n\r");
    putrs1USART(" *************************************** \n\r");  
	crlf_print();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
}

void menu(void){    
    outCHK: if (USB_RTS != 0) goto outCHK;
          
    crlf_print();
    putrs1USART("****** MENU *** \n\r");
    putrs1USART("1. Set RTC time \n\r");
    putrs1USART("2. Time OK, RUN \n\r"); 
    putrs1USART("3. Another item \n\r");
    putrs1USART("ENTER SELECTION: "); 
    Nop();
    Nop();
} 


//  ...............................................
//  ......   END of "msgserials.c"   ..............
//  ...............................................

