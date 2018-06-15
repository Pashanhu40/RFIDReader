#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "rc522.h"
#include "stdio.h"
#include "reader.h"
#include "cJSON.h"
#include "malloc.h"

/*ȫ�ֱ���*/
unsigned char CT[2]; //������
unsigned char SN[4]; //����
unsigned char SN_Backup[4];


int main(void)
{	
	//char status;
	unsigned char i;
	cJSON *reader = NULL;
	cJSON *seralcode = NULL;
	cJSON *sn = NULL;
	char *jsonout=0;
	
	mem_init();
	delay_init();	  	  
	LED_Init();	
	
  RC522_Init();
	
	uart_init(115200);
	
  while(1)
	{
		
    Read_CardID(SN);

		if((SN[0] != SN_Backup[0]) && (SN[1] != SN_Backup[1]) && (SN[2] != SN_Backup[2]) && (SN[3] != SN_Backup[3]))
		{
			if((SN[1] != 0X20) && (SN[2] != 0X00) &&(SN[3] != 0X20))
			{
					printf("\n%X   %X   %X   %X\n",SN[0],SN[1],SN[2],SN[3]);
					
					//��������
					reader = cJSON_CreateObject();
					if (reader == NULL)
					{
						 cJSON_Delete(reader);
						 printf("��������ʧ��\n");
					}
					//����json����
					seralcode = cJSON_CreateArray();
					if (seralcode == NULL)
					{
						cJSON_Delete(reader);
						printf("�����������\n");
					}			
					//��json�����������ֵ��
					cJSON_AddItemToObject(reader, "seralcode", seralcode);
					for(i=0;i<4;i++)
					{
						sn = cJSON_CreateNumber(SN[i]);
						if (sn == NULL)
						{
							 cJSON_Delete(reader);
							 printf("���������Ա����\n");
						}
						cJSON_AddItemToArray(seralcode,sn);
						
					}
					//��ӡjson
						jsonout = cJSON_Print(reader);
						if(jsonout == NULL)
						{
							printf("��ӡ����\n");
						}
						
						printf("%s\n",jsonout);
						//�ͷ��ڴ�
						cJSON_Delete(reader);
						myfree(jsonout);
					
					for(i=0;i<4;i++)
					{
						SN_Backup[i] = SN[i];
					}
			}
			
		}
		
		delay_ms(50);
		
		LED_INS_ON;
	}
	
}

