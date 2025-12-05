#ifndef PERIPHERAL_H_
#define PERIPHERAL_H_
#include <stdint.h>
#define MAX_RX_BUFFER_SIZE 100
void USART1_init(void);
void Serial_print(const char* input);
void Serial_println(const char* input);
char* Serial_read(void);
void Serial_begin(int baudrate);
void USART2_init(void);
void Serial2_print(const char* input);
void Serial2_println(const char* input);
char* Serial2_read(void);
void Serial2_begin(int baudrate);
void USART6_init(void);
void Serial6_print(const char* input);
void Serial6_println(const char* input);
char* Serial6_read(void);
void Serial6_begin(int baudrate);
#endif
