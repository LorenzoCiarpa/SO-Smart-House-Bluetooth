#include <avr/eeprom.h>
#include "uart.h"
#include "buffer_utils.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <util/atomic.h>
#include <stdint.h>
#include <stdio.h>

#include "eeprom_functions.h"

/*
void eprom_mem_set(int size, uint8_t word);
void eprom_read_one(char* res, int size);
void eprom_write_one(char* parola, int size);
int eeprom_get_size(void);
*/


void eprom_read_one(char* res, int size){
  int idx = 0;
  while(idx < size){
    eeprom_busy_wait();
    res[idx] = eeprom_read_byte(( uint8_t *) idx);
    idx++;
  }
  res[idx] = '\0';
}

void eprom_write_one(char* parola, int size){
  int idx = 0;
  int idx_start = eeprom_get_size();
  while(idx < size){
    eeprom_busy_wait();
    eeprom_write_byte(( uint8_t *) idx_start, parola[idx]);
    idx++;
    idx_start++;
  }
  eeprom_busy_wait();
  eeprom_write_byte(( uint8_t *) idx_start, ';');
}

void eprom_mem_set(int size, uint8_t word){
  int idx = 0;
  while (idx < size){
    eeprom_busy_wait();
    eeprom_write_byte(( uint8_t *) idx, word);
    idx++;
  }

}


int eeprom_get_size(){
  uint8_t ByteOfData;
  int cont = 0;
  eeprom_busy_wait();
  ByteOfData = eeprom_read_byte(( uint8_t *) cont);
  while (ByteOfData != 0xff){
    cont++;
    eeprom_busy_wait();
    ByteOfData = eeprom_read_byte(( uint8_t *) cont);

  }
  return cont;
}
