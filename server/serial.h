
void serial_init(void);

void serial_put_char(uint8_t c);

uint8_t serial_get_char(void);

void serial_put_string(uint8_t* buf);

uint8_t serial_get_string(uint8_t* buf);
