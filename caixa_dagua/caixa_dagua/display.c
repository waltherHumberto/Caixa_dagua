#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define LED_PIN 0

#include <avr/io.h>
#include <util/delay.h>
int i;
void put_mensage(char string_to_print)
{
	i = 1;
}
