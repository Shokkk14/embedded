#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "lcd.h"

//==========================================================================================
//   System clocks configuration.
//==========================================================================================

void RCC_Configuration(void)
{
    RCC_DeInit ();                        /* RCC system reset(for debug purpose)*/
    RCC_HSEConfig (RCC_HSE_ON);           /* Enable HSE                         */

    /* Wait till HSE is ready                                                   */
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);

    RCC_HCLKConfig   (RCC_SYSCLK_Div1);   /* HCLK   = SYSCLK                    */
    RCC_PCLK2Config  (RCC_HCLK_Div1);     /* PCLK2  = HCLK                      */
    RCC_PCLK1Config  (RCC_HCLK_Div2);     /* PCLK1  = HCLK/2                    */
    RCC_ADCCLKConfig (RCC_PCLK2_Div4);    /* ADCCLK = PCLK2/4                   */

    *(vu32 *)0x40022000 = 0x01;           /* Flash 2 wait state                 */

    /* PLLCLK = 8MHz * 3 = 24 MHz */
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_3);

    RCC_PLLCmd (ENABLE);                  /* Enable PLL                         */

    /* Wait till PLL is ready                                                   */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    /* Select PLL as system clock source                                        */
    RCC_SYSCLKConfig (RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source                             */
    while (RCC_GetSYSCLKSource() != 0x08);
}

int main(void)
{
RCC_Configuration(); // Configure the system clocks.
 LCD_Init();          // Initialize the LCD.
 LCD_Clear();         //Clear the LCD.
 LCD_GoTo(0,0);       //Go to Line 0, position 0 of LCD.
 LCD_SendText ("Luciana ");
 LCD_GoTo(1,0);       //Go to Line 1, position 2 (on the right) of LCD.
 LCD_SendText ("Eu TE AMO");
 delay_ms(1000);

while (1)

   {
   }

}
