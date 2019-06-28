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
# include <stdio.h>
# include <stdlib.h>

#define   FASTFLASHTIME			(uint32_t) 300000
#define   SLOWFLASHTIME			(uint32_t) FASTFLASHTIME*20


void 		Delay(uint32_t value);
void 		S800_GPIO_Init(void);
void		PF0_Flash(uint32_t key_value);
void		PF1_Flash(uint32_t key_value);
void PORTJ_IntHandler(void);

uint32_t ui32SysClock;
uint32_t delay_time,key_value;
uint32_t last = 0, state = 0, counter = 0;

void 		Delay(uint32_t value);
void 		S800_GPIO_Init(void);

void PF1_Flash(uint32_t key_value)
{
	uint32_t delay_time;
		if (key_value	== 0)						//USR_SW1-PJ0 pressed
			delay_time							= FASTFLASHTIME;
		else													//USR_SW1-PJ0 released
			delay_time							= SLOWFLASHTIME;
		
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);			// Turn on the LED.
		Delay(delay_time);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);							// Turn off the LED.
		Delay(delay_time);
}

void PF0_Flash(uint32_t key_value)
{
	uint32_t delay_time;
		if (key_value	== 0)						//USR_SW1-PJ0 pressed
			delay_time							= FASTFLASHTIME;
		else													//USR_SW1-PJ0 released
			delay_time							= SLOWFLASHTIME;
		
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);			// Turn on the LED.
		Delay(delay_time);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x0);							// Turn off the LED.
		Delay(delay_time);
}

void display(uint32_t state)
{
	//*counter = (*counter + 1) % SLOWFLASHTIME;
	switch (state)
	{
		case 0: PF0_Flash(0);
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
						break;
		case 1: GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
						break;
		case 2: GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
						PF1_Flash(0);
						break;
		case 3: GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
						break;
	}
}

void Delay(uint32_t value)
{
	uint32_t ui32Loop;
	for(ui32Loop = 0; ui32Loop < value; ui32Loop++){};
}

int main(void)
{
	ui32SysClock=SysCtlClockFreqSet((SYSCTL_OSC_INT | SYSCTL_USE_OSC),16000000);
	
	
	IntMasterEnable();
	S800_GPIO_Init();
	delay_time = SLOWFLASHTIME;
	while(1)
  {
		/*key_value = GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_0)	;				//read the PJ0 key value

		if (key_value == 0 && last == 1)
		{
			state = (state + 1) % 4;
			//counter = 0;
		}*/
		display(state);
		
		//last = key_value;
  }
}



void S800_GPIO_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);						//Enable PortF
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));			//Wait for the GPIO moduleF ready
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);						//Enable PortJ	
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ));			//Wait for the GPIO moduleJ ready	
	
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1);			//Set PF0 as Output pin
	GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE,GPIO_PIN_0 | GPIO_PIN_1);//Set the PJ0,PJ1 as input pin
	GPIOPadConfigSet(GPIO_PORTJ_BASE,GPIO_PIN_0 | GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	
	GPIOIntRegister(GPIO_PORTJ_BASE,PORTJ_IntHandler);
	GPIOIntTypeSet(GPIO_PORTJ_BASE,GPIO_PIN_0,GPIO_FALLING_EDGE);
	GPIOIntEnable(GPIO_PORTJ_BASE,GPIO_PIN_0);
	
}

void PORTJ_IntHandler(void)
{
	uint32_t ulstatus;
	ulstatus=GPIOIntStatus(GPIO_PORTJ_BASE,true);
	GPIOIntClear(GPIO_PORTJ_BASE,ulstatus);
	//if(ulstatus)
	state=(state+1)%4;
	
}
	







/*
uint32_t key_value,delay_time,last_key_value=0,state=0;

void PF1_Flash(uint32_t key_value)
{
	uint32_t delay_time;
		if (key_value	== 0)						//USR_SW1-PJ0 pressed
			delay_time							= FASTFLASHTIME;
		else													//USR_SW1-PJ0 released
			delay_time							= SLOWFLASHTIME;
		
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);			// Turn on the LED.
		Delay(delay_time);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);							// Turn off the LED.
		Delay(delay_time);
}

void display(uint32_t state)
{
	switch(state)
	{
		case 0:
			PF0_Flash(0);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
			break;
		case 1:
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0);
			break;
		case 2:
			PF1_Flash(0);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
			break;
		case 3:
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
			break;
	}
}
			

int main(void)
{
	uint32_t ui32Loop; 
//Enable PortF 
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //Set PF0 as Output pin 
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
	
	
	S800_GPIO_Init();
	while(1)
	{
		key_value=GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0);
		if(key_value==0&&last_key_value==1)
		{
			state=(state+1)%4;
			
		}
		display(state);
		last_key_value=key_value;
			
		
		// Turn on the LED. 
//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0); // Delay 
//for(ui32Loop = 0; ui32Loop < 800000; ui32Loop++){}; 
// Turn off the LED. 
//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x0); 
// Delay 
//for(ui32Loop = 0; ui32Loop < 800000; ui32Loop++){}; 

	}
}

void PF0_Flash(uint32_t key_value)
{
	uint32_t delay_time;
		if (key_value	== 0)						//USR_SW1-PJ0 pressed
			delay_time							= FASTFLASHTIME;
		else													//USR_SW1-PJ0 released
			delay_time							= SLOWFLASHTIME;
		
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);			// Turn on the LED.
		Delay(delay_time);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x0);							// Turn off the LED.
		Delay(delay_time);
}




void Delay(uint32_t value)
{
	uint32_t ui32Loop;
	for(ui32Loop = 0; ui32Loop < value; ui32Loop++){};
}


void S800_GPIO_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);						//Enable PortF
	//����ԭ�ͣ�void SysCtlPeripheralEnable(uint32_t ui32Peripheral)
	
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));			//Wait for the GPIO moduleF ready
	//����ԭ�ͣ�bool SysCtlPeripheralReady(uint32_t ui32Peripheral)
	//���ָ�������豻ʹ�ܳɹ�������true�����򷵻�false
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);						//Enable PortJ	
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ));			//Wait for the GPIO moduleJ ready	
	
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);			//Set PF0 as Output pin
	//����ԭ�ͣ�void GPIOPinTypeGPIOOutput(uint32_t ui32Port, uint8_t ui8Pins)
	//����GPIO�˿�����Ϊ������ţ�����ַ��ͣ�uint8_t������ui8PinsĳλΪ1����GPIO�˿ڶ�Ӧλ����Ϊ�������
	
	GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE,GPIO_PIN_0 | GPIO_PIN_1);//Set the PJ0,PJ1 as input pin
	//����GPIO�˿�����Ϊ�������ţ���GPIOPinTypeGPIOOutput()���ơ�GPIO_PIN_0 | GPIO_PIN_1 = 00000011b
	
	GPIOPadConfigSet(GPIO_PORTJ_BASE,GPIO_PIN_0 | GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	//����ԭ�ͣ�void GPIOPadConfigSet(uint32_t ui32Port, uint8_t ui8Pins, uint32_t ui32Strength, uint32_t ui32PinType)
	//GPIO�˿����á�uint32_t ui32Port��GPIO�˿ڻ���ַ
	//ui8Pins���˿�����λ��ϱ�ʾ����10000001b��ʾ���ö˿ڵ�D7��D0λ
	//ui32Strength���˿ڵ��������������������������Ч����ѡ�����GPIO_STRENGTH_2MA/4MA/8MA/8MA_SC/6MA/10MA/12MA
	//ui32PinType���������ͣ���ѡ�����GPIO_PIN_TYPE_STD�����죩��GPIO_PIN_TYPE_STD_WPU��������������GPIO_PIN_TYPE_STD_WPD��������������
	//GPIO_PIN_TYPE_OD����©����GPIO_PIN_TYPE_ANALOG��ģ�⣩��GPIO_PIN_TYPE_WAKE_HIGH���ߵ�ƽ�Ӷ��߻��ѣ���GPIO_PIN_TYPE_WAKE_LOW���ͣ�
}

*/
/*
uint32_t delay_time,key_value;
uint32_t last = 0, state = 0, counter = 0;

void 		Delay(uint32_t value);
void 		S800_GPIO_Init(void);

void display(uint32_t state, uint32_t* counter)
{
	*counter = (*counter + 1) % SLOWFLASHTIME;
	switch (state)
	{
		case 0: GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
						break;
		case 1: GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, (*counter > FASTFLASHTIME));
						break;
		case 2: GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
						break;
		case 3: GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, (*counter > FASTFLASHTIME)<<1);
						break;
	}
}


int main(void)
{
	S800_GPIO_Init();
	delay_time = SLOWFLASHTIME;
	while(1)
  {
		key_value = GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_0)	;				//read the PJ0 key value

		if (key_value == 0 && last == 1)
		{
			state = (state + 1) % 4;
			counter = 0;
		}
		display(state, &counter);
		
		last = key_value;
  }
}


void S800_GPIO_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);						//Enable PortF
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));			//Wait for the GPIO moduleF ready
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);						//Enable PortJ	
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ));			//Wait for the GPIO moduleJ ready	
	
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1);			//Set PF0 as Output pin
	GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE,GPIO_PIN_0 | GPIO_PIN_1);//Set the PJ0,PJ1 as input pin
	GPIOPadConfigSet(GPIO_PORTJ_BASE,GPIO_PIN_0 | GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
}
*/