/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"

#define NUM 10

int i,j;
char name[NUM+1] = {'\0'};
volatile char StringLoop[17];
static int tx_index = 0;
static int rx_index = 0;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;

/* Private function prototypes -----------------------------------------------*/
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void USART_Configuration(void);
void USART1_IRQHandler(void);
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount);

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles USARTx global interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{

	  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // Received characters modify string
	  {
		GPIO_WriteBit(GPIOC,GPIO_Pin_0,Bit_SET);
	    StringLoop[rx_index++] = USART_ReceiveData(USART1);

	    if (rx_index >= (sizeof(StringLoop) - 1))
	      rx_index = 0;
	  }

		LCD_Clear();         //Clear the LCD.
		LCD_GoTo(0,0);       //Go to Line 0, position 0 of LCD.
		LCD_SendText(StringLoop);


//	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
//	{
//    	GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_SET);
//			i = USART_ReceiveData(USART1);
//			if(j == NUM)
//			{
//                name[j] = i;
//			    j = 0;
//			}
//			else
//			{
//                name[j++] = i;
//			}
//			name[j] = '\0';
//	}
    GPIO_WriteBit(GPIOC,GPIO_Pin_0,Bit_RESET);
}

void USART2_IRQHandler(void)
{

	  if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) // Received characters modify string
	  {
		GPIO_WriteBit(GPIOC,GPIO_Pin_2,Bit_SET);
	    StringLoop[rx_index++] = USART_ReceiveData(USART2);

	    if (rx_index >= (sizeof(StringLoop) - 1))
	      rx_index = 0;
	  }

		LCD_Clear();         //Clear the LCD.
		LCD_GoTo(0,0);       //Go to Line 0, position 0 of LCD.
		LCD_SendText(StringLoop);


//	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
//	{
//    	GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_SET);
//			i = USART_ReceiveData(USART1);
//			if(j == NUM)
//			{
//                name[j] = i;
//			    j = 0;
//			}
//			else
//			{
//                name[j++] = i;
//			}
//			name[j] = '\0';
//	}
    GPIO_WriteBit(GPIOC,GPIO_Pin_2,Bit_RESET);
}

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : usart_rxtx
* Description    : Print "Welcome to CooCox!"  on Hyperterminal via USART1.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void usart_init(void)
{
		NVIC_USART1_Configuration();
		NVIC_USART2_Configuration();
	    USART_Configuration();
	    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : Configures the USART1.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART_Configuration(void)
{
	USART1_Configuration();
	USART2_300_7E1();
}


void USART2_300_7E1(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 300;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Cmd(USART2, DISABLE);
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
}

void USART2_9600_7E1(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Cmd(USART2, DISABLE);
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
}

void USART2_9600_8N1(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Cmd(USART2, DISABLE);
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
}

void USART1_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_USART1_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void NVIC_USART2_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : UARTSend
* Description    : Send a string to the UART.
* Input          : - pucBuffer: buffers to be printed.
*                : - ulCount  : buffer's length
* Output         : None
* Return         : None
*******************************************************************************/
void UART1Send(const unsigned char *pucBuffer, unsigned long ulCount)
{
    while(ulCount--)
    {
    	GPIO_WriteBit(GPIOC,GPIO_Pin_1,Bit_SET);
        USART_SendData(USART1, *pucBuffer++);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        {
        }
    }
    GPIO_WriteBit(GPIOC,GPIO_Pin_1,Bit_RESET);
}

void UART2Send(const unsigned char *pucBuffer, unsigned long ulCount)
{
    while(ulCount--)
    {
    	GPIO_WriteBit(GPIOC,GPIO_Pin_3,Bit_SET);
        USART_SendData(USART2, *pucBuffer++);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
        {
        }
    }
    GPIO_WriteBit(GPIOC,GPIO_Pin_3,Bit_RESET);
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
