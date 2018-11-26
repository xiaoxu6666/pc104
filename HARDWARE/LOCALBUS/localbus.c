#include "sys.h"	
#include "localbus.h"

uint16_t temp[100];
uint16_t i = 0;

uint8_t Localbus_ARM_WAIT = 0;


GPIO_TypeDef* Localbus_ADDR_GPIO_type[18] = {GPIOF, GPIOF, GPIOF, GPIOF, GPIOF, GPIOF, GPIOF, GPIOF, GPIOF, GPIOF, GPIOD, GPIOD, GPIOD, GPIOD, GPIOE, GPIOE, GPIOE, GPIOE};
uint16_t Localbus_ADDR_GPIO_pins[18] = {GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_5, GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15, 
                                        GPIO_Pin_14, GPIO_Pin_15, GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10};
uint32_t Localbus_ADDR_mask[18] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000, 0x10000, 0x20000};

GPIO_TypeDef* Localbus_WR_GPIO_type[8] = {GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOD, GPIOD, GPIOD};
uint16_t Localbus_WR_GPIO_pins[8] = {GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10};
uint8_t Localbus_WR_mask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

void Localbus_GPIO_Init(void) { 	
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOG, ENABLE);//使能PD,PE时钟  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//复用输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//复用输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化  
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//PE7~15,AF OUT
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//LED0和LED1对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO
}

void Localbus_Exit_Init(void) {
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
  Localbus_GPIO_Init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource1);
	
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;//LINE10
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE10
  EXTI_Init(&EXTI_InitStructure);//配置
	
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//外部中断10
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
}

void Localbus_WriteGPIO(uint8_t data) {
	uint8_t pis = 0;
	uint8_t bit = 0;
  for (pis = 0; pis < 8; pis ++) {
	  bit = (uint8_t)(data & Localbus_WR_mask[pis]) >> pis;
		GPIO_WriteBit(Localbus_WR_GPIO_type[pis], Localbus_WR_GPIO_pins[pis], bit);
    data = data << 1;
	}
}

void Localbus_WriteAddr(uint32_t addr) {
	uint8_t pis = 0;
	uint8_t bit = 0;
	addr = addr & 0x3FFFF;
	for (pis = 0; pis < 18; pis ++) {
		bit = (uint8_t)(addr & Localbus_ADDR_mask[pis]) >> pis;
		GPIO_WriteBit(Localbus_ADDR_GPIO_type[pis], Localbus_ADDR_GPIO_pins[pis], bit);
    addr = addr << 1;
  }
}
void Localbus_WriteData(const uint32_t* addr, const uint8_t* data) {
	//Localbus_WriteAddr(*addr);
  Localbus_WriteGPIO(*data);
	Localbus_MEM_W = 0;
	Localbus_ARM_CS = 0;
	while (!Localbus_ARM_WAIT);
	Localbus_MEM_W = 1;
	Localbus_ARM_CS = 1 ;
	Localbus_ARM_WAIT = 0; 
}

void Creat_localbus_write_signal(){
    
}


uint16_t Localbus_ReadGPIO(void) {
	uint8_t data;
	uint8_t pis = 0;
	for (pis = 0; pis < 7; pis ++) {
		data |= GPIO_ReadInputDataBit(Localbus_WR_GPIO_type[pis], Localbus_WR_GPIO_pins[pis]);
    data = data << 1;
	}
	data |= GPIO_ReadInputDataBit(Localbus_WR_GPIO_type[pis], Localbus_WR_GPIO_pins[pis]);
  return data;
}

uint8_t Localbus_ReadData(const uint32_t* addr) {
	uint8_t temp;
	//Localbus_WriteAddr(*addr);
	Localbus_MEM_R = 0;
	Localbus_ARM_CS = 0;
  while (!Localbus_ARM_WAIT);
	Localbus_ARM_WAIT = 0;
	Localbus_MEM_R = 1;
	Localbus_ARM_CS = 1;
  temp = Localbus_ReadGPIO();
  return temp;
}


void Localbus_Read_IRQHandler(void) {
	Localbus_ARM_WAIT = 1;
	EXTI_ClearITPendingBit(EXTI_Line1);//清除LINE4上的中断标志位  
}

void Local_put_bytes(const uint32_t *addr, const uint8_t *data, uint16_t len) {
  while (len--) {
	  Localbus_WriteData(addr, data);
		addr++;
		data++;
  }
}
