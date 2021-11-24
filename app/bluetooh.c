#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define LED_PIN 8

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "bluetooh.h"
#include <stdbool.h>

void init_bluetooth(void)
{
    PORTD = ~PORTD & LED_PIN;
}

bool verifica_bluetooth()
{
    return 1;
}

void get_message_bt(char string)
{
    strcat(string, "BB");
}