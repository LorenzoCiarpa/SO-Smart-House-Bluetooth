/* avr_client errors codes */
#define SUCCESS 2
#define NO_ARGS 101
#define NO_NAME 201
#define BAD_NAME 202
#define NAME_SETTED 203
#define BAD_CHANNEL_NAME 301
#define BAD_ARGS 302
#define BAD_VALUE 401
#define NO_COMMAND 501
#define BAD_DATA 601

void error_handler(int status, char **args);
