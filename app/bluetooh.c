#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define LED_PIN 5

#include <avr/io.h>
#include <util/delay.h>

void blink(void)
{
    PORTD = ~PORTD & 0x07;
    _delay_ms(500); // busy wait, 500ms
}