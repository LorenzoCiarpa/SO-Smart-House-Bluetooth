#include <stdlib.h>
#include <stdio.h>
#include "shell.h"
#include "avr_client_functions.h"
#include "webserver.h"
int main(int argc, char const *argv[]){

/*
  if(avr_connection_init() == 0){
    fprintf(stderr, "Connection problem to the host\n");
    return EXIT_FAILURE;
  }
*/

//SE CAMBI TRA BLUETOOTH E SERIAL DEVI MODIFICARE ANCHE LA UART



if(avr_bluetooth_connection_init() == 0){
  fprintf(stderr, "Connection problem to the host\n");
  return EXIT_FAILURE;
}



//UNCOMMENT TO USE SHELL, COMMENT TO USE WEB-SERVER
  avr_client_loop();


//UNCOMMENT TO USE WEB-SERVER, COMMENT TO USE SHELL
/*
  if(webServerCreate() == -1){
    fprintf(stderr, "Errore nella socket\n");
    return EXIT_FAILURE;
  };
*/

  return EXIT_SUCCESS;
}
