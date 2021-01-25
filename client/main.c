#include <stdlib.h>
#include <stdio.h>
#include "shell.h"
#include "avr_client_functions.h"

int main(int argc, char const *argv[]){

  if(avr_connection_init() == 0){
    fprintf(stderr, "Connection problem to the host\n");
    return EXIT_FAILURE;
  }
  avr_client_loop();

  return EXIT_SUCCESS;
}
