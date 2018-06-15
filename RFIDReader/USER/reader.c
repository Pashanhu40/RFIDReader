#include "reader.h"
#include "stdio.h"


char Read_CardID(u8 *pSnr)
{

	char status = MI_ERR;
	unsigned char card_type[2];
	
	status = PcdRequest(PICC_REQALL,card_type);
	//寻卡检测成功
	if(status == MI_OK)
		{
			 status = MI_ERR;
			 status = PcdAnticoll(pSnr);
			 //防碰撞检测成功
			 if(status == MI_OK)
			 {
				 status = MI_ERR;
				 //读取完成后休眠
				 status = PcdHalt();
				 
				 //status = PcdReset();
			 }
		 }
	  return status;
}
		









