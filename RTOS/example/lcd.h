#include "stm32f10x.h"

/* Connections between LCD and STM32F100RB  */
#define LCD_RS          GPIO_Pin_15
#define LCD_EN          GPIO_Pin_14
#define LCD_D4          GPIO_Pin_5
#define LCD_D5          GPIO_Pin_6
#define LCD_D6  		GPIO_Pin_7
#define LCD_D7          GPIO_Pin_8
#define LCD_PORT        GPIOB        // Port that is connected the LCD (current port is PORT C)
#define LCD_D_ALL  		(GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8)

void delay_ms(int ms);
void delay_us(int us);
void LCD_SendByte(unsigned char cmd); //Sends a data byte to LCD (byte 'cmd').
void LCD_SendCmd (unsigned char cmd); //Sends a command byte to LCD (byte 'cmd').
void LCD_SendData(unsigned char data);
void LCD_SendText(char *text);
void LCD_GoTo (unsigned char line, unsigned char column);
void LCD_Clear (void);
void LCD_Init (void);
