
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "lcd.h"
#include "main.h"
#include "test.h"
#include <util/delay.h>
#include <stdlib.h>


int main() {

    system_init();
    uint8_t last_sec = date.sec;

    run_tests();

    while (1) {
        if(last_sec != date.sec){
            PORTB &= ~(1<<PORTB5); //Turn pulse port low
            update_time();
            update_led();
            update_display(MODE_RUNNING, 0);
            last_sec = date.sec;
        } 

        while(current_mode) {
            // Control to update display. Last thing to do should be to SAVE (exit by pressing mode)
            if(time_control & MODE) {
                current_mode--;
                update_display(MODE_SETUP, '*');
            }
            else if(time_control & PLUS) {
                change_time(INCREASE);
            }
            else if(time_control & MINUS) {
                change_time(DECREASE);
            }
            time_control = 0;
        } 
    }
    return 0;
}

void update_led(void){
    PORTB = date.day;
}

void update_time(void){

    if ( date.sec == 0 ) {
        date.min++;
    }
    if ( date.min == 60 ) {
        date.min = 0;
        date.hour++;
    }
    if ( date.hour == 24 ) {
        date.hour = 0;
        date.day++;
    }
    if ( date.day >= 31 || date.month != 12) {
        lcd_clrscr();
        lcd_home();
        lcd_puts("APOCALYPSE!!");
        while(1);
    }
}

uint8_t change_time(uint8_t input) {
    
    char ch = 0;

    switch ( current_mode ) {
        case SECONDS:
            ch = 'S';
            if ( input == INCREASE ) {
                date.sec++;
            } else {
                date.sec--;
            }
            break;
        case MINUTE:
            ch = 'M';
            if ( input == INCREASE ) {
                date.min++;
            } else {
                date.min--;
            }
            break;
        case HOUR:
            ch = 'H';
            if ( input == INCREASE ) {
                date.hour++;
            } else {
                date.hour--;
            }
            break;
        case DAY:
            ch = 'D';
            if ( input == INCREASE ) {
                date.day++;
            } else {
                date.day--;
            }
            break;
        case MONTH:
            ch = 'M';
            if ( input == INCREASE ){
                date.month++;
            } else {
                date.month--;
            }
            break;
        case YEAR:
            ch = 'Y';
            if ( input == INCREASE ) {
                date.year++;
            } else {
                date.year--;
            }
            break;
    }
    update_display(MODE_SETUP, ch);
    return 1;
}

void update_display(uint8_t mode, char ch){

    itoa(date.sec, date.sec_array, 10);
    itoa(date.min, date.min_array, 10);
    itoa(date.hour, date.hour_array, 10);
    itoa(date.day, date.day_array, 10);
    itoa(date.month, date.month_array, 10);
    itoa(date.year, date.year_array, 10);


    char row1[16] = "DATE: 2016/12/01";
    char row2[16] = "Time: 12:00:00  ";

    uint8_t i;

    for (i = 0; i < 4; i++) row1[6+i] = date.year_array[i];
    for (i = 0; i < 2; i++) row1[11+i] = date.month_array[i];
    if(date.day < 10){
        row1[14] = '0';
        row1[15] = date.day_array[0];
    } else {
        for (i = 0; i < 2; i++) row1[14+i] = date.day_array[i];
    }
    
    if(date.hour < 10){
        row2[6] = '0';
        row2[7] = date.hour_array [0];
    } else {
        for (i = 0; i < 2; i++) row2[6+i] = date.hour_array [i];
    }

    if(date.min < 10){
        row2[9] = '0';
        row2[10] = date.min_array[0];
    } else {
        for (i = 0; i < 2; i++) row2[9+i] = date.min_array[i];
    }

    if(date.sec < 10){
        row2[12] = '0';
        row2[13] = date.sec_array[0];
    } else {
        for (i = 0; i < 2; i++) row2[12+i] = date.sec_array[i];
    }

    if(mode == MODE_SETUP){
        row2[14] = '*';
        row2[15] = ch;
    }

    lcd_gotoxy(0, 0);
    lcd_puts(row1);

    lcd_gotoxy(0, 1);
    lcd_puts(row2);

    // DATE: 2016/12/23
    // Time: 15:23:01

}

void system_init(void){

    lcd_init(LCD_DISP_ON_CURSOR);
    lcd_clrscr();
    timer_init();
    pin_interrupt_init();


    interrupt_bits_old = 0;
    firtst_time = 0;
    current_mode = 0;
    time_control = 0;
    button_pressed = 0;


    PORTB = 0;
    DDRB = (1<<PORTB0) | (1<<PORTB1) | (1<<PORTB2) | (1<<PORTB3) | (1<<PORTB4) | (1<<PORTB5);

    date.day = 1;
    date.month = 12;
    date.year = 2016;
    date.hour = 23;
    date.min = 59;
    date.sec = 50;

    current_mode = 0;

}

void timer_init(void) {

    TCCR1B |= (1<<WGM12); // Configure timer1 for CTC mode
    TIMSK1 |= (1<<OCIE1A); // Interrupt enable
    
    // 8 000 000 / 256 = 31 250
    // 65 535 / 31 250 = 2.09712
    // 65535 / 2.09712 = 31 250

    // 1 0000 000 / 256 = 3 906.25
    // 65 535 / 3 906.25 = 9.09696
    // 65535 / 9.09696 = 7204

    OCR1A = 7204; // Trigger interrupt when overflow
    TCCR1B |= (1<<CS12); // Prescaler = 256 to get a even number

    sei();
}

void pin_interrupt_init(void){

    PCMSK1 |= (1<<PCINT11) | (1<<PCINT12) | (1<<PCINT13);
    PCICR |= (1<<PCIE1); // Enable interrupt on PCMSK1
}

ISR(TIMER1_COMPA_vect) {
    date.sec++;
    PORTB |= (1<<PORTB5); //Turn port HIGH
    if(date.sec >= 60) date.sec = 0;
}

ISR(PCINT1_vect) {

    uint8_t interrupt_bits_new, reg;

    reg = PINC;

    interrupt_bits_new = reg ^ interrupt_bits_old;
    interrupt_bits_old = reg;


    if(interrupt_bits_new & (1<<PCINT11)){
        time_control |= MODE;
        if (current_mode <= 0)
        current_mode = 7;
    }
    else if(interrupt_bits_new & (1<<PCINT12)){
        time_control |= PLUS;
    }
    else if(interrupt_bits_new & (1<<PCINT13)){
        time_control |= MINUS;
    }

}

//PCMSK0 = PCINT 0-7
//PCMSK1 = PCINT 8-14
//PCMSK2 = PCINT 16-23