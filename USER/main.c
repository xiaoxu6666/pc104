#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "rs485.h"
#include "localbus.h"
#include "timer.h"



//ALIENTEK ̽����STM32F407������ ʵ��26
//RS485ͨ��ʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

uint32_t test_addr = 0xFFFFFFF;
uint8_t  test_data = 0x33;
uint8_t  read_data;

uint16_t data =0;

int main(void)
{ 
	uint32_t i = 0;
	
	u8 rs485buf[5]; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);   //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	//Localbus_GPIO_Init();
	//LED_Init();					//��ʼ��LED 
	//RS485_Init(9600);		//��ʼ��RS485����2
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
