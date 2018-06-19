#ifndef _READER_H
#define _READER_H

#include "stm32f10x.h"
#include "rc522.h"

#define mydebug  0

enum reader_t
{
	WRITE_SUCCESS = 50,
	WRITE_FAILE,
	START_SIGNIN,
	STOP_SIGNIN,

};


char Read_SeralCode(u8 *pSnr);
char TX_cJSON_SeralCode(u8 *pSnr);
unsigned char  RX_cJSON_Parse(char *pSnr);

void Write_Success(void);
void Write_Faile(void);
void Start_Signin(void);
void Stop_Signin(void);

#endif


