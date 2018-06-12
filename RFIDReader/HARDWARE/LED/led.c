#include "led.h"	   

//PB7---red
//PB8---green
//PB9---bule
//PC13---ºËÐÄ°åÖ¸Ê¾µÆ

void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
 GPIO_Init(GPIOB, &GPIO_InitStructure);					
 GPIO_ResetBits(GPIOB,GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);		

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	    		 
 GPIO_Init(GPIOC, &GPIO_InitStructure);	  			
 GPIO_SetBits(GPIOC,GPIO_Pin_13); 						 
}
 
