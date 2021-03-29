#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef _RENAME_UART_
#define UART UART_
#endif

struct UART;

// initializes a uart object
// returns 0 on failure
struct UART*  UART_init(const char* device, uint32_t baud);

// puts a character in the buffer of a uart
// if the buffer is full, the function waits until
// there is room in the buffer
void UART_putChar(struct UART* uart, uint8_t c);

// returns a character from the uart.
// if no character is available, the function waits
uint8_t UART_getChar(struct UART* uart);

// returns the size of the rx buffer
int  UART_rxbufferSize(struct UART* uart);

// returns the size of the tx buffer
int  UART_txBufferSize(struct UART* uart);

// returns the number of characters to be read fron the rx buffer
int UART_rxBufferFull(struct UART* uart);

// returns the number of available characters in the tx buffer
int UART_txBufferFree(struct UART* uart);

// returns the number of characters to be read fron the rx buffer
int UART_txBufferFull(struct UART* uart);


//ELIMINA DA QUI IN GIÙ
void usart_init(void);
char usart_getchar( void );
void usart_putchar( char data );
void usart_pstr (char *s);
unsigned char usart_kbhit(void);
int usart_putchar_printf(char var, FILE *stream);
void printf_init(void);
uint8_t usart_getString(uint8_t* buf);
