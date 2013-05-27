void Tim2toTrigTim3Ch3OPM(void)
{
	/*
	 * Use my previous example and disable "TIM_Cmd(TIM3, ENABLE);" line
	 * 
	 * */
	

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

//enable clocks to tim2 and gpio port a
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	
	//init port A,pin9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
		GPIO_Init(GPIOA, &GPIO_InitStructure);


	NVIC_InitTypeDef NVIC_InitStructure;

	//enable tim2 irq
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);


//setting timer 2 interrupt to 1hz ((2400000/24000)*1000)s
	TIM_TimeBaseStructure.TIM_Prescaler = 24000 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 1000-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	 /* TIM IT enable */
	   TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	   /* TIM2 enable counter */
	   TIM_Cmd(TIM2, ENABLE);

}
//timer 2 interrupt
void TIM2_IRQHandler(void){
	
	//if interrupt happens the do this
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
	//clear interrupt and start counting again to get precise freq
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	
	//enable tim3 to one pulse
	TIM_Cmd(TIM3,ENABLE);
	//toggle led to see some chance in port A pin 9
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_9) == RESET){
			GPIO_WriteBit(GPIOA,GPIO_Pin_9,SET);
	}else{
				GPIO_WriteBit(GPIOA,GPIO_Pin_9,RESET);
			}
	}
}