#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "lcd.h"

const char UserFont[8][8] = {  /* 8 user defined characters to be loaded into CGRAM (used for bargraph)*/
    { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
    { 0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10 },
    { 0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18 },
    { 0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C },
    { 0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E },
    { 0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F },
    { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
    { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }
};
//==========================================================================================
//
//==========================================================================================
void delay_ms(int ms)
{
 long i, tms;
 tms = 1000*ms;
 for(i=0;i<tms;i++);
}

//==========================================================================================
//
//==========================================================================================
void delay_us(int us)
{
 long i, tus;
 tus = 1*us;
 for(i=0;i<tus;i++);
}

//==========================================================================================
//
//==========================================================================================
void LCD_SendByte(uint8_t cmd)
{
 uint8_t tcmd = 0;

 GPIO_ResetBits(LCD_PORT, LCD_D_ALL);
        GPIO_ResetBits(LCD_PORT, LCD_EN);


 tcmd = cmd >> 4;
 if( tcmd & 0x01 )
 GPIO_SetBits(LCD_PORT, LCD_D4);
 if( tcmd & 0x02 )
  GPIO_SetBits(LCD_PORT, LCD_D5);
 if( tcmd & 0x04 )
  GPIO_SetBits(LCD_PORT, LCD_D6);
 if( tcmd & 0x08 )
  GPIO_SetBits(LCD_PORT, LCD_D7);
GPIO_SetBits(LCD_PORT, LCD_EN);
delay_us(50);


 GPIO_ResetBits(LCD_PORT, LCD_EN);
 delay_ms(10);


 GPIO_ResetBits(LCD_PORT, LCD_D_ALL);

 cmd &= 0x0F;
 if( cmd & 0x01 )
  GPIO_SetBits(LCD_PORT, LCD_D4);
 if( cmd & 0x02 )
  GPIO_SetBits(LCD_PORT, LCD_D5);
 if( cmd & 0x04 )
  GPIO_SetBits(LCD_PORT, LCD_D6);
 if( cmd & 0x08 )
  GPIO_SetBits(LCD_PORT, LCD_D7);
GPIO_SetBits(LCD_PORT, LCD_EN);
delay_us(50);
 GPIO_ResetBits(LCD_PORT, LCD_EN);
        delay_ms(10);
 GPIO_ResetBits(LCD_PORT, LCD_D_ALL);
 GPIO_ResetBits(LCD_PORT, LCD_RS);

}

//==========================================================================================
//
//==========================================================================================
void LCD_SendCmd(uint8_t cmd)
{
 GPIO_ResetBits(LCD_PORT, LCD_RS);
 delay_us(50);
 LCD_SendByte(cmd);
}

//==========================================================================================
//
//==========================================================================================
void LCD_SendData(uint8_t data)
{
 GPIO_SetBits(LCD_PORT, LCD_RS);
 delay_us(50);
 LCD_SendByte(data);
}

//==========================================================================================
//   print a string on LCD.
//==========================================================================================
void LCD_SendText(char *text)
{
 while(*text)
 {
  LCD_SendData(*text);
  text++;
 }
}

//==========================================================================================
//   Clear the LCD display.
//==========================================================================================
void LCD_Clear(void)
{
 LCD_SendCmd(0x01);
}

//==========================================================================================
//   Set cursor position on LCD display.
//==========================================================================================
void LCD_GoTo (unsigned char line, unsigned char column)
{
 uint8_t position = 0;

 position = (line * 0x40) + column;
 position = 0x80 + (position & 0x7F);

 LCD_SendCmd(position);
}

//==========================================================================================
//   Initialize the LCD.
//==========================================================================================
void LCD_Init(void)
{
 uint8_t i;
 char const *p;

 GPIO_InitTypeDef GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
 GPIO_InitStructure.GPIO_Pin = (LCD_D7 | LCD_D6 | LCD_D5 | LCD_D4 | LCD_EN | LCD_RS);
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(LCD_PORT, &GPIO_InitStructure);

 delay_ms(30);
 GPIO_ResetBits(LCD_PORT, LCD_RS);
 GPIO_ResetBits(LCD_PORT, LCD_EN);
 for(i = 0; i<3; i++)
 {

  GPIO_SetBits(LCD_PORT, LCD_D4 | LCD_D5);
  GPIO_ResetBits(LCD_PORT, LCD_D6 | LCD_D7);
                GPIO_SetBits(LCD_PORT, LCD_EN);
                delay_us(50);
  GPIO_ResetBits(LCD_PORT, LCD_EN);
                delay_ms(10);
 }

 delay_ms(50);
 GPIO_SetBits(LCD_PORT, LCD_D5);
 GPIO_ResetBits(LCD_PORT,LCD_D4 | LCD_D6 | LCD_D7);
        GPIO_SetBits(LCD_PORT, LCD_EN);
 delay_ms(50);
 GPIO_ResetBits(LCD_PORT, LCD_EN);
 LCD_SendCmd(0x28);
 LCD_SendCmd(0x08);
 LCD_SendCmd(0x01);
 LCD_SendCmd(0x06);
 LCD_SendCmd(0x0C); //
 delay_ms(5);

 /* Load user-specific characters into CGRAM                               */
 LCD_SendCmd(0x40);                  /* Set CGRAM address counter to 0     */
 p = &UserFont[0][0];
 for (i = 0; i < sizeof(UserFont); i++, p++)
  LCD_SendData (*p);
 LCD_SendCmd(0x80);                 /* Set DDRAM address counter to 0     */
}
