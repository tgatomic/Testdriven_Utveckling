/*
 * main.h
 *
 * Created: 2016-12-05 11:40:43
 *  Author: Atomic
 */ 


#ifndef MAIN_H_
#define MAIN_H_


#define MODE (1<<0)
#define PLUS (1<<1)
#define MINUS (1<<2)
#define SAVED (1<<7)
uint8_t button_pressed;
uint8_t time_control;

uint8_t current_mode;
#define SECONDS 6
#define MINUTE 5
#define HOUR 4

#define DAY 3
#define MONTH 2
#define YEAR 1

#define INCREASE 0xFF
#define DECREASE 0xFE

#define MODE_RUNNING 1
#define MODE_SETUP 0


typedef struct {

    volatile uint8_t sec;
    char sec_array[15];

    uint8_t min;
    char min_array[15];

    uint8_t hour;
    char hour_array[15];

    uint8_t day;
    char day_array[15];

    uint8_t month;
    char month_array[15];
    
    uint16_t year;
    char year_array[15];

}date_time; // Maybe do two structs???

date_time date;

void update_led(void);
void update_time(void);
uint8_t change_time(uint8_t input);
void timer_init(void);
void system_init(void);
void pin_interrupt_init(void);
void update_display(uint8_t mode, char ch);

volatile uint8_t interrupt_bits_old;
volatile uint8_t firtst_time;


#endif /* MAIN_H_ */