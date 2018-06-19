#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

//核心板指示灯
#define LED_INS_ON   PCout(13) = 0
#define LED_INS_OFF  PCout(13) = 1
//LED red
#define LED_GRE_ON   PBout(8) = 1
#define LED_RED_ON   PBout(7) = 1
#define LED_RED_OFF  PBout(7) = 0
//LED green
#define LED_GRE_OFF  PBout(8) = 0
#define LED_BLU_ON   PBout(9)	= 1
//LED blue
#define LED_BLU_OFF  PBout(9)	= 0


void LED_Init(void);//初始化

		 				    
#endif
