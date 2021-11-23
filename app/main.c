#define F_CPU 16000000UL
#define LED_PIN 0

#include <avr/io.h>
#include <util/delay.h>
#include "ultrassom.h"
#include "bluetooh.h"
#include "display.h"

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRD = 0x07;
    while (1)
    {
        blink();
    }
    return 1;
}