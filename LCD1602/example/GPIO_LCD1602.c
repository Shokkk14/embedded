#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stdint.h>
#include <stdio.h>

/***************************************************************************//**
 * Global variables, private define and typedef
 ******************************************************************************/
#define RS  GPIO_Pin_10
#define RW  GPIO_Pin_11
#define EN  GPIO_Pin_12

const unsigned int SWAP_DATA[16] = { 0x0, 0x8, 0x4, 0xC, 0x2, 0xA, 0x6, 0xE,
                                     0x1, 0x9, 0x5, 0xD, 0x3, 0xB, 0x7, 0xF};


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

/***************************************************************************//**
 * Declare function prototypes
 ******************************************************************************/
void RCC_Configuration(void);
void LCD_DATA_DIR_OUT(void);
void LCD_DATA_DIR_IN(void);
void LCD_ALL_DIR_OUT(void);
unsigned int LCD_DATA_IN(void);
static unsigned char Wait_While_Busy(void);
static unsigned char Lcd_Read_Status (void);
void Lcd_Write_4bits(uc8 byte);
void Delay(vu32 nCount);
void Lcd_Write_Command(uc8 command);
void Lcd_Write_Data(uc8 data);
void Lcd_Init(void);
void Lcd_Write_Line1(void);
void Lcd_Write_Line2(void);
void set_cursor(int, int);
void lcd_print (char *string);
void lcd_clear (void);

/***************************************************************************//**
 * @brief First set the RCC clock, then initial the LCD, display characters.
 ******************************************************************************/
void GPIO_LCD1602(void)
{
    Lcd_Init();                         /* initial       */
    lcd_clear();                        /* clean the LCD */
    Delay(1000);
    set_cursor(0,0);                    /* set cursor    */
    lcd_print ("     welcome !  ");     /* display       */
    set_cursor(0, 1);
    lcd_print (" www.coocox.org ");
}


/***************************************************************************//**
 * @brief Delay some time
 ******************************************************************************/
void Delay(vu32 nCount)
{
    for(; nCount != 0; nCount--);
}

/***************************************************************************//**
 * @brief  Setting all pins to output mode
 ******************************************************************************/
void LCD_ALL_DIR_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}


/***************************************************************************//**
 * @brief  Setting DATA pins to input mode
 ******************************************************************************/
void LCD_DATA_DIR_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/***************************************************************************//**
 * @brief  Setting DATA pins to output mode
 ******************************************************************************/
void LCD_DATA_DIR_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/***************************************************************************//**
 * @brief  Reading DATA pins
 * @return the data value.
 ******************************************************************************/
unsigned int LCD_DATA_IN(void)
{
    uint16_t u16Temp=0;
    u16Temp = GPIO_ReadInputData(GPIOB)&0x000F;
    return SWAP_DATA[u16Temp];
}

/***************************************************************************//**
 * @brief  Read status of LCD controller
 * @return status : Status of LCD controller
 ******************************************************************************/
static unsigned char Lcd_Read_Status (void)
{
    unsigned char status;

    LCD_DATA_DIR_IN();
    GPIO_WriteBit(GPIOA, RS, Bit_RESET);
    GPIO_WriteBit(GPIOA, RW, Bit_SET);
    Delay(10000);
    GPIO_WriteBit(GPIOA, EN, Bit_SET);
    Delay(10000);
    status  = LCD_DATA_IN() << 4;
    GPIO_WriteBit(GPIOA, EN, Bit_RESET);
    Delay(10000);
    GPIO_WriteBit(GPIOA, EN, Bit_SET);
    Delay(10000);
    status |= LCD_DATA_IN();
    GPIO_WriteBit(GPIOA, EN, Bit_RESET);
    LCD_DATA_DIR_OUT();
    return (status);
}

/***************************************************************************//**
 * @brief Wait while LCD is busy
 * @return status : Status of LCD controller
 ******************************************************************************/
static unsigned char Wait_While_Busy()
{  
    unsigned char status;
    do{
    status = Lcd_Read_Status();
    }while(status & 0x80);

    return status;
}
/***************************************************************************//**
 * @brief  Write 4-bits to LCD controller
 ******************************************************************************/
void Lcd_Write_4bits(uc8 byte)
{
    uint16_t u16Temp=0;
    GPIO_WriteBit(GPIOA, RW, Bit_RESET);
    GPIO_WriteBit(GPIOA, EN, Bit_SET);
    u16Temp = GPIO_ReadOutputData(GPIOB)&0xFFF0;
    u16Temp |=  SWAP_DATA[byte&0x0F];
    GPIO_Write(GPIOB, u16Temp);
    Delay(10000);
    GPIO_WriteBit(GPIOA, EN, Bit_RESET);
    Delay(10000);
}

/***************************************************************************//**
 * @brief:    Write command to LCD controller
 * @param[in] command :  Command to be written
 ******************************************************************************/
void Lcd_Write_Command(uc8 command)
{
    Wait_While_Busy();
    GPIO_WriteBit(GPIOA, RS, Bit_RESET);
    Lcd_Write_4bits(command>>4);
    Lcd_Write_4bits(command);
}

/***************************************************************************//**
 * @brief:     Write data to LCD controller
  * @param[in] data :  Data to be written
 ******************************************************************************/
void Lcd_Write_Data(uc8 data)
{
    Wait_While_Busy();
    GPIO_WriteBit(GPIOA, RS, Bit_SET);
    Lcd_Write_4bits(data>>4);
    Lcd_Write_4bits(data);
}

/*******************************************************************************
* @brief : Set cursor position on LCD display
* @param[in] column : Column position
* @param[in] line   : Line position
*******************************************************************************/
void set_cursor(int column, int line)
{
    unsigned char address;

    address = (line * 40) + column;
    address = 0x80 + (address & 0x7F);
    Lcd_Write_Command(address);               /* Set DDRAM address counter to 0     */
}

/***************************************************************************//**
 * @brief  Initial the LCD1602
 ******************************************************************************/
void Lcd_Init(void)
{
    char const *p;
    int i;

    /* Enable clock for peripheral        */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    Delay(1500000);
    GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
    /* Set all pins for LCD as outputs    */
    LCD_ALL_DIR_OUT();
    Delay(1500000);
    GPIO_WriteBit(GPIOA, RS, Bit_RESET);
    Lcd_Write_4bits(0x3);  /* Select 4-bit interface  */
    Delay(410000);
    Lcd_Write_4bits(0x3);
    Delay(10000);
    Lcd_Write_4bits(0x3);
    Lcd_Write_4bits(0x2);
    
    Lcd_Write_Command(0x28); /* 2 lines, 5x8 character matrix      */
    Lcd_Write_Command(0x0C); /* Display ctrl:Disp=ON,Curs/Blnk=OFF */
    Lcd_Write_Command(0x06); /* Entry mode: Move right, no shift   */

    /* Load user-specific characters into CGRAM                                 */
    Lcd_Write_Command(0x40);                  /* Set CGRAM address counter to 0     */
    p = &UserFont[0][0];
    for (i = 0; i < sizeof(UserFont); i++, p++)
        lcd_print (*p);
    Lcd_Write_Command(0x80);                 /* Set DDRAM address counter to 0     */
    GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_RESET);
}

/***************************************************************************//**
 * @brief   print a string on LCD1602.
 * @param[in] *string : point to the string which will be printed on LCD.
 ******************************************************************************/
void lcd_print (char *string)
{
    while (*string) 
    {
        Lcd_Write_Data (*string++);
    }
}

/*******************************************************************************
 * @brief  Clear the LCD display                                                        *
*******************************************************************************/
void lcd_clear (void)
{
    Lcd_Write_Command(0x01);                  /* Display clear                      */
    set_cursor (0, 0);
}
