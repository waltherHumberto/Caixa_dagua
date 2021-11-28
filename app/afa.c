#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

//the following are definitions for the HC-SR04 sensor
#define trigPin 3
#define echoPin 2

//header files
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdlib.h>

//volatile global variables, can change in interrupt INT0
static volatile unsigned int echoFlag = 0; //volatile int , echoFlag = 0.
static volatile unsigned int pulseStart = 0;
static volatile unsigned int pulseEnd = 0;
static volatile unsigned int microSecPulse = 0;

//function to initialise USART
void initUSART()
{
    UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8); //this sets the BAUD rate, must shift UBRROH 8 bits to the right
    UBRR0L = (uint8_t)(BAUD_PRESCALLER);      //sets lower 8 bits of UBRR0
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);     //enable receive and send of data
    UCSR0C = (3 << UCSZ00);                   //it is set to 8 bit
}

//initialise values
void init()
{
    DDRB |= (1 << trigPin);  //all pins as output
    PORTB = 0x00;            //set the pins to low initially, prevent floating
    DDRD &= ~(1 << echoPin); //all pins as output
    PORTD = 0x00;            //set pins to low
    EIMSK |= (1 << INT0);    //interrupt INT0 enabled
    EICRA |= (1 << ISC00);   // interrupt on rising edge and falling edge
}

//delay function using timer0
void delayFunction_Timer0(int a) //8 bit timer
{
    OCR0A = a;              //counter limit
    TCNT0 = 0x00;           //initialise Timer0 bits
    TCCR0A |= (1 << WGM01); //Timer0, set to CTC mode
    TCCR0B |= (1 << CS00);  //Timer0, CS00 = no pre-scaler
    while ((TIFR0 & (1 << OCF0A)) == 0)
    { //count up to OCF0 value
    }
    TCCR0B = 0x00;        //stop clock when OCF1 value is reached
    TIFR0 = (1 << OCF0A); //set flag to 1
}

//delay function using timer1
void delayFunction_Timer1(int a, int b) //inputs a and b correspond to HIGH and LOW of 16bit timer
{
    OCR1AH = a;    //upper 8 bits of counter limit
    OCR1AL = b;    //lower 8 bits of counter limit
    TCNT1H = 0x00; //initialise Timer1 HIGH bits
    TCNT1L = 0x00; //initialise Timer1 LOW bits
    TCCR1B = 0x0D; //timer1 mode
    while ((TIFR1 & (1 << OCF1A)) == 0)
    { //count up to OCF1 value
    }
    TCCR1B = 0x00;        //stop clock when OCF1 value is reached
    TIFR1 = (1 << OCF1A); //set flag to 1
}

//pulse 10uSecs to trigger input, this starts the ranging process
void signalPulse()
{
    PORTB |= (1 << trigPin);    //set trigPin high
    delayFunction_Timer0(0x9F); //0x9F = 159 = 10 uSecs
    PORTB &= ~(1 << trigPin);   //set trigPin low again
}

//function to iterate over the message string
//cannot directly iterate over the string, so must use char pointer
void putStringUSART(char *stringPtr) //take pointer to char as parameter
{
    while (*stringPtr != 0)
    { //while the data the pointer points to is not null
        while (!(UCSR0A & (1 << UDRE0)))
            ;              //wait to receive data
        UDR0 = *stringPtr; //UDR0 = dereferenced stringPtr characters
        stringPtr++;       //advance stringPtr by 1
    }
}

int main()
{
    int16_t pulseRange = 0;           //counter is 0 initially
    char pulseString[5];              //char array, used for sending string to USART
    initUSART();                      //USART initialization
    init();                           //output initialization
    delayFunction_Timer1(0x03, 0x0C); //	030C = 50 ms delay
    sei();                            //set external interrupt
    while (1)
    {

        signalPulse(); //call signal pulse
        //the following calculates distance.
        pulseRange = microSecPulse * 1024 / (58 * 16); //divide by 58 to get readings in cm, pulse defined in INT0
        putStringUSART("Distance: ");                  //print the distance to data visualizer
        itoa(pulseRange, pulseString, 10);             //convert int to string
        putStringUSART(pulseString);                   //print string to data visualizer
        putStringUSART(" cm\n");                       //print to data visualizer, new line after
        delayFunction_Timer1(0x06, 0x19);              //	0x0619 = 1561 machine cycles = 100ms
                                                       //delayFunction_Timer1(0x3D, 0x08);	//	0x3D08 =  15624 machine cycles = 1 sec
    }
    return 0;
}

//every 16 ticks = 1us.

ISR(INT0_vect)
{
    //if the flag is 0, start timer2 with 1024 pre-scaler
    if (echoFlag == 0)
    {
        TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
        pulseStart = TCNT2;
        echoFlag = 1; //set flag to 1
    }
    else
    {
        TCCR2B = 0;                            //stop timer
        pulseEnd = TCNT2;                      //pulse takes values of counter
        microSecPulse = pulseEnd - pulseStart; //measure total pulse length
        TCNT2 = 0;                             //reset counter
        echoFlag = 0;                          //reset flag
    }
}