#include <stdio.h>
#include <stdlib.h>
#include <libwebsockets.h>
#include "avr_client_functions.h"
#include "avr_client_shell.h"
#include "webserver.h"


typedef struct WSContent{
  int port;
  const char *interface;

}WSContent;

static int callback_http(
    struct lws *wsi,
    enum lws_callback_reasons reason,
    void *user,
    void *in,
    size_t len
) {
  switch (reason) {

    case LWS_CALLBACK_ESTABLISHED:
      printf("connection established\n");
      break;

    case LWS_CALLBACK_RECEIVE: {

      break;
    }
    default:
      break;
  }

  return 0;
}

static int callback_dumb_increment( /*struct libwebsocket_context * this_context,*/
                                  struct lws *wsi,
                                  enum lws_callback_reasons reason,
                                  void *user, void *in, size_t len) {


  switch (reason) {
  case LWS_CALLBACK_ESTABLISHED:
    printf("connection established\n");

    break;



  case LWS_CALLBACK_RECEIVE:
    {


      char buf[len+1];

      strncpy(buf,in,len);
      buf[len] = '\0';



      int res = manageWebServerRequest(wsi, buf);

    }
    break;


  default:
    break;
  }

  return 0;
}

int manageWebServerRequest(struct lws *wsi, char* buf){
  char** args;
  char* temp;
  char* answer;
  char* final_answer;


  int final_size;
  int answer_size;
  int res;
  int size = strlen(buf);

  args = avr_client_parse(buf);

  answer = malloc(5);
  final_answer = malloc(6+size);

  if(strcmp(args[0],"get_channel_value_web") == 0){
    strcpy(final_answer, args[2]);
    strcat(final_answer," ");


    res = get_channel_value_web(args,answer);

    strcat(final_answer, answer);
    final_size = strlen(final_answer);

    lws_write(wsi, final_answer,final_size, LWS_WRITE_TEXT);



  }else if(strcmp(args[0],"get_adc_channel_value_web") == 0){
    strcpy(final_answer, args[2]);
    strcat(final_answer," ");


    res = get_adc_channel_value_web(args,answer);

    strcat(final_answer, answer);
    final_size=strlen(final_answer);
    lws_write(wsi, final_answer,final_size, LWS_WRITE_TEXT);


  }else if(strcmp(args[0],"set_name") == 0){
    res = set_name(args);



  }else if(strcmp(args[0],"set_channel_value_web")==0){
    res = set_channel_value_web(args, answer);

  }

  free(args);
  free(answer);

}


int webServerCreate(){
  int port = 9000;
  int opts = 0;
  const char *interface = NULL;
  struct lws_context *context;
  struct lws_context_creation_info info;


  memset(&info, 0, sizeof info);

  struct lws_protocols protocols[] = {
    /* first protocol must always be HTTP handler */
    {
      .name="http-only",   // name
      .callback=callback_http, // callback
      .per_session_data_size=0,              // per_session_data_size
      .rx_buffer_size=0
    },
    {
      .name = "dumb-increment-protocol", // protocol name - very important!
      .callback = callback_dumb_increment,   // callback
      .per_session_data_size=0,                          // we don't use any per session data
      .rx_buffer_size=0
    },
    {
      .name=NULL,
        .callback=NULL,
        .per_session_data_size=0,   /* End of list */
        .rx_buffer_size=0
    }
  };

  info.port = port;
  info.iface = interface;
  info.protocols = protocols;
  //info.extensions = lws_get_internal_extensions();
  //if (!use_ssl) {
  info.ssl_cert_filepath = NULL;
  info.ssl_private_key_filepath = NULL;
  //} else {
  //  info.ssl_cert_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.pem";
  //  info.ssl_private_key_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.key.pem";
  //}
  info.gid = -1;
  info.uid = -1;
  info.options = opts;



  // create libwebsocket context representing this server
  context = lws_create_context(&info);

  if (context == NULL) {
    fprintf(stderr, "libwebsocket init failed\n");
    return -1;
  }

  printf("starting server...\n");

  // infinite loop, to end this server send SIGTERM. (CTRL+C)
  while (1) {
    lws_service(context, 50);
    // libwebsocket_service will process all waiting events with
    // their callback functions and then wait 50 ms.
    // (this is a single threaded web server and this will keep our
    // server from generating load while there are not
    // requests to process)
  }

  lws_context_destroy(context);

  return 0;

}
