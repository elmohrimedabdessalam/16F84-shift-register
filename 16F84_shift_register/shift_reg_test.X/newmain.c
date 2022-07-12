/*
 * File:   newmain.c
 * Author: Candela
 *
 * Created on 26 June 2022, 17:43
 */

#define _XTAL_FREQ 8000000UL

#include <xc.h>
#include <stdbool.h>
// BEGIN CONFIG
#pragma config FOSC = HS // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config CP = OFF // Flash Program Memory Code Protection bit (Code protection off)
//END CONFIG


volatile unsigned long millis = 0, time;
volatile bool transmit_byte = false, ris = true, MVT = false;
volatile uint8_t byte = 0, i = 0;
volatile int16_t bit_counter = 0;

void __interrupt() my_ISR(void)
{
    
   if (INTCONbits.T0IF == 1) // timer overflow interrupt
    {
        INTCONbits.T0IF = 0;// clear flag
        if (transmit_byte == true)
        {
            RA0 ^= 1;
            if (RA0 == 0){
                if (bit_counter < 8){
                    if ((byte & (1<<(7 - bit_counter))) == (1<<(7 - bit_counter)))
                    {
                        PORTAbits.RA1 = 1;
                    }
                    else 
                    {
                        PORTAbits.RA1 = 0;
                    }
                }
                else if (bit_counter == 8)
                {
                    PORTAbits.RA1 = 0;// S_Data line = zero 
                    PORTAbits.RA2 = 1;//show out data in the shift register (latch)
                }
                else 
                {
                    PORTAbits.RA2 = 0;
                    //PORTAbits.RA0 = 1;
                    bit_counter = -1;
                    transmit_byte = false;
                }
                bit_counter++;
            }
        }
        TMR0 = 6;
    }
}

void main()
{
 
  TRISA0 = 0; //CLK as Output PIN
  TRISA1 = 0; //Data_In as Output PIN
  TRISA2 = 0; //Latch as Output PIN
  
  INTCONbits.GIE = 1;// enable global interrupt
  
  
  INTCONbits.T0IE = 1;//Enables the TMR0 interrupt
  INTCONbits.T0IF = 0;//clear timer interrupt flag
  INTCONbits.INTE = 0;//Enable interrupt 
  INTCONbits.INTF = 0;
  
  OPTION_REGbits.T0CS = 0;//  Internal instruction cycle clock (CLKOUT)
  OPTION_REGbits.T0SE = 0;
  OPTION_REGbits.INTEDG = 1; //rising edge
  OPTION_REGbits.PSA = 0;//  Prescaler is assigned to the Timer0 module
  OPTION_REGbits.PS0 = 0; // set Prescaler 8
  OPTION_REGbits.PS1 = 1;
  OPTION_REGbits.PS2 = 0;

  RA0 = 1; //CLK set it HIGH
  RA1 = 0; // S_Data
  RA2 = 0; // latch
  
  while(1)
    {
      byte = 0xAA;
     transmit_byte = true;
     __delay_ms(500);
     byte >>= 1;
     transmit_byte = true;
     __delay_ms(500);
    }
}