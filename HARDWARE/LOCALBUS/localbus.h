#ifndef __LOCALBUS_H
#define __LOCALBUS_H		
#include "sys.h"	 
#include "stdlib.h" 

//#define Localbus_ARM_CS    PDout(3)
#define Localbus_MEM_W     PDout(4)
#define Localbus_MEM_R     PDout(5)    
#define Localbus_ARM_CS    PGout(0)


#define Localbus_Read_IRQHandler EXTI1_IRQHandler
#define Localbus_Read_IRQn EXTI1_IRQn


void Localbus_Exit_Init(void);

void Localbus_GPIO_Init(void);

void Localbus_WriteData(const uint32_t* addr, const uint8_t* data);
uint8_t Localbus_ReadData(const uint32_t* addr);
#endif  
	 
	 



