#include <stdint.h>
#include <stdlib.h>
#include "main.h"
#include "lcd.h"
#include "test.h"
#include <util/delay.h>


uint8_t run_tests(void){

    uint8_t test_passed = 0;
    uint8_t nmbr_of_tests = 6;

    test_passed += test_leds();
    test_passed += test_sec();
    test_passed += test_lcd();
    test_passed += test_lcd_date();
    test_passed += test_lcd_sec_update();
    test_passed += test_button();

    if(test_passed == nmbr_of_tests){
        lcd_clrscr();
        lcd_gotoxy(0,0);
        lcd_puts("PASSED!");
        return TRUE;
    } else {
        lcd_clrscr();
        lcd_gotoxy(0,0);
        lcd_puts("PASSED!");
        return FALSE;
    }
    _delay_ms(3000);
}

uint8_t test_leds(void){

    PORTB = (1<<PORTB0);
    _delay_ms(500);
    PORTB = (1<<PORTB1);
    _delay_ms(500);
    PORTB = (1<<PORTB2);
    _delay_ms(500);
    PORTB = (1<<PORTB3);
    _delay_ms(500);
    PORTB = (1<<PORTB4);
    _delay_ms(500);

    return TRUE;

}

uint8_t test_sec(void ){
    uint8_t last_sec = 0;
    last_sec = date.sec;
    uint8_t passed = 0;

    for (uint8_t i = 0; i<5; i++){
        if(date.sec > last_sec){
            passed += TRUE;
        } else {
            return FALSE;
        }
        last_sec = date.sec;
        _delay_ms(1200);
    }
    return TRUE;
}

uint8_t test_lcd(void){

    lcd_clrscr();
    lcd_gotoxy(0,0);
    lcd_puts("TEST_LCD");
    _delay_ms(1000);
    return TRUE;
}

uint8_t test_lcd_date(void){

    lcd_clrscr();
    update_display(MODE_RUNNING, 0);
    _delay_ms(1000);

    return TRUE;
}

uint8_t test_lcd_sec_update(void){

    char i = 0;
    char i_array[10];

    for(; i<20; i++){
        lcd_clrscr();
        lcd_gotoxy(0,0);
        lcd_puts("Updating screen");
        lcd_gotoxy(0,1);
        itoa(i, i_array, 10);
        lcd_puts(i_array);
        _delay_ms(1000);
    }
    return TRUE;
}

uint8_t test_button(void){

    lcd_clrscr();
    lcd_gotoxy(0,0);
    lcd_puts("Press mode 3");
    lcd_gotoxy(0,1);
    lcd_puts("times to exit");

    uint8_t ret_function = 3;

    while(ret_function){
        if(time_control & MODE){
            PORTB = (1<<PORTB0);
            ret_function--;
        }
        else if (time_control & PLUS){
            PORTB = (1<<PORTB1);
            ret_function = 3;
        }
        else if (time_control & MINUS){
            PORTB = (1<<PORTB2);
            ret_function = 3;
        }
        time_control = 0;
    }
    return TRUE;
}

