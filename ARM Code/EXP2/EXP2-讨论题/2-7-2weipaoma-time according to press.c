
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
#include "SysTick.h"
#include "interrupt.h"
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
void 		S800_Clock_Init(void);
void 		S800_GPIO_Init(void);
void		S800_I2C0_Init(void);
uint8_t I2C0_WriteByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t WriteData);
uint8_t I2C0_ReadByte(uint8_t DevAddr, uint8_t RegAddr);
void 		S800_SysTick_Init(void);

uint32_t ui32SysClock;
uint8_t seg7[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x58,0x5e,0x079,0x71,0x5c};
uint8_t key,last,state,first,cnt_time,cnt1;
//systick software counter define
volatile uint16_t systick_100ms_couter,systick_1000ms_couter,systick_1ms_counter; //10ms?100ms???
volatile uint8_t	systick_100ms_status,systick_1000ms_status,systick_1ms_status; //10ms?100ms????

int main(void)
{
	volatile uint8_t result;
	volatile uint8_t cnt=0; 
	
	S800_Clock_Init();
	S800_GPIO_Init();
	S800_I2C0_Init();
	S800_SysTick_Init();

  IntMasterEnable();	//???????
	
	while (1)
	{
		key = GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_0);
		if(key==0&&last==1)
		{
			state=(state+1)%4;
		}
		last=key;
		if(state!=3) first=1;
		
	if(systick_1ms_status==1)
		{
			systick_1ms_status=0;
			
		if(cnt<=6)
		{
			switch(cnt1)
			{
				case 0:
				
		result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0X0);
		result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[cnt+1]);	//write port 1 				
		result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,1<<cnt);	//write port 2
		break;
		//while(systick_20ms_status!=1);
				case 1:
		result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0x0);
		result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[cnt+2]);	//write port 1 				
		result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,1<<(cnt+1));	//write port 2
		break;
			//while(systick_20ms_status!=1);
			}
			
		result = I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_OUTPUT,~((1<<cnt)+(1<<(cnt+1))));	
			if(++cnt1>=2)
				cnt1=0;
		}
		else
		{
			switch (cnt1)
			{
				case 0:
		result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0X0);
		result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[cnt+1]);	//write port 1 				
		result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,1<<cnt);	//write port 2
		break;
			//while(systick_20ms_status!=1);
				case 1:
		result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0X0);
		result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[1]);	//write port 1 				
		result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,1<<0);	//write port 2
		break;
			//while(systick_20ms_status!=1);
			}
		result = I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_OUTPUT,~((1<<cnt)+(1<<0)));	
			if(++cnt1>=2)
				cnt1=0;
		}
		}
		
		//if (GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_0)) //????,???
		if(systick_1000ms_status==1)
		{
		  systick_1000ms_status=0;
		}
		if(systick_100ms_status==1)
		{
			systick_100ms_status=0;
			if(state==0){
				if(++cnt_time >= 10)  //1s
				{
					cnt_time=0;
					cnt++;
					if (cnt >= 0x8)
					cnt = 0;
				}
			}
			else if(state==1)
			{
				if(++cnt_time>=20) //2s
				{
					cnt_time=0;
					cnt++;
					if (cnt >= 0x8)
					cnt = 0;
				}
			}
			else if (state==2)
			{
				if(++cnt_time>=2) //200ms
				{
					cnt_time=0;
					cnt++;
					if (cnt >= 0x8)
					cnt = 0;
				}
			}
			else if(state==3)
			{
				if(++cnt_time>=5) //0.5s
				{
					cnt_time=0;
					cnt++;
					if(first) //第一次进状态3要回初始状态
						{
							cnt=0;
							first=0;
						}
						
					if (cnt >= 0x8)
					cnt = 0;
				}
			}
					
		}
		
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0));		// Turn over the PF0 
		//SysCtlDelay(ui32SysClock/6); //??0.5s

	
}
}

void S800_Clock_Init(void)
{
	//use internal 16M oscillator, HSI
	//ui32SysClock = SysCtlClockFreqSet((SYSCTL_OSC_INT |SYSCTL_USE_OSC), 1000000);		

	//use extern 25M crystal
	//ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |SYSCTL_OSC_MAIN |SYSCTL_USE_OSC), 25000000);		

	//use PLL
	//ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |SYSCTL_OSC_MAIN |SYSCTL_USE_PLL |SYSCTL_CFG_VCO_480), 60000000);	
	ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_16MHZ |SYSCTL_OSC_INT |SYSCTL_USE_PLL |SYSCTL_CFG_VCO_480), 20000000);	
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
	
  SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	GPIOPinConfigure(GPIO_PB2_I2C0SCL);
  GPIOPinConfigure(GPIO_PB3_I2C0SDA);
  GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
  GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

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
	
	while(I2CMasterBusy(I2C0_BASE)){}; //???
	I2CMasterSlaveAddrSet(I2C0_BASE, DevAddr, false); //?????,?
	I2CMasterDataPut(I2C0_BASE, RegAddr);	//?????
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START); //??????
	while(I2CMasterBusy(I2C0_BASE)){};
	rop = (uint8_t)I2CMasterErr(I2C0_BASE);

	I2CMasterDataPut(I2C0_BASE, WriteData); //????
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH); //??????,?????
	while(I2CMasterBusy(I2C0_BASE)){};

	rop = (uint8_t)I2CMasterErr(I2C0_BASE);
	return rop;
}

uint8_t I2C0_ReadByte(uint8_t DevAddr, uint8_t RegAddr)
{
	uint8_t value,rop;
	while(I2CMasterBusy(I2C0_BASE)){};	//???
	I2CMasterSlaveAddrSet(I2C0_BASE, DevAddr, false); //?????,?
	I2CMasterDataPut(I2C0_BASE, RegAddr); //?????
	I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_SINGLE_SEND); //??????
	while(I2CMasterBusBusy(I2C0_BASE));
	rop = (uint8_t)I2CMasterErr(I2C0_BASE);
	Delay(1);
	//receive data
	I2CMasterSlaveAddrSet(I2C0_BASE, DevAddr, true); //?????,?
	I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_SINGLE_RECEIVE); //??????
	while(I2CMasterBusBusy(I2C0_BASE));
	value=I2CMasterDataGet(I2C0_BASE);

	return value;
}

void S800_SysTick_Init(void)
{
	SysTickPeriodSet(ui32SysClock/SYSTICK_FREQUENCY); //??1ms
	SysTickEnable();
	SysTickIntEnable();
}

/*
	Corresponding to the startup_TM4C129.s vector table systick interrupt program name
*/
void SysTick_Handler(void)
{
	if (systick_1000ms_couter == 0)
	{
		systick_1000ms_couter = SYSTICK_FREQUENCY;
		systick_1000ms_status = 1;
	}
	else
		systick_1000ms_couter--;
	
	if (systick_100ms_couter	== 0)
	{
		systick_100ms_couter	 = SYSTICK_FREQUENCY/10;
		systick_100ms_status  = 1;
	}
	else
		systick_100ms_couter--;			
	
	
	if (systick_1ms_counter	!= 0)
		systick_1ms_counter--;
	else
	{
		systick_1ms_counter		= SYSTICK_FREQUENCY/1000;
		systick_1ms_status 	= 1;
	}
	
	
}

void Delay(uint32_t value)
{
	uint32_t ui32Loop;
	for(ui32Loop = 0; ui32Loop < value; ui32Loop++){};
}



