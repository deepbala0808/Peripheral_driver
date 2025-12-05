#include "Peripheral.h"
#include <stdint.h>
#define PCLK 16000000
//Register definitions for USART1
#define RCC_AHB1ENR *(volatile int*) 0x40023830
#define RCC_APB2ENR *(volatile int*) 0x40023844
#define RCC_APB1ENR *(volatile int*) 0x40023840
#define GPIOA_MODE *(volatile int*) 0x40020000
#define GPIOA_AFRL *(volatile int*) 0x40020020
#define GPIOA_AFRH *(volatile int*) 0x40020024
#define USART1_BRR *(volatile int*) 0x40011008
#define USART1_CR *(volatile int*) 0x4001100C
#define USART1_SR *(volatile int*) 0x40011000
#define USART1_DR *(volatile int*) 0x40011004
//Register Definitions for USART2
#define GPIOD_MODE *(volatile int*) 0x40030C00
#define GPIOD_AFRL *(volatile int*) 0x40020C20
#define GPIOD_AFRH *(volatile int*) 0x40020C24
#define USART2_BRR *(volatile int*) 0x40004408
#define USART2_CR *(volatile int*) 0x4000440C
#define USART2_SR *(volatile int*) 0x40004400
#define USART2_DR *(volatile int*) 0x40004404
//Register Definitions for USART6
#define GPIOC_MODE *(volatile int*) 0x40020800
#define GPIOC_AFRL *(volatile int*) 0x40020820
#define GPIOC_AFRH *(volatile int*) 0x40020824
#define USART6_BRR *(volatile int*) 0x40011408
#define USART6_CR *(volatile int*) 0x4001140C
#define USART6_SR *(volatile int*) 0x40011400
#define USART6_DR *(volatile int*) 0x40011404
//Function definitions for USART1
void USART1_init(void)
{
	RCC_AHB1ENR |= 1;
		RCC_APB2ENR |= (1<<4);
		GPIOA_MODE&=~((0x3<<18)|(0x3<<20));
		GPIOA_MODE|=(0x2<<18);
		GPIOA_MODE|=(0x2<<20);
		GPIOA_AFRH&=~(0xFF<<4);//Setting only for PA9 and PA10
		GPIOA_AFRH|=(0x07<<4);
		GPIOA_AFRH|=(0x07<<8);
		USART1_CR&=~(1<<13); //Reset UE bit
		USART1_CR|=(1<<13); //Set UE bit
		USART1_CR&=~(1<<3); //Reset TE bit
		USART1_CR|=(1<<3); //Set TE  bit
		USART1_CR&=~(1<<2);//Reset RE bit
		USART1_CR|=(1<<2);//Set RE bit
		//usart_flag=1;
		//return usart_flag;
	}
 static void usart_data_put(char data)
 {
        while(!(USART1_SR&(1<<7)));//If you read 1 you go ahead, if you read 0 you loop continuously

	   USART1_DR = data;

 }

 void Serial_print(const char* input)
 {
	 const char* i= input;
	 while(*i!=0)
	 {
		 usart_data_put(*i);
		 i++;
	 }
 }
 void Serial_println(const char* input)
 {
	 const char* i= input;
	 while(*i!=0)
	 {
		 usart_data_put(*i);
		 i++;
	 }
	 usart_data_put('\r');
	 usart_data_put('\n');
 }
static char usart_receive_data(void)
{
	while(!(USART1_SR&(1<<5)));
    return USART1_DR;
}
char* Serial_read(void)
{
	char byte;
	static char array[MAX_RX_BUFFER_SIZE];
	uint8_t i=0;
    while(1)
    {
    	byte=usart_receive_data();
    	if(byte=='\n'||byte=='\r')
    	{
    		array[i]='\0';
    		break;
    	}
    	if(i<(MAX_RX_BUFFER_SIZE-1))
    	{
    		array[i]=byte;
    		i++;
    	}
    }
	return array;
	}
 void Serial_begin(int baudrate)
{
	 uint32_t mantissa;
	 uint32_t DIV;
	 uint32_t fraction;
	 uint32_t usart_div;
	 uint32_t rem;
	 if(16*baudrate>PCLK)
	 {
		 USART1_CR|=(1<<15);
		 if(8*baudrate>PCLK)
		 {
			 baudrate=2000000;

		 }
			 usart_div=PCLK/(8*baudrate);
			 rem=PCLK%(8*baudrate);
			 				 mantissa = (uint32_t)usart_div;
			 				 fraction = ((rem*8+(8*baudrate)/2)/(8*baudrate));
			 				 DIV = fraction;
			 				 if(DIV>=8)
			 				 {
			 					 DIV=0;
			 					 mantissa++;
			 				 }
	 }
	 else
	 {
		 USART1_CR&=~(1<<15);
		 usart_div=PCLK/(16*baudrate);
		 			 rem=PCLK%(16*baudrate);
		 			 				 mantissa = (uint32_t)usart_div;
		 			 				 fraction = ((rem*16+(16*baudrate)/2)/(16*baudrate));
		 			 				 DIV = fraction;
		 			 				 if(DIV>=16)
		 			 				 {
		 			 					 DIV=0;
		 			 					 mantissa++;
		 			 				 }
	 }
	 USART1_BRR&=~(0xFFFF);
	 USART1_BRR|=(mantissa<<4);
	 USART1_BRR|=DIV;
	 }
 //Function definitions for USART2
 void USART2_init(void)
 {
	 RCC_AHB1ENR|=1;
	 RCC_APB1ENR|=(1<<17);
	 GPIOA_MODE&=~(0xF<<4);
	 GPIOA_MODE|=(10<<4);//PA2-TX PA3-RX
	 GPIOA_AFRL&=~(0xFF<<8);
	 GPIOA_AFRL|=(0x77<<8);
	 USART2_CR|=(1<<13);
	 USART2_CR|=12;
 }
void Serial2_begin(int baudrate)
{
 uint32_t MANTISSA;
 uint32_t DIV;
 uint32_t usart_div;
 uint32_t rem;
 uint32_t fraction;
 if(16*baudrate>PCLK)
 {
   if(8*baudrate>PCLK)
    {
    baudrate=2000000;
    }
   USART2_CR|=(1<<15);
   usart_div=PCLK/(8*baudrate);
   rem=PCLK%(8*baudrate);
   MANTISSA=(uint32_t)usart_div;
   fraction=(((rem*8)+((8*baudrate)/2))/(8*baudrate));
   DIV= fraction;
   if(DIV>=8)
   {
	   DIV=0;
	   MANTISSA++;
   }
 }
 else
 {
	 USART2_CR&=~(1<<15);
	 usart_div=PCLK/(16*baudrate);
	 MANTISSA=(uint32_t)usart_div;
	 rem=PCLK%(16*baudrate);
	 fraction=(((rem*16)+((16*baudrate)/2))/(16*baudrate));
	 DIV=fraction;
	 if(DIV>=16)
	 {
		 DIV=0;
		 MANTISSA++;
	 }
	 }
 USART2_BRR&=~(0xFFFF);
 USART2_BRR|=(MANTISSA<<4);
 USART2_BRR|=DIV;
}
void usart2_data_put(char data)
{
 while(!(USART2_SR&(1<<7)));
 USART2_DR=data;
}
void Serial2_print(const char* input)
{
	const char* i=input;
	while(*i!=0)
	{
		usart2_data_put(*i);
		i++;
	}
}
void Serial2_println(const char* input)
{
 const char* i=input;
 while(*i!=0)
 {
	 usart2_data_put(*i);
	 i++;
 }
 usart2_data_put('\r');
 usart2_data_put('\n');
}
static char usart2_receive_data(void)
{
while(!(USART2_SR&(1<<5)));
return USART2_DR;
}
char* Serial2_read(void)
{
	uint8_t i=0;
	static char buffer[MAX_RX_BUFFER_SIZE];
	char byte;
	while(1)
	{
		byte=usart2_receive_data();
		if(byte=='\n'||byte=='\r')
		{
			buffer[i]='\0';
			break;
		}
		if(i<MAX_RX_BUFFER_SIZE)
		{
			buffer[i]=byte;
			i++;
		}
	}
	return buffer;
}

// Functions for USART6
void USART6_init()
{
	RCC_AHB1ENR&=~(1<<2);
	RCC_AHB1ENR|=(1<<2);
	RCC_APB2ENR&=~(1<<5);
	RCC_APB2ENR |=(1<<5);
	GPIOC_MODE &=~(10<<12);
	GPIOC_MODE |=(10<<12);//PC6-TX PC7-RX
	GPIOC_AFRL&=~(0xFF<<24);
	GPIOC_AFRL|=(0x88<<24);
	USART6_CR&=~(1<<13);
	USART6_CR|=(1<<13);
	USART6_CR&=~(0xC);
	USART6_CR|=0xC;
}
void Serial6_begin(int baudrate)
{
  uint32_t usart_div, mantissa, div, fraction, rem;
  if(16*baudrate>PCLK)
  {
	  USART6_CR|=(1<<15);
	  if(8*baudrate>PCLK)
	  {
		  baudrate=2000000;
	  }
	  usart_div=PCLK/(8*baudrate);
	  mantissa=(uint32_t)usart_div;
	  rem=PCLK%(8*baudrate);
	  fraction=((rem*8)+((8*baudrate)/2))/(8*baudrate);
	  div=(uint32_t)fraction;
	  if(div>=8)
	  {
		  div=0;
		  mantissa++;
	  }
  }
  else
  {
	  USART6_CR&=~(1<<15);
	  usart_div=PCLK/(16*baudrate);
	  	  mantissa=(uint32_t)usart_div;
	  	  rem=PCLK%(16*baudrate);
	  	  fraction=((rem*16)+((16*baudrate)/2))/(16*baudrate);
	  	  div=(uint32_t)fraction;
	  	  if(div>=16)
	  	  {
	  		  div=0;
	  		  mantissa++;
	  	  }
  }
  USART6_BRR&=~(0xFFFF);
  USART6_BRR|=(mantissa<<4);
  USART6_BRR|=div;

}
void usart6_put_data(char data)
{
 while(!(USART6_SR&(1<<7)));
 USART6_DR=data;
}
void Serial6_print(const char* input)
{
  while(*input!=0)
  {
	  usart6_put_data(*input);
	  input++;
  }
}
void Serial6_println(const char* input)
{
	while(*input!=0)
	{
		usart6_put_data(*input);
		input++;
	}
	usart6_put_data('\n');
	usart6_put_data('\r');
}
char usart6_receive_data(void)
{
while(!(USART6_SR&(1<<5)));
 return USART6_DR;
}

char* Serial6_read(void)
{
	static char array[MAX_RX_BUFFER_SIZE];
	uint32_t i=0;
	char data;
while(1)
{
	data=usart6_receive_data();

	if(data=='\n'||data=='\r')
	{
		array[i]='\0';
		break;
	}
	if(i<MAX_RX_BUFFER_SIZE)
	{
		array[i]=data;
		i++;
	}

}
return array;
}

