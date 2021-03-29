#include <stdio.h>
#include <stdint.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR (F_CPU/16/BAUD-1)

//macros for Serial commmunication
#define UCSR0A  (* (volatile uint8_t *) 0xC0)
#define UCSR0B  (* (volatile uint8_t *) 0xC1)
#define UBRR0H  (* (volatile uint8_t *) 0xC5) //USART0 Baud Rate Register High Byte
#define UBRR0L  (* (volatile uint8_t *) 0xC4) //USART0 Baud Rate Register Low Byte
#define UCSR0C  (* (volatile uint8_t *) 0xC2) //UCSR0C Status Register
#define UDR0    (* (volatile uint8_t *) 0xC6) //UDR0 Data Register (Sent/Received)


void serial_init(void){
    UBRR0H = (uint8_t)(MYUBRR>>8);
    UBRR0L = (uint8_t)(MYUBRR);

    UCSR0C = (1<<2) | (1<<1); //8-bit data
    UCSR0B = (1<<4) | (1<<3) | (1<<7);
}

void serial_put_char(uint8_t c){
  while(!(UCSR0A &(1<<5)));

  UDR0 = c;

}

uint8_t serial_get_char(void){
  while(!(UCSR0A &(1<<7)));

  return UDR0;
}

void serial_put_string(uint8_t* buf){
  while(*buf){
    serial_put_char(*buf);
    ++buf;
  }
}

uint8_t serial_get_string(uint8_t* buf){
  uint8_t* b0 = buf;
  while(1){
    uint8_t c = serial_get_char();
    *buf=c;
    ++buf;
    if(c==0)
      return buf-b0;
    if(c=='\n' || c=='\r'){
      *buf=0;
      ++buf;
      return buf-b0;
    }
  }
}
