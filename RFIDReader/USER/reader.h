#ifndef _READER_H
#define _READER_H

#include "stm32f10x.h"
#include "rc522.h"

#define PAUSE      0x55
#define WORKING    0xAA

char Read_CardID(u8 *pSnr);


#endif


