
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


#define   FASTFLASHTIME			(uint32_t) 300000
#define   SLOWFLASHTIME			(uint32_t) FASTFLASHTIME*20
#define SYSTICK_FREQUENCY					1000			//1000hz
uint32_t ui32SysClock;

void 		Delay(uint32_t value);
void 		S800_GPIO_Init(void);
void		PF0_Flash(uint32_t key_value);
void		PF1_Flash(uint32_t key_value);



uint32_t delay_time,key_value,key_value2;
uint32_t last = 1, state = 0, counter = 0;

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
void PF1_twink(uint32_t key_value2)
{
	if (key_value2==0)
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
	else
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
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
	S800_GPIO_Init();
	delay_time = SLOWFLASHTIME;
	while(1)
  {
		key_value = GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_0)	;				//read the PJ0 key value
		key_value2= GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_1);
	
				
		PF1_twink(key_value);
		PF0_Flash(key_value2);
	
		}
}



void S800_GPIO_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);						//Enable PortF
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));			//Wait for the GPIO moduleF ready
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);						//Enable PortJ	
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ));			//Wait for the GPIO moduleJ ready	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);						//Enable PortN	
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));			//Wait for the GPIO moduleN ready	
	
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1);			//Set PF0 as Output pin
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);			//Set PF1 as Output pin
	GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);			//Set PN01 as Output pin
	GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE,GPIO_PIN_0 | GPIO_PIN_1);//Set the PJ0,PJ1 as input pin
	GPIOPadConfigSet(GPIO_PORTJ_BASE,GPIO_PIN_0 | GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
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
	
	
	
}




