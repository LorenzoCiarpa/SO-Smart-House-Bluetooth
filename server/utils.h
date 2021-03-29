#define EEPROM_MAX_SIZE 4096
#define SIZE 256
#define DIGITAL_OUT 0
#define DIGITAL_IN 1
#define ANALOG 2
#define SET_CHANNEL_NAME 3
#define NAME 0
#define INPUT_TYPE 1
#define CHANNEL 2
#define VALUE 3
#define ACK "ack"
#define NACK "nack"
#define SNAME "sname"

//macros for handling memory addresses of I/O ports

#define PORTA   (* (volatile uint8_t *) 0x22) //PORTA is used for digital in
#define PORTB   (* (volatile uint8_t *) 0x25) //PORTB is used for digital out
#define DDRA    (* (volatile uint8_t *) 0x21) //Data Direction Register to setup I/O
#define DDRB    (* (volatile uint8_t *) 0x24)
#define PINA    (* (volatile uint8_t *) 0x20) //PORTA input pins
#define ADMUX   (* (volatile uint8_t *) 0x7C) //ADC Multiplexer section register
#define ADCSRA  (* (volatile uint8_t *) 0x7A) //ADC Status and Control Register
#define ADC     (* (volatile uint16_t*) 0x78) //ADC Data Register 16 bit


#define REFS0  6 //ADMUX bit 0 select voltage reference for ADC
#define ADPS0  0 //ADCSRA bits. These bits determine the division factor between the XTAL frequency and the input clock to the ADC.
#define ADPS1  1
#define ADPS2  2
#define ADEN   7 //ADCSRA bit 7 set to 1 enables the ADC
#define ADSC   6 //ADC start conversion


#define TCCRA_MASK (1<<WGM10)|(1<<COM1C0)|(1<<COM1C1) | (1<<COM1A1) | (1<<COM1A0) | (1<<COM1B1) | (1<<COM1B0)
#define TCCRB_MASK ((1<<WGM12)|(1<<CS10)) | (1>>WGM13)
#define TCCRA1_MASK (1<<COM2A0) | (1<<COM2A1) | (1<<WGM20)
#define TCCRB1_MASK ((1<WGM22)|(1<<CS20))
