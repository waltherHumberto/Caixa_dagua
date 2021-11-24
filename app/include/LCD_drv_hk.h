/*******************************************************************************************************/
/**												BIblioteca de funcoes de acesso ao display de LCD														**/
/**														Configurações I2C, I2CRGB, P4BITS e P8BITS															**/
/**														Autorizado o uso publico, sem modificações															**/
/**															Desenvolvida por : Prof. Hegler Rocha																	**/
/**																				PUC Minas - 05/2019																					**/
/**																				Arquivo LCD_drv_hk.c																				**/
/**																				 Versão : v1.0.0																						**/
/**															Problemas : contactar hk4@pucminas.br																	**/
/**																																																		**/
/*******************************************************************************************************/

#ifndef	LCD_DRIVER_H
#define LCD_DRIVER_H

#include <inttypes.h>

#define EN											2// Enable bit
#define RS											0// Register select bit
#define D4											4// Data 4
#define D5											5// Data 5
#define D6											6// Data 6
#define D7											7// Data 7
#define BK											3// Back light control
#define DT0											PORTB0
#define DT1											PORTB1
#define DT2											PORTB2
#define DT3											PORTB3
#define DT4											PORTD4										
#define DT5											PORTD5
#define DT6											PORTD6
#define DT7											PORTD7
#define DRS											PORTC0				
#define DEN											PORTC1
#define _PINB										0x23
#define _DDRB										0x24
#define _PORTB									0x25
#define _PINC										0x26
#define _DDRC										0x27
#define _PORTC									0x28
#define _PIND										0x29
#define _DDRD										0x2A
#define _PORTD									0x2B
#define COMMAND                 0
#define LCD_DATA                1
#define FOUR_BITS               2

#define I2C											0
#define I2CRGB									1
#define P4BITS									2
#define P8BITS									3

#define LCD_NOBACKLIGHT					0x00
#define LCD_BACKLIGHT						0xFF
#define BACKLIGHT_OFF           0
#define BACKLIGHT_ON						255


extern uint8_t _backlightStsMask;
extern uint8_t _backlightPinMask;

#ifdef	_LCD_DRIVER
uint8_t _d4=D4,_d5=D5,_d6=D6,_d7=D7,_rs=RS,_en=EN,_bk=BK;
uint8_t _Addr = 0;             // I2C Address of the IO expander
uint8_t _ddr_p=0xD0;
uint16_t _ddr_c=0x0C03;
uint8_t *_port_dl=(uint8_t *)_PORTB;
uint8_t *_port_d=(uint8_t *)_PORTD;
uint8_t *_port_c=(uint8_t *)_PORTC;
#endif

#ifdef _USE_LCD_I2C_RGB
	uint8_t _inter=I2CRGB;			 // interface type : i2c4bits, i2c8bits, p4bits and p8bits
#elif defined _USE_LCD_I2C
	uint8_t _inter=I2C;			 // interface type : i2c4bits, i2c8bits, p4bits and p8bits
#elif defined _USE_LCD_P4BITS
	uint8_t _inter=P4BITS;			 // interface type : i2c4bits, i2c8bits, p4bits and p8bits
#elif defined _USE_LCD_P8BITS
	uint8_t _inter=P8BITS;			 // interface type : i2c4bits, i2c8bits, p4bits and p8bits
#else
	extern uint8_t _inter;			 // interface type : i2c4bits, i2c8bits, p4bits and p8bits
#endif

uint8_t _inter;
uint8_t _data_pins[4];
uint8_t _shadow;      
uint8_t _dirMask;     
typedef enum {POSITIVE,NEGATIVE} t_backlightPol;
t_backlightPol _polarity;  

void PulseRsEn(uint8_t);
void config_driver(int8_t,int8_t,int8_t,int8_t,int8_t);
void write_i2c(uint8_t);
void pulseEnable (uint8_t);
void write4bits(uint8_t, uint8_t);
void i2c_send_byte(uint8_t *, uint8_t);
void send_LCD(uint8_t, uint8_t);
void init_driver(uint8_t);
void setReg(uint8_t, uint8_t, uint8_t);
void setBacklight(uint8_t);
void setBacklightPin(uint8_t, t_backlightPol);

#endif