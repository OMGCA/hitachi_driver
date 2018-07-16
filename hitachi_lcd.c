/*Examination Number: Y3834764*/
#include <stdio.h>
#include <string.h>
#include "serial.h"
#include "stm32f4xx.h"
#include "delay.h"
#include "hitachi_lcd.h"

void enablePortA(void){
  GPIOA->MODER &= 0x3FFFFFFF;//   00 11 11 11
  GPIOA->MODER |= 0x40000000;//OE:01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
	delay_ms(100);
}

void enablePortB(void){
  GPIOB->MODER &= 0xFFFFFFC0;//   11 11 11 11 11 11 11 11 11 11 11 11 11 00 00 00
  GPIOB->MODER |= 0x00000015;//   00 00 00 00 00 00 00 00 00 00 00 00 00 01 01 01
	delay_ms(100);
}

void enablePortD(void){
  GPIOD->MODER &= 0xFFFF0000;//   11 11 11 11 11 11 11 11 00 00 00 00 00 00 00 00
  GPIOD->MODER |= 0x00005555;//   00 00 00 00 00 00 00 00 01 01 01 01 01 01 01 01
	delay_ms(100);
}

void setPortAOE(void){
  GPIOA->ODR |= 0x00008000; // 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 -> 8000
  delay_ms(55);
}

void cursor_set_home(void){

  GPIOB->ODR &= 0xFFFFFFF0;
  GPIOB->ODR |= 0x00000000;
  delay_us(40);
  GPIOD->ODR &= 0xFFFFFF00;
  GPIOD->ODR |= 0x00000002; // 0 0 0 0 0 0 1 *
  delay_us(100);
  lineEProcedure();

}

void lineEProcedure(void){
  GPIOB->ODR &= 0xFFFFFFFB; // 0 0 0 0 0 0 0 0 0 0 0 0 1 0 1 1
  GPIOB->ODR |= 0x00000004; // 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0
  delay_us(250);
  GPIOB->ODR &= 0xFFFFFFF0;
  delay_us(100);
}

void lineFontInit(void){
  GPIOB->ODR &= 0xFFFFFFF0;
  GPIOB->ODR |= 0x00000000;
  delay_us(40);
  GPIOD->ODR &= 0xFFFFFF00;
  GPIOD->ODR |= 0x0000003C;
  delay_us(100);
  lineEProcedure();

}

void displayOn(void){
  GPIOB->ODR &= 0xFFFFFFF0;
  GPIOB->ODR |= 0x00000000;
  delay_us(40);
  GPIOD->ODR &= 0xFFFFFF00;
  GPIOD->ODR |= 0x0000000F;
  delay_us(100);
  lineEProcedure();
}

void lcd_function_set(void){
  /*Set RS & R/W*/
  GPIOB->ODR &= 0xFFFFFFF0;
  GPIOB->ODR |= 0x00000000;
  delay_us(40);
	/*Set data*/
  GPIOD->ODR &= 0xFFFFFF0F;
  GPIOD->ODR |= 0x00000030;// 0011 0000
  delay_ms(100);
  lineEProcedure();

}

void screen_clear(void){
  GPIOB->ODR &= 0xFFFFFFF0;
  GPIOB->ODR |= 0x00000000;
  delay_us(40);
  GPIOD->ODR &= 0xFFFFFF00;
  GPIOD->ODR |= 0x00000001;
  delay_us(100);
  lineEProcedure();

  cursor_set_home();
}

void entryModeSet(void){
  GPIOB->ODR &= 0xFFFFFFF0;
  GPIOB->ODR |= 0x00000000;
  delay_us(40);
  GPIOD->ODR &= 0xFFFFFFF0;
  GPIOD->ODR |= 0x00000007;
  delay_us(100);
  lineEProcedure();
}

void lcd_init(void){
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIODEN;

  enablePortA();

  enablePortB();

  enablePortD();
  /*Enable Port A Pin15*/
  setPortAOE();

  /*Function Set*/
  lcd_function_set();
  lcd_function_set();
  lcd_function_set();

  /*Specify line and font*/
  lineFontInit();

  /*Display on*/
  displayOn();

  /*Display Clear*/
  screen_clear();

  /*Entry Mode Set*/
  entryModeSet();

  cursor_set_home();
  cursor_set_home();
}

void screen_shift(int pos){

	int counter = 0;
	/*Screen shift right*/
	if ( pos > 0 ){
		for ( counter = 0; counter < pos; counter++ )
		{
			GPIOB->ODR &= 0xFFFFFFF0;
			GPIOB->ODR |= 0x00000000;
			delay_us(40);
			GPIOD->ODR &= 0xFFFFFF00;
			GPIOD->ODR |= 0x0000001C;
			delay_us(100);
			GPIOB->ODR &= 0xFFFFFFF0;
			GPIOB->ODR |= 0x00000004;
			delay_us(250);
			GPIOB->ODR &= 0xFFFFFFF0;
			delay_us(100);
		}
	}

	/*Screen shift left*/
	else if ( pos < 0 ){
		for ( counter = 0; counter > pos; counter-- )
		{
			GPIOB->ODR &= 0xFFFFFFF0;
			GPIOB->ODR |= 0x00000000;
			delay_us(40);
			GPIOD->ODR &= 0xFFFFFF00;
			GPIOD->ODR |= 0x00000018;
			delay_us(100);
			GPIOB->ODR &= 0xFFFFFFF0;
			GPIOB->ODR |= 0x00000004;
			delay_us(250);
			GPIOB->ODR &= 0xFFFFFFF0;
			delay_us(100);
		}
	}
}

void set_text(char* input_string){

	int len = strlen(input_string);
	int counter = 0;

	for ( counter = 0; counter < len; counter++ )
	{
		GPIOD->ODR &= 0xFFFFFF00;
		GPIOD->ODR |= input_string[counter];
		delay_us(100);
		GPIOB->ODR &= 0xFFFFFFF0; // RS R/W setting for write DDRAM/CGRAM
		GPIOB->ODR |= 0x00000001;
		delay_us(40);

		lineEProcedure();
	}
}

void cursor_move(int pos){
	int counter = 0;

	/*Cursor move right*/
	if ( pos > 0 ){
		for ( counter = 0; counter < pos; counter++ )
		{
			GPIOB->ODR &= 0xFFFFFFF0;
			GPIOB->ODR |= 0x00000000;
			delay_us(40);
			GPIOD->ODR &= 0xFFFFFF00;
			GPIOD->ODR |= 0x00000014;
			delay_us(100);
			lineEProcedure();
		}
	}

	/*Cursor move left*/
	else if ( pos < 0 ){
		for ( counter = 0; counter > pos; counter-- )
		{
			GPIOB->ODR &= 0xFFFFFFF0;
			GPIOB->ODR |= 0x00000000;
			delay_us(40);
			GPIOD->ODR &= 0xFFFFFF00;
			GPIOD->ODR |= 0x00000010;
			delay_us(100);
			lineEProcedure();
		}
	}
}

void cursor_enter(void){
	cursor_set_home();
	cursor_move(40);//40 units per line
}


