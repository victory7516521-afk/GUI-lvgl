#include "delay.h"


void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;	//系统定时器的重载值	    	 
	ticks=nus*(SystemCoreClock/1000000);//需要的节拍数 
	told=SysTick->VAL;        	//刚进入时的计数器值
 
	/* 挂起调度器[可选,会导致高优先级任务无法抢占当前任务，但能够提高当前任务时间的精确性] */
	vTaskSuspendAll();	
 
	while(1)
	{
		tnow=SysTick->VAL;
		
		if(tnow!=told)
		{	 
			/* SYSTICK是一个递减的计数器 */
			if(tnow<told)
				tcnt+=told-tnow;		
			else 
				tcnt+=reload-tnow+told+1;	  
			
			told=tnow;
			
			/* 时间超过/等于要延迟的时间,则退出。*/
			if(tcnt>=ticks)
				break;			
		}  
	}

	/* 恢复调度器[可选] */
	xTaskResumeAll();
} 


void delay_ms(uint32_t nms)
{
	vTaskDelay(nms);
}

//粗延时(就是延时不一定准确的意思)
void delay(int n)
{
    int i, j;
    
    for(i=0; i<n; i++)
        for(j=0; j<35000; j++);
}