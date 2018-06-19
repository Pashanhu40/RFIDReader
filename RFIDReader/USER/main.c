#include "stm32f10x.h"
#include "delay.h"
#include "uart.h"
#include "led.h"
#include "rc522.h"
#include "stdio.h"
#include "reader.h"
#include "cJSON.h"
#include "malloc.h"


/*ȫ�ֱ���*/
unsigned char SN[4]; //��������
unsigned char SN_Backup[4]; //�������뱸��
enum reader_t READER_STATUS = STOP_SIGNIN; //RFID������ִ�е�ָ��

unsigned char RX_STATE = 0;  //�Ƿ������һ֡״̬��־λ
char RX_cJSON[200] = {0};    //���ջ�����
unsigned char RX_BYTE_LEN = 0; 


int main(void)
{	
	unsigned char i;
	unsigned char cmd_buff;
	
	mem_init();
	delay_init();	  	  
	LED_Init();	
	
  RC522_Init();
	uart_init(9600);
	
  while(1)
	{
		//�ж��Ƿ������һ֡����
		if(RX_STATE == 1)
		{
			cmd_buff = RX_cJSON_Parse(RX_cJSON);
			if(cmd_buff == WRITE_SUCCESS) READER_STATUS = WRITE_SUCCESS;
			else if(cmd_buff == WRITE_FAILE) READER_STATUS = WRITE_FAILE;
			else if(cmd_buff == START_SIGNIN) READER_STATUS = START_SIGNIN;
			else if(cmd_buff == STOP_SIGNIN) READER_STATUS = STOP_SIGNIN;
			
			//���ջ��������Ƿ������һ֡״̬��־λ�������ֽڳ��ȱ�־λ����
			for(i=RX_BYTE_LEN;i>0;i--) RX_cJSON[i] = 0;
			RX_STATE = 0;
			RX_BYTE_LEN = 0;
		}
		
		switch(READER_STATUS)
		{
			
			case WRITE_SUCCESS://50:Android��д��ɹ�
				Write_Success();
				break;
			
			case WRITE_FAILE://51:Android��д��ʧ��
				Write_Faile();
				break;
			
			case START_SIGNIN://52:��ʼ��
				Start_Signin();
				break;
			
			case STOP_SIGNIN://53:ֹͣ��
				Stop_Signin();
			  break;
			
			default:
        Write_Faile();
				break;

		}
		
		delay_ms(50);
		
		LED_INS_ON;
	}
	
}

