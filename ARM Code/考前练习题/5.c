
#include <stdint.h>
#include <stdbool.h>
#include "hw_memmap.h"
#include "debug.h"
#include "gpio.h"
#include "hw_i2c.h"
#include "hw_types.h"
#include "i2c.h"
#include "pin_map.h"
#include "sysctl.h"
#include "systick.h"
#include "interrupt.h"
#include "uart.h"
#include "hw_ints.h"
#include "cstring"
//*****************************************************************************
//
//I2C GPIO chip address and resigster define
//
//*****************************************************************************
#define TCA6424_I2CADDR 					0x22
#define PCA9557_I2CADDR						0x18

#define PCA9557_INPUT							0x00
#define	PCA9557_OUTPUT						0x01
#define PCA9557_POLINVERT					0x02
#define PCA9557_CONFIG						0x03

#define TCA6424_CONFIG_PORT0			0x0c
#define TCA6424_CONFIG_PORT1			0x0d
#define TCA6424_CONFIG_PORT2			0x0e

#define TCA6424_INPUT_PORT0				0x00
#define TCA6424_INPUT_PORT1				0x01
#define TCA6424_INPUT_PORT2				0x02

#define TCA6424_OUTPUT_PORT0			0x04
#define TCA6424_OUTPUT_PORT1			0x05
#define TCA6424_OUTPUT_PORT2			0x06
#define SYSTICK_FREQUENCY					1000			//1000hz



void 		Delay(uint32_t value);
void 		S800_GPIO_Init(void);
uint8_t 	I2C0_WriteByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t WriteData);
uint8_t 	I2C0_ReadByte(uint8_t DevAddr, uint8_t RegAddr);
void		S800_I2C0_Init(void);
volatile uint8_t result;
uint32_t ui32SysClock;
uint8_t seg7[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x58,0x5e,0x079,0x71,0x5c};
void 		S800_UART_Init(void);
void S800_SysTick_init(void);
uint32_t systick_10ms_counter,systick_100ms_counter;
uint8_t systick_10ms_counter_status,systick_100ms_counter_status;
uint8_t cnt_500,cnt_1000,a=5,b=9,c=5,d=0;
uint8_t key,last,start=0,led=0x0f,ledcount=0,jiana,jianb,jianc,jiand;
char cmd[1];
void 		UARTStringPutNonBlocking(const char *cMessage);

#define	I2C_FLASHTIME							1000				//1000mS,I2C定时时长
#define GPIO_FLASHTIME						300				//30mS,GPIO定时时长

void displayA(void)
{
	
			if(systick_100ms_counter_status==1)
			{
				systick_100ms_counter_status=0;
				if(++cnt_1000 >= I2C_FLASHTIME/100)  //??
				{
						cnt_1000=0;
	//每秒变化					
				if(d-1>=0) {d=d-1;jiand=0;}
				else jiand=1;
				if(d==0&&jiand==1)
				{
					d=9;
					if(c-1>=0) {c=c-1;jianc=0;}
					else jianc=1;
					if(c==0&&jianc==1)
					{
						c=5;
						if(b-1>=0) {b=b-1;jianb=0;}
						else jianb=1;
						if(b==0&&jianb==1)
						{
							b=9;
							if(a-1>=0) {a=a-1;jiana=0;}
							else jiana=1;
							if(a==0&&jiana==1) a=5;
						}
					}
				}
					
	
			}
		
			if(++cnt_500 >= 5)  //5次100ms=500ms
				{
						cnt_500=0;
				
			if(	ledcount)
			{
				result = I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_OUTPUT,~led);	 
				ledcount=!ledcount;
			}
			else
			{
				result = I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_OUTPUT,0xff);
				ledcount=!ledcount;
			}
		}
		}
}


void displayB(void)
{
		if(systick_100ms_counter_status==1)
			{
				systick_100ms_counter_status=0;
			if(++cnt_500 >= 5)  //5次100ms=500ms
				{
						cnt_500=0;
				
			if(	ledcount)
			{
				result = I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_OUTPUT,~0x07);	 
				ledcount=!ledcount;
			}
			else
			{
				result = I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_OUTPUT,0xff);
				ledcount=!ledcount;
			}
		}
	}
		
}


void displayC(void)
{
	
			if(systick_100ms_counter_status==1)
			{
				systick_100ms_counter_status=0;
				if(++cnt_1000 >= I2C_FLASHTIME/100)  //??
				{
						cnt_1000=0;
	//每秒变化加顺序技计数				
				if(d+1<=9) {d=d+1;jiand=0;}
				else jiand=1;
				if(d==9&&jiand==1)
				{
					d=0;
					if(c+1<=5) {c=c+1;jianc=0;}
					else jianc=1;
					if(c==5&&jianc==1)
					{
						c=0;
						if(b+1<=9) {b=b+1;jianb=0;}
						else jianb=1;
						if(b==9&&jianb==1)
						{
							b=0;
							if(a+1<=5) {a=a+1;jiana=0;}
							else jiana=1;
							if(a==5&&jiana==1) a=0;
						}
					}
				}
					
	
			}
		
			if(++cnt_500 >= 5)  //5次100ms=500ms
				{
						cnt_500=0;
				
			if(	ledcount)
			{
				result = I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_OUTPUT,~led);	 
				ledcount=!ledcount;
			}
			else
			{
				result = I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_OUTPUT,0xff);
				ledcount=!ledcount;
			}
		}
		}
}

void displayD(void)
{
	
			if(systick_100ms_counter_status==1)
			{
				systick_100ms_counter_status=0;
				if(++cnt_1000 >= I2C_FLASHTIME/100)  //??
				{
						cnt_1000=0;
	//每秒变化加顺序技计数				
					a=0;b=0;c=0;d=0;
				}
			if(++cnt_500 >= 5)  //5次100ms=500ms
				{
						cnt_500=0;
				
			if(	ledcount)
			{
				result = I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_OUTPUT,~led);	 
				ledcount=!ledcount;
			}
			else
			{
				result = I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_OUTPUT,0xff);
				ledcount=!ledcount;
			}
		}
		}
}


int main(void)
{
	uint8_t count=0;
	
	
	
	//use internal 16M oscillator, HSI
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_16MHZ |SYSCTL_OSC_INT |SYSCTL_USE_OSC), 16000000);		
	
	S800_GPIO_Init();
	S800_I2C0_Init();
	S800_SysTick_init();
	
	S800_UART_Init();
	
	while (1)
	{
		
	result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[a]);						//write port 1 				
	result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0x1<<0);				//1移左移count位	//write port 2
	Delay(20000);
	result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[b]);						//write port 1 				
	result= I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0x1<<1);				//1移左移count位	//write port 2
	Delay(20000);
	result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[c]);						//write port 1 				
	result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0x1<<2);				//1移左移count位	//write port 2
	Delay(20000);
	result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[d]);						//write port 1 				
	result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0x1<<3);				//1移左移count位	//write port 2
	Delay(20000);
		
		if(strcmp(cmd,"A")==0)
			displayA();
		else if (strcmp(cmd,"B")==0)
			displayB();
		else if (strcmp(cmd,"C")==0)
			displayC();
		else if (strcmp(cmd,"D")==0)
			displayD();
		
		
		
		
	
		
	
	}
}



	
void Delay(uint32_t value)
{
	uint32_t ui32Loop;
	for(ui32Loop = 0; ui32Loop < value; ui32Loop++){};
}

void UARTStringPut(const char *cMessage)
{
	while(*cMessage!='\0')
		UARTCharPut(UART0_BASE,*(cMessage++));
}

void 	UARTStringPutNonBlocking(const char *cMessage)
{
	while(*cMessage!='\0')
		UARTCharPutNonBlocking(UART0_BASE,*(cMessage++));
}

void S800_UART_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);						//Enable PortA
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));			//Wait for the GPIO moduleA ready

	GPIOPinConfigure(GPIO_PA0_U0RX);												// Set GPIO A0 and A1 as UART pins.
  GPIOPinConfigure(GPIO_PA1_U0TX);    			

  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	// Configure the UART for 115,200, 8-N-1 operation.
  UARTConfigSetExpClk(UART0_BASE, ui32SysClock,115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
	
	IntEnable(INT_UART0);
	UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);	//Enable UART0 RX,TX interrupt
	
	
	UARTStringPut((uint8_t *)"\r\nHello, world!\r\n");
}


void UART0_Handler(void)
{
	int32_t uart0_int_status;
	uart0_int_status = UARTIntStatus(UART0_BASE,true);//get interrupt status
	
	UARTIntClear(UART0_BASE,uart0_int_status);
	
	while(UARTCharsAvail(UART0_BASE))
	{
		*cmd=UARTCharGetNonBlocking(UART0_BASE);
	}
	

}

void S800_SysTick_init(void)
{
	SysTickPeriodSet(ui32SysClock/SYSTICK_FREQUENCY);//1ms
	
	SysTickEnable();
	SysTickIntEnable();
	IntMasterEnable();
}

void SysTick_Handler(void) //中断服务函数尽量简单，否则可能跑不完
{
	if(systick_100ms_counter == 0)
	{
		systick_100ms_counter = SYSTICK_FREQUENCY/10;
		systick_100ms_counter_status = 1;
	}
	else
		systick_100ms_counter--;
	
	if(systick_10ms_counter == 0)
	{
		systick_10ms_counter = SYSTICK_FREQUENCY/100;
		systick_10ms_counter_status = 1;
	}
	else
		systick_10ms_counter --;
	
}
	

void S800_GPIO_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);						//Enable PortF
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));			//Wait for the GPIO moduleF ready
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);						//Enable PortJ	
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ));			//Wait for the GPIO moduleJ ready	
	
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);			//Set PF0 as Output pin
	GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE,GPIO_PIN_0 | GPIO_PIN_1);//Set the PJ0,PJ1 as input pin
	GPIOPadConfigSet(GPIO_PORTJ_BASE,GPIO_PIN_0 | GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
}


void S800_I2C0_Init(void)
{
	uint8_t result;
  SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);//初始化i2c模块
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//使用I2C模块0，引脚配置为I2C0SCL--PB2、I2C0SDA--PB3
	GPIOPinConfigure(GPIO_PB2_I2C0SCL);//配置PB2为I2C0SCL
  GPIOPinConfigure(GPIO_PB3_I2C0SDA);//配置PB3为I2C0SDA
  GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);//I2C将GPIO_PIN_2用作SCL
  GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);//I2C将GPIO_PIN_3用作SDA

	I2CMasterInitExpClk(I2C0_BASE,ui32SysClock, true);										//config I2C0 400k
	I2CMasterEnable(I2C0_BASE);	

	result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_CONFIG_PORT0,0x0ff);		//config port 0 as input
	result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_CONFIG_PORT1,0x0);			//config port 1 as output
	result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_CONFIG_PORT2,0x0);			//config port 2 as output 

	result = I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_CONFIG,0x00);					//config port as output
	result = I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_OUTPUT,0x0ff);				//turn off the LED1-8
	
}


uint8_t I2C0_WriteByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t WriteData)
{
	uint8_t rop;
	while(I2CMasterBusy(I2C0_BASE)){};//如果I2C0模块忙，等待
		//
	I2CMasterSlaveAddrSet(I2C0_BASE, DevAddr, false);
		//设置主机要放到总线上的从机地址。false表示主机写从机，true表示主机读从机
		
	I2CMasterDataPut(I2C0_BASE, RegAddr);//主机写设备寄存器地址
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);//执行重复写入操作
	while(I2CMasterBusy(I2C0_BASE)){};
		
	rop = (uint8_t)I2CMasterErr(I2C0_BASE);//调试用

	I2CMasterDataPut(I2C0_BASE, WriteData);
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);//执行重复写入操作并结束
	while(I2CMasterBusy(I2C0_BASE)){};

	rop = (uint8_t)I2CMasterErr(I2C0_BASE);//调试用

	return rop;//返回错误类型，无错返回0
}

uint8_t I2C0_ReadByte(uint8_t DevAddr, uint8_t RegAddr)
{
	uint8_t value,rop;
	while(I2CMasterBusy(I2C0_BASE)){};	
	I2CMasterSlaveAddrSet(I2C0_BASE, DevAddr, false);
	I2CMasterDataPut(I2C0_BASE, RegAddr);
//	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);		
	I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_SINGLE_SEND);//执行单词写入操作
	while(I2CMasterBusBusy(I2C0_BASE));
	rop = (uint8_t)I2CMasterErr(I2C0_BASE);
	Delay(1);
	//receive data
	I2CMasterSlaveAddrSet(I2C0_BASE, DevAddr, true);//设置从机地址
	I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_SINGLE_RECEIVE);//执行单次读操作
	while(I2CMasterBusBusy(I2C0_BASE));
	value=I2CMasterDataGet(I2C0_BASE);//获取读取的数据
		Delay(1);
	return value;
}