#include "reader.h"
#include "stdio.h"
#include "cJSON.h"
#include "malloc.h"
#include "led.h"
#include "delay.h"


extern unsigned char SN[4];
extern unsigned char SN_Backup[4];
extern enum reader_t READER_STATUS;

char Read_SeralCode(u8 *pSnr)
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
				 
				 //��ȡ��ɺ�λ
				 status = PcdReset();
			 }
		 }
	  return status;
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         :
* Return         : 0���ɹ�
*   							 1����������ʧ��
*									 2�������������
*                  3�����������Ա����
*								   4����ӡ����
*                  5���ظ���ȡ
*******************************************************************************/
char TX_cJSON_SeralCode(u8 *pSnr)
{
	char status = 5;
	unsigned char i;
	cJSON *reader = NULL;
	cJSON *seralcode = NULL;
	cJSON *sn = NULL;
	char *jsonout=0;
	
	//һ�ſ�ֻ��һ�Σ����ظ���ȡ
	if((*(pSnr+0) != SN_Backup[0]) && (*(pSnr+1) != SN_Backup[1]) && (*(pSnr+2) != SN_Backup[2]) && (*(pSnr+3) != SN_Backup[3]))
	{
			//RC522���������л���� (** 20 00 20) �����룬�����������޳�
			if((*(pSnr+1) != 0X20) && (*(pSnr+2) != 0X00) && (*(pSnr+3) != 0X20))
			{
				 #if mydebug
					printf("\n%X   %X   %X   %X\n",*(pSnr+0),*(pSnr+1),*(pSnr+2),*(pSnr+3));
				#endif	
				
					//��������
					reader = cJSON_CreateObject();
					if (reader == NULL)
					{
						 cJSON_Delete(reader);
						
						#if mydebug
					  printf("��������ʧ��\n");
						#endif

						return status = 1;
					}
					//����json����
					seralcode = cJSON_CreateArray();
					if (seralcode == NULL)
					{
						cJSON_Delete(reader);
						
						#if mydebug
						printf("�����������\n");
						#endif
						
						return status = 2;
					}			
					//��json�����������ֵ��
					cJSON_AddItemToObject(reader, "seralcode", seralcode);
					for(i=0;i<4;i++)
					{
						sn = cJSON_CreateNumber(*(pSnr+i));
						if (sn == NULL)
						{
							 cJSON_Delete(reader);
							 
							#if mydebug
							 printf("���������Ա����\n");
							#endif
							
							return status = 3;
						}
						cJSON_AddItemToArray(seralcode,sn);
						
					}
					//��ӡjson
						jsonout = cJSON_Print(reader);
						if(jsonout == NULL)
						{
							
							#if mydebug
							printf("��ӡ����\n");
							#endif
							
							return status = 4;
						}
						
						printf("%s\n",jsonout);
						status = 0;
						
						//�ͷ��ڴ�
						cJSON_Delete(reader);
						myfree(jsonout);
					
					for(i=0;i<4;i++)
					{
						SN_Backup[i] = *(pSnr+i);
					}
		 }
	}
	
	return status;
}


unsigned char RX_cJSON_Parse(char *pSnr)
{
	cJSON *root = NULL;
	cJSON *command = NULL;
  unsigned char buff;
	
	root = cJSON_Parse(pSnr);
	
	#if mydebug
	 if (!root)  
    {  
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());    
    }  
	#endif
		
	command = cJSON_GetObjectItem(root,"command");
	
	#if mydebug
	printf("%d\n",command->valueint);
	#endif
		
	buff = command->valueint;
		
	cJSON_Delete(root);
		
	return buff;
		
}


void Write_Success(void)
{
	LED_BLU_OFF;
	LED_RED_OFF;
	LED_GRE_ON;
	delay_ms(500);
	LED_GRE_OFF;
	
	READER_STATUS = START_SIGNIN;
}

void Write_Faile(void)
{
	unsigned char i;
	
	LED_BLU_OFF;
	LED_GRE_OFF;
	LED_RED_OFF;
	
	RC522_Init();
	
	for(i=0;i<4;i++)
	{
		SN_Backup[i] = 0;
	}
	
	TX_cJSON_SeralCode(SN);
}

void Start_Signin(void)
{
	unsigned char buff;
	LED_BLU_ON;
	LED_GRE_OFF;
	LED_RED_OFF;
	
	//��ȡ�������к�
	Read_SeralCode(SN);
	
	//JSON����������кŲ����͸�APP
	buff = TX_cJSON_SeralCode(SN);
	
	//����ֵΪ5��˵�����ͳɹ��ȴ�APP����Ӧ��
  if(buff == 0)
	{
		READER_STATUS = STOP_SIGNIN;
	}
	//����ֵΪ5��˵���ܱ��޿�������ͬһ�ſ�,��������
  else if(buff == 5)
	{
		READER_STATUS = START_SIGNIN;
	}
	//��������ֵΪ����
	else
	{
		READER_STATUS = WRITE_FAILE;
	}
}

void Stop_Signin(void)
{
	LED_BLU_OFF;
	LED_GRE_OFF;
	LED_RED_ON;
  
}





