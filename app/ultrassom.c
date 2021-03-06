#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define LED_PIN 0

#include <avr/io.h>
#include <util/delay.h>
#include "ultrassom.h"
#include <stdbool.h>

void init_ultrassom(void)
{
	PORTD = ~PORTD & LED_PIN;
}

uint16_t get_distancia_ult()
{
	static int i = 100;
	return i++;
}