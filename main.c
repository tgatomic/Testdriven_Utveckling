#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "lcd.h"


int main()
 { 
   // Write your code here
    
   PORTB = 0;
   DDRB = (1<<0);
    
   lcd_init(LCD_DISP_ON_CURSOR);
    
    
   while (1){

      lcd_putc('S');
      PORTB |= (1<<PORTB0);
      _delay_ms(1000);
      PORTB &= ~(1<<PORTB0);
      _delay_ms(1000);
      
      
   }
   return 0;
 }