/*
 * test.h
 *
 * Created: 2016-12-05 11:31:11
 *  Author: Atomic
 */ 


#ifndef TEST_H_
#define TEST_H_

#define TRUE 1
#define FALSE 0

uint8_t run_tests(void);
uint8_t test_leds(void);
uint8_t test_sec(void);
uint8_t test_lcd(void);
uint8_t test_lcd_date(void);
uint8_t test_lcd_sec_update(void);
uint8_t test_button(void);



#endif /* TEST_H_ */