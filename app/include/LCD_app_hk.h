/*******************************************************************************************************/
/**												BIblioteca de funcoes de acesso ao display de LCD														**/
/**														Configura��es I2C, I2CRGB, P4BITS e P8BITS															**/
/**														Autorizado o uso publico, sem modifica��es															**/
/**															Desenvolvida por : Prof. Hegler Rocha																	**/
/**																				PUC Minas - 05/2019																					**/
/**																				Arquivo LCD_app_hk.h																						**/
/**																				 Vers�o : v1.0.0																						**/
/**															Problemas : contactar hk4@pucminas.br																	**/
/**																																																		**/
/*******************************************************************************************************/

#ifndef LCD_app_h
#define LCD_app_h

#include <inttypes.h>
#include <LCD_drv_hk.h>

#ifndef _BV
#define _BV(bit) (1 << bit)
#endif

#define LCD_NOBACKLIGHT 0x00
#define LCD_BACKLIGHT 0xFF
#define ADDRESS_I2C 0x3F
#define ADDRESS_I2C_RGB_M 0x3E
#define ADDRESS_I2C_RGB_S 0x62
#define OUTPUT 1
#define INPUT 0

// LCD Commands
// ---------------------------------------------------------------------------
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04	//I/D (incremet/decrement cursor  , S (shift display)
#define LCD_DISPLAYCONTROL 0x08 //D (on/off), C (on/off), B (curson blink)
#define LCD_CURSORSHIFT 0x10	// S/C , R/L  (Cursor and Display shift)
#define LCD_FUNCTIONSET 0x20	// DL (data lenght), N (number of lines), F (fonte type - 5x8/5x10)
#define LCD_SETCGRAMADDR 0x40	// Set CGRAM address (character generator)
#define LCD_SETDDRAMADDR 0x80	// Set DDRAM address (display data ram)

// flags for display entry mode
// ---------------------------------------------------------------------------
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off and cursor control
// ---------------------------------------------------------------------------
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
// ---------------------------------------------------------------------------
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
// ---------------------------------------------------------------------------
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00
#define BUFFER_LENGTH 32

// Define COMMAND and DATA LCD Rs (used by send method).
// ---------------------------------------------------------------------------
#define COMMAND 0
#define LCD_DATA 1
#define FOUR_BITS 2

#define REG_RED 0x04   // pwm2
#define REG_GREEN 0x03 // pwm1
#define REG_BLUE 0x02  // pwm0
#define WHITE 0
#define RED 1
#define GREEN 2
#define BLUE 3

#define BACKLIGHT_OFF 0
#define BACKLIGHT_ON 255
#define ON 1
#define OFF 0
#define RIGHT 1
#define LEFT 0
#define I2C 0
#define I2CRGB 1
#define P4BITS 2
#define P8BITS 3

// Interface type
//----------------------------------------------------------------------------

uint8_t _backlightPinMask; // Backlight IO pin mask
uint8_t _backlightStsMask; // Backlight status mask
uint8_t _displayfunction;  // LCD_5x10DOTS or LCD_5x8DOTS, LCD_4BITMODE or
uint8_t _displaycontrol;   // LCD base control command LCD on/off, blink, cursor
uint8_t _displaymode;	   // Text entry mode to the LCD
uint8_t _numlines;		   // Number of lines of the LCD, initialized with begin()
uint8_t _numcols;		   // Number of columns in the LCD
uint8_t _red;
uint8_t _green;
uint8_t _blue;

#ifdef _USE_LCD_I2C_RGB
//#warning "_USE_LCD_I2C_RGB"
uint8_t _intertype = I2CRGB;			  // interface type : i2c4bits, i2c8bits, p4bits and p8bits
uint8_t _i2c_address = ADDRESS_I2C_RGB_M; // addres for i2c interface, 0 for parallel
uint8_t _i2c_LED_addr = ADDRESS_I2C_RGB_S;
#elif defined _USE_LCD_I2C
//#warning "_USE_LCD_I2C"
uint8_t _intertype = I2C;			// interface type : i2c4bits, i2c8bits, p4bits and p8bits
uint8_t _i2c_address = ADDRESS_I2C; // addres for i2c interface, 0 for parallel
uint8_t _i2c_LED_addr;
#elif defined _USE_LCD_P4BITS
//#warning "_P4BITS"
uint8_t _intertype = P4BITS; // interface type : i2c4bits, i2c8bits, p4bits and p8bits
uint8_t _i2c_address = 0;	 // address for i2c interface, 0 for parallel
uint8_t _i2c_LED_addr;
#elif defined _USE_LCD_P8BITS
//#warning "_P8BITS"
uint8_t _intertype = P8BITS; // interface type : i2c4bits, i2c8bits, p4bits and p8bits
uint8_t _i2c_address = 0;	 // addres for i2c interface, 0 for parallel
uint8_t _i2c_LED_addr;
#else
extern uint8_t _intertype;	 // interface type : i2c4bits, i2c8bits, p4bits and p8bits
extern uint8_t _i2c_address; // address for i2c interface, 0 for parallel
extern uint8_t _i2c_LED_addr;
#endif

void config_dsp(int8_t, int8_t, int8_t, int8_t, int8_t);
void putnumber_f(int8_t, int8_t, double, uint8_t);
void _ltoa(char *, long, uint8_t);
void putnumber_i(int8_t, int8_t, long, int8_t);
void putmessage(int8_t, int8_t, char *);
void put_c(int8_t, int8_t, char);
void setCursor(int8_t, int8_t);
void createChar_LCD(uint8_t, char *);
void dspHome(void);
void dspClear(void);
void init_dsp(uint8_t, uint8_t);
void setRGB(uint8_t, uint8_t, uint8_t);
void autoscroll(uint8_t);
void moveCursor(uint8_t);
void entrymode(uint8_t);
void scrollDisplay(uint8_t);
void turncursorBlink(uint8_t);
void turncursor(uint8_t);
void turndisplay(uint8_t, uint8_t);
void setBacklight(uint8_t);
void setBacklightPin(uint8_t, t_backlightPol);
void turnbacklight(uint8_t);

#endif