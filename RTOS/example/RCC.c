#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

/* Uncomment the line corresponding to the desired System clock (SYSCLK)
 frequency (after reset the HSI is used as SYSCLK source) */
#define SYSCLK_HSE
//#define SYSCLK_FREQ_24MHz
//#define SYSCLK_FREQ_36MHz
//#define SYSCLK_FREQ_48MHz
//#define SYSCLK_FREQ_56MHz
//#define SYSCLK_FREQ_72MHz
#define DELAY_COUNT    0x3FFFF

/***************************************************************************//**
 * Global variables
 ******************************************************************************/
GPIO_InitTypeDef GPIO_InitStructure;
RCC_ClocksTypeDef RCC_ClockFreq;
ErrorStatus HSEStartUpStatus;

/***************************************************************************//**
 * Declare function prototype
 ******************************************************************************/
void NVIC_Configuration(void);
void SetSysClock(void);
#ifdef SYSCLK_HSE
void SetSysClockToHSE(void);
#elif defined SYSCLK_FREQ_24MHz
void SetSysClockTo24(void);
#elif defined SYSCLK_FREQ_36MHz
void SetSysClockTo36(void);
#elif defined SYSCLK_FREQ_48MHz
void SetSysClockTo48(void);
#elif defined SYSCLK_FREQ_56MHz
void SetSysClockTo56(void);
#elif defined SYSCLK_FREQ_72MHz
void SetSysClockTo72(void);
#endif

/***************************************************************************//**
 * @brief  RCC example.This example shows how to configure the System clock(SYSCLK) 
 *            to have different frequencies: 24MHz, 36MHz, 48MHz, 56MHz and 72MHz.
 *            You will see the LED PB8 PB9 PB10 blink successively.
 ******************************************************************************/
void RCC_Init(void)
{
    /* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
    SetSysClock();

    /* This function fills the RCC_ClockFreq structure with the current
     frequencies of different on chip clocks (for debug purpose) */
    RCC_GetClocksFreq(&RCC_ClockFreq);

    /* Enable Clock Security System(CSS): this will generate an NMI exception
     when HSE clock fails */
    RCC_ClockSecuritySystemCmd(ENABLE);

    /* NVIC configuration ------------------------------------------------------*/
    NVIC_Configuration();

}

/***************************************************************************//**
 * @brief  Configures the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers.
 ******************************************************************************/
void SetSysClock(void) {
    /* The System clock configuration functions defined below assume that:
     - For Low, Medium and High density devices an external 8MHz crystal is
     used to drive the System clock.
     - For Connectivity line devices an external 25MHz crystal is used to drive
     the System clock.
     If you are using different crystal you have to adapt those functions accordingly.*/

#if defined SYSCLK_HSE
    SetSysClockToHSE();
#elif defined SYSCLK_FREQ_24MHz
    SetSysClockTo24();
#elif defined SYSCLK_FREQ_36MHz
    SetSysClockTo36();
#elif defined SYSCLK_FREQ_48MHz
    SetSysClockTo48();
#elif defined SYSCLK_FREQ_56MHz
    SetSysClockTo56();
#elif defined SYSCLK_FREQ_72MHz
    SetSysClockTo72();
#endif

    /* If none of the define above is enabled, the HSI is used as System clock
     source (default after reset) */
}

/***************************************************************************//**
 * @brief  Selects HSE as System clock source and configure HCLK, PCLK2 and PCLK1 prescalers.
 ******************************************************************************/
void SetSysClockToHSE(void) 
{
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
    /* RCC system reset(for debug purpose) */
    RCC_DeInit();

    /* Enable HSE */
    RCC_HSEConfig( RCC_HSE_ON);

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if (HSEStartUpStatus == SUCCESS) 
    {
        /* Enable Prefetch Buffer */
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

        /* Flash 0 wait state */
        FLASH_SetLatency( FLASH_Latency_0);

        /* HCLK = SYSCLK */
        RCC_HCLKConfig( RCC_SYSCLK_Div1);

        /* PCLK2 = HCLK */
        RCC_PCLK2Config( RCC_HCLK_Div1);

        /* PCLK1 = HCLK */
        RCC_PCLK1Config(RCC_HCLK_Div1);

        /* Select HSE as system clock source */
        RCC_SYSCLKConfig( RCC_SYSCLKSource_HSE);

        /* Wait till PLL is used as system clock source */
        while (RCC_GetSYSCLKSource() != 0x04) 
        {
        }
    } 
    else 
    { /* If HSE fails to start-up, the application will have wrong clock configuration.
     User can add here some code to deal with this error */

        /* Go to infinite loop */
        while (1) 
        {
        }
    }
}

/***************************************************************************//**
 * @brief  Sets System clock frequency to 24MHz and configure HCLK, PCLK2 and PCLK1 prescalers.
 ******************************************************************************/
void SetSysClockTo24(void) 
{
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
    /* RCC system reset(for debug purpose) */
    RCC_DeInit();

    /* Enable HSE */
    RCC_HSEConfig( RCC_HSE_ON);

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if (HSEStartUpStatus == SUCCESS) 
    {
        /* Enable Prefetch Buffer */
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

        /* Flash 0 wait state */
        FLASH_SetLatency( FLASH_Latency_0);

        /* HCLK = SYSCLK */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);

        /* PCLK2 = HCLK */
        RCC_PCLK2Config(RCC_HCLK_Div1);

        /* PCLK1 = HCLK */
        RCC_PCLK1Config(RCC_HCLK_Div1);

        /* PLLCLK = 8MHz * 3 = 24 MHz */
        RCC_PLLConfig(0x00010000, 0x00040000);

        /* Enable PLL */
        RCC_PLLCmd(ENABLE);

        /* Wait till PLL is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) 
        {
        }

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK);

        /* Wait till PLL is used as system clock source */
        while (RCC_GetSYSCLKSource() != 0x08) 
        {
        }
    } 
    else 
    { /* If HSE fails to start-up, the application will have wrong clock configuration.
     User can add here some code to deal with this error */

        /* Go to infinite loop */
        while (1) 
        {
        }
    }
}

/***************************************************************************//**
 * @brief  Sets System clock frequency to 36MHz and configure HCLK, PCLK2 and PCLK1 prescalers.
 ******************************************************************************/
void SetSysClockTo36(void) 
{
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
    /* RCC system reset(for debug purpose) */
    RCC_DeInit();

    /* Enable HSE */
    RCC_HSEConfig( RCC_HSE_ON);

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if (HSEStartUpStatus == SUCCESS) 
    {
        /* Enable Prefetch Buffer */
        FLASH_PrefetchBufferCmd( FLASH_PrefetchBuffer_Enable);

        /* Flash 1 wait state */
        FLASH_SetLatency( FLASH_Latency_1);

        /* HCLK = SYSCLK */
        RCC_HCLKConfig( RCC_SYSCLK_Div1);

        /* PCLK2 = HCLK */
        RCC_PCLK2Config( RCC_HCLK_Div1);

        /* PCLK1 = HCLK */
        RCC_PCLK1Config(RCC_HCLK_Div1);

        /* PLLCLK = (8MHz / 2) * 9 = 36 MHz */
        RCC_PLLConfig(0x00030000, RCC_PLLMul_9);

        /* Enable PLL */
        RCC_PLLCmd( ENABLE);

        /* Wait till PLL is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) 
        {
        }

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK);

        /* Wait till PLL is used as system clock source */
        while (RCC_GetSYSCLKSource() != 0x08) 
        {
        }
    } 
    else 
    { /* If HSE fails to start-up, the application will have wrong clock configuration.
     User can add here some code to deal with this error */

        /* Go to infinite loop */
        while (1) 
        {
        }
    }
}

/***************************************************************************//**
 * @brief  Sets System clock frequency to 48MHz and configure HCLK, PCLK2 and PCLK1 prescalers.
 ******************************************************************************/
void SetSysClockTo48(void) 
{
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
    /* RCC system reset(for debug purpose) */
    RCC_DeInit();

    /* Enable HSE */
    RCC_HSEConfig( RCC_HSE_ON);

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if (HSEStartUpStatus == SUCCESS) 
    {
        /* Enable Prefetch Buffer */
        FLASH_PrefetchBufferCmd( FLASH_PrefetchBuffer_Enable);

        /* Flash 1 wait state */
        FLASH_SetLatency( FLASH_Latency_1);

        /* HCLK = SYSCLK */
        RCC_HCLKConfig( RCC_SYSCLK_Div1);

        /* PCLK2 = HCLK */
        RCC_PCLK2Config( RCC_HCLK_Div1);

        /* PCLK1 = HCLK/2 */
        RCC_PCLK1Config( RCC_HCLK_Div2);

        /* PLLCLK = 8MHz * 6 = 48 MHz */
        RCC_PLLConfig(0x00010000, RCC_PLLMul_6);

        /* Enable PLL */
        RCC_PLLCmd( ENABLE);

        /* Wait till PLL is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) 
        {
        }

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK);

        /* Wait till PLL is used as system clock source */
        while (RCC_GetSYSCLKSource() != 0x08) 
        {
        }
    }
    else 
    { /* If HSE fails to start-up, the application will have wrong clock configuration.
     User can add here some code to deal with this error */

        /* Go to infinite loop */
        while (1)
        {
        }
    }
}

/***************************************************************************//**
 * @brief  Sets System clock frequency to 56MHz and configure HCLK, PCLK2 and PCLK1 prescalers.
 ******************************************************************************/
void SetSysClockTo56(void) 
{
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
    /* RCC system reset(for debug purpose) */
    RCC_DeInit();

    /* Enable HSE */
    RCC_HSEConfig( RCC_HSE_ON);

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if (HSEStartUpStatus == SUCCESS) 
    {
        /* Enable Prefetch Buffer */
        FLASH_PrefetchBufferCmd( FLASH_PrefetchBuffer_Enable);

        /* Flash 2 wait state */
        FLASH_SetLatency( FLASH_Latency_2);

        /* HCLK = SYSCLK */
        RCC_HCLKConfig( RCC_SYSCLK_Div1);

        /* PCLK2 = HCLK */
        RCC_PCLK2Config( RCC_HCLK_Div1);

        /* PCLK1 = HCLK/2 */
        RCC_PCLK1Config( RCC_HCLK_Div2);

        /* PLLCLK = 8MHz * 7 = 56 MHz */
        RCC_PLLConfig(0x00010000, RCC_PLLMul_7);

        /* Enable PLL */
        RCC_PLLCmd( ENABLE);

        /* Wait till PLL is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK);

        /* Wait till PLL is used as system clock source */
        while (RCC_GetSYSCLKSource() != 0x08) 
        {
        }
    } 
    else 
    { /* If HSE fails to start-up, the application will have wrong clock configuration.
     User can add here some code to deal with this error */

        /* Go to infinite loop */
        while (1) 
        {
        }
    }
}

/***************************************************************************//**
 * @brief  Sets System clock frequency to 72MHz and configure HCLK, PCLK2 and PCLK1 prescalers.
 ******************************************************************************/
void SetSysClockTo72(void)
{
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
    /* RCC system reset(for debug purpose) */
    RCC_DeInit();

    /* Enable HSE */
    RCC_HSEConfig( RCC_HSE_ON);

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if (HSEStartUpStatus == SUCCESS) 
    {
        /* Enable Prefetch Buffer */
        FLASH_PrefetchBufferCmd( FLASH_PrefetchBuffer_Enable);

        /* Flash 2 wait state */
        FLASH_SetLatency( FLASH_Latency_2);

        /* HCLK = SYSCLK */
        RCC_HCLKConfig( RCC_SYSCLK_Div1);

        /* PCLK2 = HCLK */
        RCC_PCLK2Config( RCC_HCLK_Div1);

        /* PCLK1 = HCLK/2 */
        RCC_PCLK1Config( RCC_HCLK_Div2);

        /* PLLCLK = 8MHz * 9 = 72 MHz */
        RCC_PLLConfig(0x00010000, RCC_PLLMul_9);

        /* Enable PLL */
        RCC_PLLCmd( ENABLE);

        /* Wait till PLL is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) 
        {
        }

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK);

        /* Wait till PLL is used as system clock source */
        while (RCC_GetSYSCLKSource() != 0x08) 
        {
        }
    } 
    else 
    { /* If HSE fails to start-up, the application will have wrong clock configuration.
     User can add here some code to deal with this error */

        /* Go to infinite loop */
        while (1) 
        {
        }
    }
}

/***************************************************************************//**
 * @brief  Configures Vector Table base location.
 ******************************************************************************/
void NVIC_Configuration(void) 
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable and configure RCC global IRQ channel */
    NVIC_InitStructure.NVIC_IRQChannel = RCC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/***************************************************************************//**
 * @brief  This function handles RCC interrupt request.
 ******************************************************************************/
void RCC_IRQHandler(void) 
{
    if (RCC_GetITStatus(RCC_IT_HSERDY) != RESET) 
    {
        /* Clear HSERDY interrupt pending bit */
        RCC_ClearITPendingBit( RCC_IT_HSERDY);

        /* Check if the HSE clock is still available */
        if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET) 
        {
            /* Enable PLL: once the PLL is ready the PLLRDY interrupt is generated */
            RCC_PLLCmd( ENABLE);
        }
    }

    if (RCC_GetITStatus(RCC_IT_PLLRDY) != RESET) 
    {
        /* Clear PLLRDY interrupt pending bit */
        RCC_ClearITPendingBit( RCC_IT_PLLRDY);

        /* Check if the PLL is still locked */
        if (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != RESET) 
        {
            /* Select PLL as system clock source */
            RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK);
        }
    }
}

/***************************************************************************//**
 * @brief  This function handles NMI exception.
 ******************************************************************************/
void NMI_Handler(void) 
{
    /* This interrupt is generated when HSE clock fails */

    if (RCC_GetITStatus(RCC_IT_CSS) != RESET)
    {
    	/* At this stage: HSE, PLL are disabled (but no change on PLL config) and HSI
        is selected as system clock source */

        /* Enable HSE */
        RCC_HSEConfig( RCC_HSE_ON);

        /* Enable HSE Ready interrupt */
        RCC_ITConfig(RCC_IT_HSERDY, ENABLE);

        /* Enable PLL Ready interrupt */
        RCC_ITConfig(RCC_IT_PLLRDY, ENABLE);

        /* Clear Clock Security System interrupt pending bit */
        RCC_ClearITPendingBit( RCC_IT_CSS);

        /* Once HSE clock recover, the HSERDY interrupt is generated and in the RCC ISR
         routine the system clock will be reconfigured to its previous state (before
         HSE clock failure) */
    }
}
