#include<stdio.h>
#include "error.h"

void error_handler(int status, char **args){
  switch(status){
    case SUCCESS:
      printf("Done!\n");
    break;
    case NO_ARGS:
      fprintf(stderr, "avr_client: expected arguments. Please use help for more info\n");
    break;
    case NO_NAME:
      fprintf(stderr, "avr_client: expected device name. Please use help for more info\n");
    break;
    case NAME_SETTED:
      fprintf(stderr, "avr_client: name already setted\n");
    break;
    case BAD_NAME:
      fprintf(stderr, "avr_client: no device named %s ", args[1]);
    break;
    case BAD_CHANNEL_NAME:
      fprintf(stderr,"avr_client: no channnel named: %s\n", args[2]);
    break;
    case BAD_ARGS:
        fprintf(stderr, "avr_client: bad args. Please use help for more info. \n");
    break;
    case BAD_VALUE:
        fprintf(stderr, "avr_client: invalid switch value.\n");
    break;
    case NO_COMMAND:
       fprintf(stderr, "avr_client: command not found. Please use \"help\" for info.\n");
    break;
  }
}
