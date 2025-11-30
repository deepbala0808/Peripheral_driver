#include "Peripheral.h"
#include <stdint.h>
#define RCC_AHB1ENR *(volatile int*) 0x40023830
#define RCC_APB2ENR *(volatile int*) 0x40023844
#define GPIOA_MODE *(volatile int*) 0x40020000
#define GPIOA_AFRL *(volatile int*) 0x40020020
#define GPIOA_AFRH *(volatile int*) 0x40020024
#define USART1_BRR *(volatile int*) 0x40011008
#define USART1_CR *(volatile int*) 0x4001100C
#define USART1_SR *(volatile int*) 0x40011000
#define USART1_DR *(volatile int*) 0x40011004
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

 static void usart_data(char data)
 {
        while(!(USART1_SR&(1<<7)));//If you read 1 you go ahead, if you read 0 you loop continuously

	   USART1_DR = data;

 }

 void Serial_print(const char* input)
 {
	 const char* i= input;
	 while(*i!=0)
	 {
		 usart_data(*i);
		 i++;
	 }
 }
 void Serial_println(const char* input)
 {
	 const char* i= input;
	 while(*i!=0)
	 {
		 usart_data(*i);
		 i++;
	 }
	 usart_data('\r');
	 usart_data('\n');
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
	 if(16*baudrate>16000000)
	 {
		 USART1_CR|=(1<<15);
		 if(8*baudrate>16000000)
		 {
			 baudrate=2000000;

		 }
			 usart_div=16000000/(8*baudrate);
			 rem=16000000%(8*baudrate);
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
		 usart_div=16000000/(16*baudrate);
		 			 rem=16000000%(16*baudrate);
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
