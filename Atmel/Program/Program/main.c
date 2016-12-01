#define F_CPU 8000000

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdlib.h>
#include "lcd.h"

#define MODE (1<<0)
#define PLUS (1<<1)
#define MINUS (1<<2)
#define SAVED (1<<7)
uint8_t button_pressed = 0;
uint8_t time_control = 0;

uint8_t current_mode = 0;
#define SECONDS 6
#define MINUTE 5
#define HOUR 4

#define DAY 3
#define MONTH 2
#define YEAR 1

#define INCREASE 0xFF
#define DECREASE 0xFE


typedef struct {
    uint8_t sec;
    char sec_array[10];

    uint8_t min;
    char min_array[10];

    uint8_t hour;
    char hour_array[10];

    uint8_t day;
    char day_array[10];

    uint8_t month;
    char month_array[10];
    
    uint16_t year;
    char year_array[10];

}date_time; // Maybe do two structs???

date_time date;

void update_led(void);
void update_time(void);
uint8_t change_time(uint8_t input);
void timer_init(void);
void system_init(void);
void pin_interrupt_init(void);
void update_display(void);

volatile uint8_t interrupt_bits_old = 0;

int main() {

    system_init();
    uint8_t last_sec = date.sec;

    while (1) {
        if(last_sec != date.sec){
            lcd_putc('s');
            update_time();
            update_led();
            update_display();
            last_sec = date.sec;
        } 
/*
        while(current_mode) {
            // Control to update display. Last thing to do should be to SAVE (exit by pressing mode)
            if(time_control & MODE) {
                current_mode--;
            }
            else if(time_control & PLUS) {
                change_time(INCREASE);
            }
            else if(time_control & MINUS) {
                change_time(DECREASE);
            }
            time_control = 0;
        } */
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
    if ( date.day == 31 || date.month != 12) {
        lcd_clrscr();
        lcd_home();
        lcd_puts("APOCALYPSE!!");
        while(1);
    }
}

uint8_t change_time(uint8_t input) {

    switch ( current_mode ) {
        case SECONDS:
            if ( input == INCREASE ) {
                date.sec++;
            } else {
                date.sec--;
            }
            break;
        case MINUTE:
            if ( input == INCREASE ) {
                date.min++;
            } else {
                date.min--;
            }
            break;
        case HOUR:
            if ( input == INCREASE ) {
                date.hour++;
            } else {
                date.hour--;
            }
            break;
        case DAY:
            if ( input == INCREASE ) {
                date.day++;
            } else {
                date.day--;
            }
            break;
        case MONTH:
            if ( input == INCREASE ){
                date.month++;
            } else {
                date.month--;
            }
            break;
        case YEAR:
            if ( input == INCREASE ) {
                date.year++;
            } else {
                date.year--;
            }
            break;
    }
    update_display();
    return 1;
}

void update_display(void){

    itoa(date.sec, date.sec_array, 10);
    itoa(date.min, date.min_array, 10);
    itoa(date.sec, date.hour_array, 10);
    itoa(date.sec, date.day_array, 10);
    itoa(date.sec, date.month_array, 10);
    itoa(date.sec, date.year_array, 10);

    char row1[16] = "DATE: 2016/12/23";
    char row2[16] = " Time: 15:23:01 ";

    uint8_t i;

    for (i = 0; i < 4; i++) row1[6+i] = date.year_array [i];
    for (i = 0; i < 2; i++) row1[11+i] = date.month_array[i];
    for (i = 0; i < 2; i++) row1[14+i] = date.day_array[i];

    for (i = 0; i < 2; i++) row1[7+i] = date.hour_array [i];
    for (i = 0; i < 2; i++) row1[10+i] = date.min_array[i];
    for (i = 0; i < 2; i++) row1[13+i] = date.sec_array[i];

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

    PORTB = 0;
    DDRB = (1<<PORTB0) | (1<<PORTB1) | (1<<PORTB2) | (1<<PORTB3) | (1<<PORTB4) | (1<<PORTB5);

    date.day = 1;
    date.month = 12;
    date.year = 2016;
    date.hour = 12;
    date.min = 0;
    date.sec = 0;

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
    // 65535 / 7204


    OCR1A = 31250; // Trigger interrupt when overflow
    TCCR1B |= (1<<CS12); // Prescaler = 256 to get a even number

    sei();
}

void pin_interrupt_init(void){

    EICRA |= (1<<ISC01); // Interrupt on falling edge
    PCICR |= (1<<PCIE1); // Enable interrupt on PCMSK1

}

ISR(TIMER1_COMPA_vect) {
    date.sec++;
    if( date.sec % 2 == 0){
        PORTB |= (1<<PORTB5);
    } else {
        PORTB &= ~(1<<PORTB5);
    }
    if(date.sec >= 60) date.sec = 0;
}
/*
ISR(PCINT0_vect){
    button_pressed = PCMSK0;

    if(button_pressed & MODE ) {
        time_control |= MODE;
        if (current_mode <= 0){
            current_mode = 7;
        }
    }
    else if ( button_pressed & PLUS ) {
        time_control |= PLUS;
    }
    else if ( button_pressed & MINUS ) {
        time_control |= MINUS;
    }
}
*/
ISR(PCINT1_vect) {
    uint8_t interrupt_bits_new;

    interrupt_bits_new = PINC ^ interrupt_bits_old;
    interrupt_bits_old = PINC;

    switch (interrupt_bits_new) {
        case PCINT11 : 
            time_control |= MODE;
            if (current_mode <= 0)
                current_mode = 7;
            break;
        case PCINT12 :
            time_control |= PLUS;
            break;
        case PCINT13 :
            time_control |= MINUS;
            break;
    }
}

//PCMSK0 = PCINT 0-7
//PCMSK1 = PCINT 8-14
//PCMSK2 = PCINT 16-23