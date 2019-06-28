//EXP BEFORE EXAM 2017-01 T02

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


#define SYSTICK_FREQUENCY		1000			//1000hz

#define	I2C_FLASHTIME				500				//500mS
#define GPIO_FLASHTIME			300				//300mS
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




void 		Delay(uint32_t value);
void 		S800_GPIO_Init(void);
uint8_t 	I2C0_WriteByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t WriteData);
uint8_t 	I2C0_ReadByte(uint8_t DevAddr, uint8_t RegAddr);
void		S800_I2C0_Init(void);
void 		S800_UART_Init(void);
uint8_t cmpstring(const char *in_string, const char *string);
void givstring(char *out_string, const char *string);
//systick software counter define
volatile uint16_t systick_10ms_couter,systick_1000ms_couter,systick_1ms_couter;
volatile uint8_t	systick_10ms_status,systick_1000ms_status,systick_1ms_status;

volatile uint8_t result,cnt1,key_value,gpio_status;
volatile uint8_t rightshift1 = 0x01;
volatile uint8_t cnt2;
volatile uint8_t rightshift2 = 0x01;
uint32_t ui32SysClock,ui32IntPriorityGroup,ui32IntPriorityMask;
uint32_t ui32IntPrioritySystick,ui32IntPriorityUart0;

uint8_t seg7[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x58,0x5e,0x079,0x71,0x5c};
uint8_t uart_receive_char;
uint8_t in_string[100];
uint8_t out_string[100];
uint8_t back_char;
uint8_t temp_char;
uint8_t i,flag;
uint8_t *pointer;
uint8_t classflag=0;
uint8_t codeflag=0;
uint8_t classdisp[] = {15,1,6,0,3,4,0,4};
uint8_t codedisp[] = {5,1,6,0,3,0,9};
uint8_t cntt,a,b,c,jinwei,count=0,mode;
int main(void)
{
	volatile uint16_t	i2c_flash_cnt,gpio_flash_cnt;
	//use internal 16M oscillator, PIOSC
   //ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_16MHZ |SYSCTL_OSC_INT |SYSCTL_USE_OSC), 16000000);		
	//ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_16MHZ |SYSCTL_OSC_INT |SYSCTL_USE_OSC), 8000000);		
	//use external 25M oscillator, MOSC
   //ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |SYSCTL_OSC_MAIN |SYSCTL_USE_OSC), 25000000);		

	//use external 25M oscillator and PLL to 120M
   //ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |SYSCTL_CFG_VCO_480), 120000000);;		
	ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_16MHZ |SYSCTL_OSC_INT | SYSCTL_USE_PLL |SYSCTL_CFG_VCO_480), 20000000);
	
  SysTickPeriodSet(ui32SysClock/SYSTICK_FREQUENCY);
	SysTickEnable();
	SysTickIntEnable();																		//Enable Systick interrupt
	  

	S800_GPIO_Init();
	S800_I2C0_Init();
	S800_UART_Init();
	
	IntEnable(INT_UART0);
  UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);	//Enable UART0 RX,TX interrupt³¬Ê±ÖÐ¶Ï
  IntMasterEnable();		
	ui32IntPriorityMask				= IntPriorityMaskGet();
	IntPriorityGroupingSet(3);														//Set all priority to pre-emtption priority
	
	IntPrioritySet(INT_UART0,3);													//Set INT_UART0 to highest priority
	IntPrioritySet(FAULT_SYSTICK,0x0e0);									//Set INT_SYSTICK to lowest priority
	
	ui32IntPriorityGroup			= IntPriorityGroupingGet();

	ui32IntPriorityUart0			= IntPriorityGet(INT_UART0);
	ui32IntPrioritySystick		= IntPriorityGet(FAULT_SYSTICK);
	while (1)
	{
		
		if(systick_1000ms_status)
		{
			systick_1000ms_status=0;
			++c;
		}
		
		if(c>=60)
		{
			jinwei = c/60;
			c = c%60;
			b += jinwei;
		}
		if(b>=60)
		{
			jinwei = b/60;
			b = b%60;
			a += jinwei;
		}
		if(a>=60)
			a = a%60;
		
		if(systick_1ms_status)
		{
			systick_1ms_status=0;
			
			
			//??????
			switch(cntt)
			{
				case 0:
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0X0);
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[a/10]);	//write port 1 				
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,1);	//write port 2
				break;
			
				case 1:
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0X0);
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[a%10]);	//write port 1 				
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,1<<1);	//write port 2
				break;
				
				case 2:
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0X0);
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,0X40);	//write port 1 '-'				
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,1<<2);	//write port 2
				break;
				
				case 3:
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0X0);
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[b/10]);	//write port 1 				
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,1<<3);	//write port 2
				break;
				
				case 4:
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0X0);
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[b%10]);	//write port 1 				
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,1<<4);	//write port 2
				break;
				
				case 5:
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0X0);
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,0X40);	//write port 1 				
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,1<<5);	//write port 2
				break;
				
				case 6:
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0X0);
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[c/10]);	//write port 1 				
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,1<<6);	//write port 2
				break;
						
				case 7:
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0X0);
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[c%10]);	//write port 1 				
			result = I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,1<<7);	//write port 2
				break;
			}
			
			if (++cntt >= 0x8)
				cntt = 0;

		}
		}
		
		
		if (systick_10ms_status)
		{
			systick_10ms_status		= 0;
			if (++gpio_flash_cnt	>= GPIO_FLASHTIME/10)
			{
				gpio_flash_cnt			= 0;
				if (gpio_status)
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,GPIO_PIN_0 );
				else
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0);
				gpio_status					= !gpio_status;
			
			}
		}
		
	



}

void Delay(uint32_t value)
{
	uint32_t ui32Loop;
	for(ui32Loop = 0; ui32Loop < value; ui32Loop++){};
}


void UARTStringPut(uint8_t *cMessage)
{
	while(*cMessage!='\0')
		UARTCharPut(UART0_BASE,*(cMessage++));
}
void UARTStringPutNonBlocking(const char *cMessage)
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

	// Configure the UART for bode rate 115,200, and data check 8-N-1 operation.
  UARTConfigSetExpClk(UART0_BASE, ui32SysClock,115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
	UARTStringPut((uint8_t *)"\r\nHello, world!\r\n");
}
void S800_GPIO_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);						//Enable PortF
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));			//Wait for the GPIO moduleF ready
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);						//Enable PortJ	
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ));			//Wait for the GPIO moduleJ ready	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);						//Enable PortN	
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));			//Wait for the GPIO moduleN ready		
	
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);			//Set PF0 as Output pin
  GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);			//Set PN0 as Output pin
  GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);			//Set PN1 as Output pin	

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
	while(I2CMasterBusy(I2C0_BASE)){};
	I2CMasterSlaveAddrSet(I2C0_BASE, DevAddr, false);
	I2CMasterDataPut(I2C0_BASE, RegAddr);
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
	while(I2CMasterBusy(I2C0_BASE)){};
	rop = (uint8_t)I2CMasterErr(I2C0_BASE);

	I2CMasterDataPut(I2C0_BASE, WriteData);
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
	while(I2CMasterBusy(I2C0_BASE)){};

	rop = (uint8_t)I2CMasterErr(I2C0_BASE);
	return rop;
}

uint8_t I2C0_ReadByte(uint8_t DevAddr, uint8_t RegAddr)
{
	uint8_t value,rop;
	while(I2CMasterBusy(I2C0_BASE)){};	
	I2CMasterSlaveAddrSet(I2C0_BASE, DevAddr, false);
	I2CMasterDataPut(I2C0_BASE, RegAddr);
//	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);		
	I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_SINGLE_SEND);
	while(I2CMasterBusBusy(I2C0_BASE));
	rop = (uint8_t)I2CMasterErr(I2C0_BASE);
	Delay(1);
	//receive data
	I2CMasterSlaveAddrSet(I2C0_BASE, DevAddr, true);
	I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_SINGLE_RECEIVE);
	while(I2CMasterBusBusy(I2C0_BASE));
	value=I2CMasterDataGet(I2C0_BASE);
		Delay(1);
	return value;
}

/*
	Corresponding to the startup_TM4C129.s vector table systick interrupt program name
*/
void SysTick_Handler(void)
{
	if (systick_1000ms_couter	!= 0)
		systick_1000ms_couter--;
	else
	{
		systick_1000ms_couter	= SYSTICK_FREQUENCY;
		systick_1000ms_status 	= 1;
	}
	
	if (systick_10ms_couter	!= 0)
		systick_10ms_couter--;
	else
	{
		systick_10ms_couter		= SYSTICK_FREQUENCY/100;
		systick_10ms_status 	= 1;
	}
	
	if (systick_1ms_couter	!= 0)
		systick_1ms_couter--;
	else
	{
		systick_1ms_couter		= SYSTICK_FREQUENCY/1000;
		systick_1ms_status 	= 1;
	}

}

/*
	Corresponding to the startup_TM4C129.s vector table UART0_Handler interrupt program name
*/
void UART0_Handler(void)
{
	int32_t uart0_int_status;
  uart0_int_status 		= UARTIntStatus(UART0_BASE, true);		// Get the interrrupt status.

  UARTIntClear(UART0_BASE, uart0_int_status);								//Clear the asserted interrupts
	
	count++;
	givstring(out_string,"");
	givstring(in_string,"");
	i = 0;
  while(UARTCharsAvail(UART0_BASE))    											// Loop while there are characters in the receive FIFO.
  {
		temp_char = UARTCharGetNonBlocking(UART0_BASE);
		if(temp_char>='a' && temp_char<='z')
			temp_char = temp_char -'a'+'A';//transform to captilization
		///Read the next character from the UART and write it back to the UART.
		//if(temp_char!='\0')
		//{
			in_string[i] = temp_char;
			i=i+1;
		//}
	}
	
	if(count==1){
	if(strncmp(in_string,"SET",3)==0)	{
		a=(in_string[3]-'0')*10+in_string[4]-'0';
		b=(in_string[6]-'0')*10+in_string[7]-'0';
		mode=1;
		//givstring(out_string,in_string);
	}	
	if(strncmp(in_string,"INC",3)==0)
	{
		a+=(in_string[3]-'0')*10+in_string[4]-'0';
		b+=(in_string[6]-'0')*10+in_string[7]-'0';
		mode=2;
	}
	if(strncmp(in_string,"GETTIME",7)==0)
		count++;
	}
	if(count==2&&mode==1)
		{
			c=(in_string[1]-'0')*10+in_string[2]-'0';
			mode=0;
		}
	if(count==2&&mode==2)
	{
		c+=(in_string[1]-'0')*10+in_string[2]-'0';
		mode=0;
	}
	
	givstring(out_string,"TIME");
	out_string[4]=(char)('0'+a/10);
	out_string[5]=(char)('0'+a%10);
	out_string[6]=(char)('0'+b/10);
	out_string[7]=(char)('0'+b%10);
	out_string[8]=(char)('0'+c/10);
	out_string[9]=(char)('0'+c%10);
	
	/*
	if (cmpstring(in_string,"AT+CLASS") == 1)
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,GPIO_PIN_2 );
	else
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,0);
	
	if (cmpstring(in_string,"AT+STUDENTCODE") == 1)
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3,GPIO_PIN_3 );
	else
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3,0);
	*/		
	
	if(count==2){
	pointer = out_string;
				
	while(*pointer!='\0')
	{
		back_char = *pointer;
    UARTCharPutNonBlocking(UART0_BASE,back_char);
		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1,GPIO_PIN_1 );		
		pointer++;
		//Delay(1000);//xuanxue de shiqing ?bing mei you shen me yong a! 
	}
	count=0;
}
	
	GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1,0 );	
	
	
	while (GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_1) == 0)
		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1,GPIO_PIN_1);	

	GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1,0);
}

uint8_t cmpstring(const char *in_string, const char *string)//compare input and previous setting
{
	flag = 1;
	while(true)
	{	
		if(*in_string == *string)
		{
			in_string++;
			string++;
		}
		else
		{
			flag = 0;
			break;
		}
		if(*in_string=='\0' || *string=='\0')
			break;
	}
	//if(*in_string!='\0' || *string!='\0')
	//	flag = 0;
	return flag;
}

void givstring(char *out_string, const char *string)
{
	while(true)
	{
		*out_string = *string;
		if(*out_string == '\0')
			break;
		out_string++;
		string++;
	}
	//*out_string = '\0';
}
