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
	_delay_ms(1000); // busy wait, 500ms
}

float get_distancia_ult()
{
	return 13;
}