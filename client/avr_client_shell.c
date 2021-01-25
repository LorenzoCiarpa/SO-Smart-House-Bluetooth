#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "avr_client_functions.h"

#define AVR_CLIENT_BUFFER 256
#define AVR_CLIENT_TOK_BUFFER 64
#define AVR_CLIENT_TOK_DELIM " \t\r\n\a"


char *avr_client_read_line(void){
  int buffsize = AVR_CLIENT_BUFFER;
  int position = 0;
  char *buffer = malloc(sizeof(char)*buffsize);
  int c;

  if(!buffer) {
    fprintf(stderr, "avr_client: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while(1){
    c = getchar();

    if(c == EOF || c == '\n'){
      buffer[position] = '\0';
      return buffer;
    }else{
      buffer[position] = c;
    }
    position++;

    if(position >= buffsize){
      buffsize += AVR_CLIENT_BUFFER;
      buffer = realloc(buffer, buffsize);

      if(!buffer){
        fprintf(stderr, "avr_client: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

char **avr_client_parse(char *line){
  int buffsize = AVR_CLIENT_TOK_BUFFER, position = 0;
  char **tokens = malloc(buffsize * sizeof(char*));
  char *token;

  if(!tokens){
    fprintf(stderr, "avr_client: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, AVR_CLIENT_TOK_DELIM);
  while(token != NULL){
    tokens[position] = token;
    position++;

    if(position  >= buffsize){
      buffsize += AVR_CLIENT_TOK_BUFFER;
      tokens = realloc(tokens, buffsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "avr_client: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, AVR_CLIENT_TOK_DELIM);

  }
  tokens[position] = NULL;
  return tokens;
}
