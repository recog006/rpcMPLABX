//
//  MODULE   : msgserials.h
//  PROJECT  : RPC DEV
//  VERSION  : 2.0
//  AUTHOR   : RGF
//  WORKDATE : APRIL 2016
//
#ifndef XC_MSGSERIALS_H
#define XC_MSGSERIALS_H

extern void byteTOascBINARY(unsigned char b2conv);
extern void wordTOascHEX(unsigned int w2conv); 

extern void byteTOascHEX(unsigned char b2conv);

extern void crlf_print(void);
extern void id_print(void);

extern void menu(void);

#endif