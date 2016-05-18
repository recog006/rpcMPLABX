/*
 * File:   test_serial.c
 
 */

/////////////PRE-PROCESSOR COMMANDS//////////////////////////////
#pragma config FOSC = INTIO67    // CONFIG1H  internal oscillator block, port function on RA6 and RA7
#pragma config WDTEN = OFF  // CONFIG2H turn the watchdog timer off
#pragma config MCLRE = EXTMCLR  //enable MCLRE (need for programmer
#pragma config DEBUG = OFF, LVP = OFF

#include <xc.h>
#include <delays.h>
#include <usart.h>

void main(void)
{
    unsigned char data_to_send = 7;

    //Initialize the system clock
   OSCCONbits.IRCF = 5;  //Sets the internal oscillator frequency to 4 MHz.
   OSCCONbits.SCS = 2;   //Sets the system clock to the internal osc.
   
   // Set the direction of the transmit/receive and flow control bits.
   ANSELC = 0;  //turn off ADC's just in case
   TRISCbits.TRISC6 = 1;  //the transmit bit - MUST BE SET TO 1, then USART controls input output
   TRISCbits.TRISC7 = 1;  //the receive bit - MUST BE SET TO 1, then USART controls input output

    //Configure the serial port
    Open1USART(USART_TX_INT_OFF &
               USART_RX_INT_OFF &
               USART_ASYNCH_MODE &
               USART_EIGHT_BIT &
               USART_CONT_RX &
               USART_BRGH_HIGH, 25);

    while(1==1)
    {
        while(Busy1USART());   //wait until USART1 isn't busy
        Write1USART(data_to_send);  //write out a char
        Delay10KTCYx(20);  //wait around
    }
}
/*end of function main()*/





