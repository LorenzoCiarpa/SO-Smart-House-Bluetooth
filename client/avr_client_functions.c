#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>
#include "shell.h"
#include "avr_client_serial.h"
#include "error.h"

#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#define DIGITAL_OUT "0"
#define DIGITAL_IN  "1"
#define ANALOG "2"
#define NACK "nack"

const char *portname = "/dev/ttyACM0";
//const char *portname = "/dev/rfcomm0";


char dest[18] = "98:D3:51:F5:E0:93";
char buf = '\0';
char *name;



bool name_is_set = false;
bool bluetooth_is_set = false;

int fd;
int sock;
int status_conn;
int byte_sent;
int byte_rec;
int byte_need;
//builtin functions of smart house

int set_name(char **args);
int set_channel_name(char **args);
int set_channel_value(char **args);
int query_channels(char **args);
int get_channel_value(char **args);
int get_adc_channel_value(char **args);
int help();

//WebServer FUNCTIONS
int set_channel_value_web(char **args, char *answer);
int get_channel_value_web(char **args, char *answer);
int get_adc_channel_value_web(char **args, char *answer);


char *user_names;
char *user_dout_channels_names[8] = { NULL };
char *user_din_channels_names[8] = { NULL };
char *user_adc_channels_names[8] = { NULL };


char *default_adc_channels_name[] = {
  "analog_in_0",
  "analog_in_1",
  "analog_in_2",
  "analog_in_3",
  "analog_in_4",
  "analog_in_5",
  "analog_in_6",
  "analog_in_7",
};

char *default_din_channels_name[] = {
  "digital_in_0",
  "digital_in_1",
  "digital_in_2",
  "digital_in_3",
  "digital_in_4",
  "digital_in_5",
  "digital_in_6",
  "digital_in_7"
};

char *default_dout_channels_name[] = {
  "switch_0",
  "switch_1",
  "switch_2",
  "switch_3",
  "switch_4",
  "switch_5",
  "switch_6",
  "switch_7"
};

char *builtin_str[] = {
  "set_name",
  "set_channel_name",
  "set_channel_value",
  "query_channels",
  "get_channel_value",
  "get_adc_channel_value",
  "help"
};

int (*builtin_func[]) (char **) = {
  &set_name,
  &set_channel_name,
  &set_channel_value,
  &query_channels,
  &get_channel_value,
  &get_adc_channel_value,
  &help
};

int avr_connection_init(){

  fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
  printf("aperto %s\n",portname);

  if(fd < 0){
    fprintf(stderr, "Error opening device: %s\n", strerror(errno));
    return 0;
  }

  serial_init(fd, 9600, 0);
  set_blocking(fd, 0);
  bluetooth_is_set = false;
  return 1;
}

int avr_bluetooth_connection_init(){
  struct sockaddr_rc addr = { 0 };
  sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

  // set the connection parameters (who to connect to)
  addr.rc_family = AF_BLUETOOTH;
  addr.rc_channel = 1;
  str2ba( dest, &addr.rc_bdaddr );
  printf("connecting...\n");
  // connect to server
  status_conn = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
  if( status_conn < 0 ) {
    fprintf(stderr, "Error opening device: %s\n", strerror(errno));
    return 0;
  }
  bluetooth_is_set = true;
  printf("Connected\n");
  return 1;
}

int avr_client_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

int avr_channels_num(char **channels){
  return sizeof(channels)/ sizeof(char );  //probabilmente char*
}

int avr_get_conf_switch(char **args, char **channels){
  int i;
  for(i = 0; i < 8; i++){
    if(channels[i] != NULL && args[2] != NULL){
      if(strcmp(args[2], channels[i]) == 0)
        return i;
    }
  }
  return -1;
}

int avr_chech_user_channel_input(char **args, char *channel_name, int size){
  if(args[3] != NULL && (strncmp(channel_name, args[2], size) == 0)){
    return 0;
  }
  return 1;
}

void avr_write_channel_name(char **args, char *channels[], int p){
  if(channels[p] == NULL){
    channels[p] = (char *) malloc(sizeof(args[3])*sizeof(char));
    strcpy(channels[p], args[3]);
  }else{
    free(channels[p]);
    channels[p] = (char *) malloc(sizeof(args[3])*sizeof(char));
    strcpy(channels[p], args[3]);
  }
}

void print_channels(char *channels[]){
  int c;
  for(c = 0; c < avr_channels_num(channels); c++){
    if(channels[c] == NULL){
      printf("empty\n");
    }else{
      printf("%s\n", channels[c]);
    }
  }
}

int send_data(char *name, char *i_type, int channel, char args[]){
  char *string;
  char c[2];
  sprintf(c,"%d", channel);
  int size = 8*sizeof(char);
  char *response = malloc(256);
  string = malloc(size);
  sprintf(string, "%s%s%s%s%s%s%s%s", ":", i_type, ":", c , ":", args, ";", "\r");
  size = sizeof(string) + sizeof(name);
  char *invia = malloc(size*sizeof(char));
  sprintf(invia, "%s%s", name, string);

  //BLUETOOTH COMMUNICATION
  if(bluetooth_is_set){
    byte_sent = send(sock, invia, strlen(invia), 0);
    //printf("byte_sent: %d\n",byte_sent);

    if(strcmp(i_type, DIGITAL_IN) == 0){
      string = calloc(10, sizeof(char));
      byte_rec = read(sock, string, 1);

      printf("valore: %s\n",string);
      //printf("byte_rec: %d, valore: %s\n", byte_rec, string);
    }else if(strcmp(i_type, ANALOG) == 0){
      string = calloc(5, sizeof(char));


      byte_need = 4;
      while(byte_need > 0){
        byte_rec = read(sock, string, 4);
        strcat(response, string);
        byte_need -= byte_rec;


      }
      printf("valore: %s\n",response);
      //printf("byte_rec: %d, valore: %s\n", byte_rec, response);


    }
  //SERIAL COMMUNICATION
  }else{
    write(fd, invia, strlen(invia));
    if(strcmp(i_type, DIGITAL_IN) == 0){
      string = calloc(2, sizeof(char));
      read(fd, string, 1);
      string[1] = '\0';

      printf("%s\n", string);
    }else if(strcmp(i_type, ANALOG) == 0){
      string = calloc(4, sizeof(char));
      byte_need = 4;
      while(byte_need > 0){
        byte_rec = read(fd, string, 1);
        strcat(response, string);
        byte_need -= byte_rec;

      }
      printf("%s\n",response);


    }
  }
  free(response);
  free(invia);
  free(string);
  return 1;
}

int send_data_channel_name(char *name, char *i_type, char *channel, char args[]){

  char *string;
  int size = 256*sizeof(char);
  string = malloc(size);
  sprintf(string, "%s%s%s%s%s%s%s%s", ":", i_type, ":", channel , ":", args, ";", "\r");
  size = sizeof(string) + sizeof(name);
  char *invia = malloc(size*sizeof(char));
  sprintf(invia, "%s%s", name, string);

  //BLUETOOTH COMMUNICATION
  if(bluetooth_is_set){

    byte_sent = send(sock, invia, strlen(invia), 0);
    //printf("byte_sent: %d\n",byte_sent);

  //SERIAL COMMUNICATION
  }else{
    write(fd, invia, strlen(invia));

  }

  free(invia);
  free(string);
  return 1;
}

int set_name(char **args){
  if(args[1] == NULL){
    return NO_ARGS;
  }else if(!name_is_set){
    int size = sizeof(args[1]);
    name = malloc(size*sizeof(char));
    sprintf(name, "%s%s", args[1], "\r");

    if(bluetooth_is_set){
      byte_sent = send(sock, name, strlen(name), 0);
      //printf("byte_sent: %d\n",byte_sent);
    }else{
      write(fd, name, strlen(name));
    }

    strcpy(name,args[1]);
    name_is_set = true;
    return SUCCESS;
  }
  return NAME_SETTED;
}

int set_channel_name(char **args){
  int p;
  if(args[1] == NULL){
    return NO_ARGS;
  }else if(!name_is_set){
    return NO_NAME;
  }else if(strcmp(name, args[1]) != 0){
    return BAD_NAME;
  }else{
    if(avr_chech_user_channel_input(args, "switch_", 7) == 0){
      p = avr_get_conf_switch(args, default_dout_channels_name);
      if(p == -1){
        return BAD_CHANNEL_NAME;
      }

      if(send_data_channel_name(args[1],"3",args[2],args[3]) == -1)
        return BAD_DATA;

      avr_write_channel_name(args, user_dout_channels_names, p);
      return SUCCESS;

    }else if(avr_chech_user_channel_input(args, "digital_in_", 11) == 0) {
      p = avr_get_conf_switch(args, default_din_channels_name);
      if(p == -1){
        return BAD_CHANNEL_NAME;
      }

      avr_write_channel_name(args, user_din_channels_names, p);

      if(send_data_channel_name(args[1],"3",args[2],args[3]) == -1)
        return BAD_DATA;
      return SUCCESS;

    }else if(avr_chech_user_channel_input(args, "analog_in_", 10) == 0){
      p = avr_get_conf_switch(args, default_adc_channels_name);
      if(p == -1){
        return BAD_CHANNEL_NAME;
      }

      if(send_data_channel_name(args[1],"3",args[2],args[3]) == -1)
        return BAD_DATA;

      avr_write_channel_name(args, user_adc_channels_names, p);
      return SUCCESS;

    }
  }
  return BAD_ARGS;
}

// && strcmp(args[1], name) == 0
int set_channel_value(char **args){
  int channel;
  if(args[1] == NULL){
    return NO_ARGS;
  }else if(!name_is_set){
    return NO_NAME;
  }else if(strcmp(args[1] ,name) != 0){
    return BAD_NAME;
  }
  channel = avr_get_conf_switch(args, user_dout_channels_names);
  if(channel == -1){
    return BAD_CHANNEL_NAME;
  }else{
    int val;
    if(args[3] == NULL || (val = atoi(args[3]) < 0) || (val = atoi(args[3]) > 255)){
      return BAD_VALUE;
    }else{
      if(send_data(name, DIGITAL_OUT, channel, args[3]) == -1)
        return BAD_DATA;
      return SUCCESS;
   }
  }
  return BAD_ARGS;
}

int get_channel_value(char **args){
  int channel;
  if(args[1] == NULL){
    return NO_ARGS;
  }else if(!name_is_set){
    return NO_NAME;
  }else if(strcmp(args[1], name) != 0){
    return BAD_NAME;
  }
  channel = avr_get_conf_switch(args, user_din_channels_names);
  if(channel == -1){
    return BAD_CHANNEL_NAME;
  }else{
    if(send_data(name, DIGITAL_IN, channel, args[2]) == -1)
      return BAD_DATA;
    return SUCCESS;
  }
  return BAD_ARGS;
}

int get_adc_channel_value(char **args){
  int channel;
  if(args[1] == NULL){
    return NO_ARGS;
  }else if(!name_is_set){
    return NO_NAME;
  }else if(strcmp(args[1], name) != 0){
    return BAD_NAME;
  }

  channel = avr_get_conf_switch(args, user_adc_channels_names);
  if(channel == -1){
      return BAD_CHANNEL_NAME;
   }else{
     if(send_data(name, ANALOG, channel, args[2]) == -1)
      return BAD_DATA;
    return SUCCESS;
  }
  return BAD_ARGS;
}



//WebServer FUNCTIONS

int send_data_web(char *name, char *i_type, int channel, char args[], char *answer){
  char *string;
  char c[2];
  sprintf(c,"%d", channel);
  int size = 8*sizeof(char);
  char *response = malloc(256);
  string = malloc(size);
  sprintf(string, "%s%s%s%s%s%s%s%s", ":", i_type, ":", c , ":", args, ";", "\r");
  size = sizeof(string) + sizeof(name);
  char *invia = malloc(size*sizeof(char));
  sprintf(invia, "%s%s", name, string);


  if(bluetooth_is_set){//CON BLUETOOTH

    byte_sent = send(sock, invia, strlen(invia), 0);
    //printf("byte_sent: %d\n",byte_sent);

    if(strcmp(i_type, DIGITAL_IN) == 0){
      string = calloc(10, sizeof(char));
      byte_rec = read(sock, string, 1);
      strcpy(answer,string);

      //printf("byte_rec: %d, valore: %s\n", byte_rec, string);
    }else if(strcmp(i_type, ANALOG) == 0){
      string = calloc(5, sizeof(char));


      byte_need = 4;
      while(byte_need > 0){
        byte_rec = read(sock, string, 4);
        strcat(response, string);
        byte_need -= byte_rec;

      }

      strcpy(answer,response);


    }
  }else{//SENZA BLUETOOTH

    write(fd, invia, strlen(invia));
    if(strcmp(i_type, DIGITAL_IN) == 0){
      string = calloc(5, sizeof(char));

      byte_rec=read(fd, string, 1);
      strcpy(answer,string);

    }else if(strcmp(i_type, ANALOG) == 0){
      string = calloc(5, sizeof(char));
      byte_need = 4;
      while(byte_need > 0){
        byte_rec = read(fd, string, 1);
        strcat(response, string);
        byte_need -= byte_rec;

      }

      strcpy(answer,response);


    }

  }
  free(invia);
  free(string);
  free(response);
  return 1;
}

int set_channel_value_web(char **args, char *answer){
  int channel;
  if(args[1] == NULL){
    return NO_ARGS;
  }else if(!name_is_set){
    return NO_NAME;
  }else if(strcmp(args[1] ,name) != 0){
    return BAD_NAME;
  }
  channel = avr_get_conf_switch(args, default_dout_channels_name);
  if(channel == -1){
    return BAD_CHANNEL_NAME;
  }else{
    int val;
    if(args[3] == NULL || (val = atoi(args[3]) < 0) && (val = atoi(args[3]) > 255)){
      return BAD_VALUE;
    }else{
      if(send_data_web(name, DIGITAL_OUT, channel, args[3], answer) == -1)
        return BAD_DATA;
      return SUCCESS;
   }
  }
  return BAD_ARGS;
}

int get_channel_value_web(char **args, char *answer){

  int channel;
  if(args[1] == NULL){
    return NO_ARGS;
  }else if(!name_is_set){
    return NO_NAME;
  }else if(strcmp(args[1], name) != 0){
    return BAD_NAME;
  }

  channel = avr_get_conf_switch(args, default_din_channels_name);
  if(channel == -1){
    return BAD_CHANNEL_NAME;
  }else{

    if(send_data_web(name, DIGITAL_IN, channel, args[2], answer) == -1)
      return BAD_DATA;
    return SUCCESS;
  }
  return BAD_ARGS;
}

int get_adc_channel_value_web(char **args, char *answer){
  int channel;
  if(args[1] == NULL){
    return NO_ARGS;
  }else if(!name_is_set){
    return NO_NAME;
  }else if(strcmp(args[1], name) != 0){
    return BAD_NAME;
  }

  channel = avr_get_conf_switch(args, default_adc_channels_name);
  if(channel == -1){
      return BAD_CHANNEL_NAME;
   }else{
     if(send_data_web(name, ANALOG, channel, args[2], answer) == -1)
      return BAD_DATA;
    return SUCCESS;
  }
  return BAD_ARGS;
}




int query_channels(char **args){
  printf("\n DIGITAL OUT:\n");
  print_channels(user_dout_channels_names);
  printf("\n DIGITAL IN:\n");
  print_channels(user_din_channels_names);
  printf("\n ANALOG IN:\n");
  print_channels(user_adc_channels_names);
}

int help(){
  printf("\nset_name <device_name> (name can't be modified if already setted)");
  printf("\nset_channel_name <device_name> <default_channel_name> <user_channel_name> (set channel's name [digital_in_(n), switch_(n), analog_in_(n)])");
  printf("\nset_channel_value <device_name> <user_channel_name> <value> (set digital_out channel's value 0 or 1 for channels from 0 to 3)");
  printf("\nset_channel_value <device_name> <user_channel_name> <value> (PWM mode for channel from 4 to 7 values from 0[HIGH] to 255[LOW])");
  printf("\nget_channel_value <device_name> <user_channel_name> (get digital_in channel's value)");
  printf("\nget_adc_channel_value <device_name> <user_channel_name> (get adc channel's value)");
  printf("\nquery_channels (lists all channels setted by the user)\n");
}

int avr_client_execute(char **args){
  int i;

  if(args[0] == NULL){
    return 1;
  }

  for (i = 0; i < avr_client_num_builtins(); i++) {
   if (strcmp(args[0], builtin_str[i]) == 0) {
     return (*builtin_func[i])(args);
   }else if(strcmp(args[0], "exit")==0){
     return EXIT_SUCCESS;
   }
  }
  return NO_COMMAND;

}
