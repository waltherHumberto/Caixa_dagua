#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define LED_PIN 8

#include <avr/io.h>
#include <util/delay.h>

void blink(void)
{
    PORTD = ~PORTD & LED_PIN;
    _delay_ms(1000); // busy wait, 500ms
}