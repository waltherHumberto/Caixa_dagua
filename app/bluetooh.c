#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "bluetooh.h"
#include <stdbool.h>

bool flag_bluetooth = false;

volatile char udr = NULL;
volatile char data[30];

void init_bluetooth(void)
{
    UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);             //this sets the BAUD rate, must shift UBRROH 8 bits to the right
    UBRR0L = (uint8_t)(BAUD_PRESCALLER);                  //sets lower 8 bits of UBRR0
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); //enable receive and send of data
    UCSR0C = (3 << UCSZ00);
    sei();
}

bool verifica_bluetooth()
{
    return flag_bluetooth;
}

void get_message_bt(char string)
{
    strcat(string, data);
    flag_bluetooth = false;
}

void send_message_bt(char *stringPtr)
{
    while (*stringPtr != 0)
    {
        while (!(UCSR0A & (1 << UDRE0)))
            ;
        UDR0 = *stringPtr;
        stringPtr++;
    }
}

char UART_recieve(void)
{
    while (!(UCSR0A & (1 << RXC0)))
        ;        // wait while data is being received
    return UDR0; // return 8-bit data
}

ISR(USART_RX_vect)
{
    static int i;
    udr = UDR0;
    data[i++] = udr;

    if (udr == "\n") //  "\r"
    {
        i = 0;
    }

    flag_bluetooth = true;
}