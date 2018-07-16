/*Examination Number: Y3834764*/
#include <stdio.h>
#include "serial.h"
#include "stm32f4xx.h"
#include "delay.h"
#include "hitachi_lcd.h"

int main(void) {
	serial_init();

    lcd_init();
	set_text("Testing text");
	cursor_enter();
	set_text("Ahhhhh");

	delay_ms(1000);
	screen_clear();
    cursor_set_home();
	/* An embedded C program should never end */
	while(1);
}




