#include "reader.h"
#include "stdio.h"


char Read_CardID(u8 *pSnr)
{

	char status = MI_ERR;
	unsigned char card_type[2];
	
	status = PcdRequest(PICC_REQALL,card_type);
	//Ѱ�����ɹ�
	if(status == MI_OK)
		{
			 status = MI_ERR;
			 status = PcdAnticoll(pSnr);
			 //����ײ���ɹ�
			 if(status == MI_OK)
			 {
				 status = MI_ERR;
				 //��ȡ��ɺ�����
				 status = PcdHalt();
				 
				 //status = PcdReset();
			 }
		 }
	  return status;
}
		









