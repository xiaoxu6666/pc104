#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "rs485.h"
#include "localbus.h"
#include "timer.h"



//ALIENTEK 探索者STM32F407开发板 实验26
//RS485通信实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

uint32_t test_addr = 0xFFFFFFF;
uint8_t  test_data = 0x33;
uint8_t  read_data;

uint16_t data =0;

int main(void)
{ 
	uint32_t i = 0;
	
	u8 rs485buf[5]; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);   //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	//Localbus_GPIO_Init();
	//LED_Init();					//初始化LED 
	//RS485_Init(9600);		//初始化RS485串口2
	Localbus_Exit_Init();
	TIM3_Int_Init(10-1,83); 									  
  Localbus_MEM_R = 1;
	Localbus_ARM_CS = 1;
	Localbus_MEM_W = 1;
	
	
  while (1) {
		Localbus_WriteData(&test_addr, &test_data);
		delay_ms(1000);
		read_data = Localbus_ReadData(&test_addr);
	  //for (i = 0; i < 1000; i++)
		//  data = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10);
		
	}
}
