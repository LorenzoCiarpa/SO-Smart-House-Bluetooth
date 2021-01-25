#include <stdio.h>
#include <stdlib.h>
#include "avr_client_shell.h"
#include "error.h"


void avr_client_loop(void){
  char *line;
  char **args;
  int   status;

  do {

    printf("\nsmart_house_host> ");
    line = avr_client_read_line();
    args = avr_client_parse(line);
    status = avr_client_execute(args);
    error_handler(status, args);

    free(line);
    free(args);

  }while(status);
}
