/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"


void open_iec1107_modeE(void);
void open_iec1107_modeC(void);
void close_iec1107(void);
void auto_detect(void);

const unsigned char frame1[] = "/?!\r\n";
const unsigned char frame2[] = "/?!\r\n";

void open_iec1107_modeE(void)
{
	USART2_300_7E1();




}

void open_iec1107_modeC(void)
{

}

void close_iec1107(void)
{

}

void auto_detect(void)
{
	USART2_300_7E1();
}
