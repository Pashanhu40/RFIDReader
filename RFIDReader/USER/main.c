#include "stm32f10x.h"
#include "delay.h"
#include "uart.h"
#include "led.h"
#include "rc522.h"
#include "stdio.h"
#include "reader.h"
#include "cJSON.h"
#include "malloc.h"


/*全局变量*/
unsigned char SN[4]; //卡序列码
unsigned char SN_Backup[4]; //卡序列码备份
enum reader_t READER_STATUS = STOP_SIGNIN; //RFID读卡器执行的指令

unsigned char RX_STATE = 0;  //是否接受完一帧状态标志位
char RX_cJSON[200] = {0};    //接收缓冲区
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
		//判断是否接受完一帧数据
		if(RX_STATE == 1)
		{
			cmd_buff = RX_cJSON_Parse(RX_cJSON);
			if(cmd_buff == WRITE_SUCCESS) READER_STATUS = WRITE_SUCCESS;
			else if(cmd_buff == WRITE_FAILE) READER_STATUS = WRITE_FAILE;
			else if(cmd_buff == START_SIGNIN) READER_STATUS = START_SIGNIN;
			else if(cmd_buff == STOP_SIGNIN) READER_STATUS = STOP_SIGNIN;
			
			//接收缓冲区、是否接收完一帧状态标志位、接收字节长度标志位清零
			for(i=RX_BYTE_LEN;i>0;i--) RX_cJSON[i] = 0;
			RX_STATE = 0;
			RX_BYTE_LEN = 0;
		}
		
		switch(READER_STATUS)
		{
			
			case WRITE_SUCCESS://50:Android端写入成功
				Write_Success();
				break;
			
			case WRITE_FAILE://51:Android端写入失败
				Write_Faile();
				break;
			
			case START_SIGNIN://52:开始打卡
				Start_Signin();
				break;
			
			case STOP_SIGNIN://53:停止打卡
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

