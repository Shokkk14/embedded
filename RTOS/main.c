#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stdio.h>
#include <CoOs.h>

void RCC_Init(void);
void board_map(void);
void usart_init(void);
int delay = 5;
int flag = 0;
int btStatus = 0;

#define STACK_SIZE_DEFAULT 128

OS_STK blinkLED_stk[STACK_SIZE_DEFAULT];
OS_STK btPRESS_stk[STACK_SIZE_DEFAULT];
OS_STK updateLCD_stk[STACK_SIZE_DEFAULT];
OS_STK USART1_stk[STACK_SIZE_DEFAULT];

void initializeBoard(){

	RCC_Init();
	board_map();//Configura todas as portas IO e perifericos
	usart_init();

	 LCD_Initialization();          // Initialize the LCD.
	 LCD_Clear();         //Clear the LCD.
	 LCD_GoTo(0,0);       //Go to Line 0, position 0 of LCD.
	 LCD_SendText ("SL7000 Bluetooth");
	 LCD_GoTo(1,0);       //Go to Line 1, position 2 (on the right) of LCD.
	 LCD_SendText ("IEC  Auto Detect");
}

void blinkLED (void* pdata){
	while(1){
		GPIO_WriteBit(GPIOC,GPIO_Pin_8,Bit_SET);
		CoTickDelay (delay);
		GPIO_WriteBit(GPIOC,GPIO_Pin_8,Bit_RESET);
		CoTickDelay (delay);
	}
}

void btPRESS (void* pdata){
	while(1){
		flag = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
		if(flag==1){
			if(btStatus==0)
			{
				GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_SET);
			    const unsigned char menu[] = "/?!\r\n";
			    UART1Send(menu, sizeof(menu));
			    UART2Send(menu, sizeof(menu));
				LCD_Clear();         //Clear the LCD.
				LCD_GoTo(0,0);       //Go to Line 0, position 0 of LCD.
				LCD_SendText("@300 7E1");
				LCD_GoTo(1,0);       //Go to Line 1, position 2 (on the right) of LCD.
				LCD_SendText (menu);
			}
			CoTickDelay (10);
			btStatus = 1;
			GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_RESET);
		}else{
			GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_RESET);
			btStatus = 0;
		}
	}
}

void USART_rx (void* pdata){

//	while(1)
//	{
//		USART_IRQHandler();
//		CoTickDelay(10);
//	}

}

int main(void)
{
	initializeBoard();
	CoInitOS();
	//CoCreateTask(USART_rx,0,0,&USART1_stk[STACK_SIZE_DEFAULT-1],STACK_SIZE_DEFAULT);
	CoCreateTask(blinkLED,0,0,&blinkLED_stk[STACK_SIZE_DEFAULT-1],STACK_SIZE_DEFAULT);
	CoCreateTask(btPRESS,0,1,&btPRESS_stk[STACK_SIZE_DEFAULT-1],STACK_SIZE_DEFAULT);
	//CoCreateTask(GPIO_LCD1602,0,0,&updateLCD_stk[STACK_SIZE_DEFAULT-1],STACK_SIZE_DEFAULT);
	CoStartOS();

	while(1);
}

