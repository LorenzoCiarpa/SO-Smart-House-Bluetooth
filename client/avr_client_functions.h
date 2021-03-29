int set_name(char **args);
int set_channel_name(char **args);
int set_channel_value(char **args);
int query_channels(char **args);
int help(char **args);
int avr_client_num_builtins();
int avr_connection_init();
int avr_bluetooth_connection_init();
int get_channel_value(char **args);
int get_adc_channel_value(char **args);

//WebServer FUNCTIONS
int set_channel_value_web(char **args, char* answer);
int get_channel_value_web(char **args, char* answer);
int get_adc_channel_value_web(char **args, char* answer);
int send_data_channel_name(char *name, char *i_type, int *channel, char args[]);
