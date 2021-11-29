#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>
#include "display.h"
#include "memoria.h"
#include "pcf8574.h"
#include "lcdpcf8574.h"
#include "uart_hal.h"

#define ON 1
#define OFF 0
#define BOMBA DDD7
#define LDVD DDD6
#define LDVM DDD5
#define TRIGGER DDD2

#define BAUDRATE 9600

void rotina_da_bomba();
void bomba_state(bool state);
void led_verde_state(bool state);
void led_vermelho_state(bool state);
void monta_mensagem(char *mensagem);
void monta_resposta();

caixa_dagua caixa;

bool encher = false;
long dist;

int main(void)
{
    char mensagem_lcd[33] = " ";
    /* configure timer1 in normal mode */
    TCCR1A = 0;
    /* configure icr on rising edge with noise filter */
    TCCR1B = _BV(ICES1) | _BV(ICNC1) | _BV(CS10);
    TIMSK1 = _BV(ICIE1);
    // LED VERMELHO // LED VERDE   // BOMBA
    DDRD = _BV(BOMBA) | _BV(LDVD) | _BV(LDVM) | _BV(TRIGGER); // Configura Saida do LED e da Bomba
    lcd_init(LCD_DISP_ON);
    lcd_puts("   Iniciando      Equipamento");
    //_delay_ms(2000); // busy wait, 500ms
    lcd_clrscr();

    lcd_puts("  Lab:Sistemas    Embarcados");
    //_delay_ms(2500); // busy wait, 500ms
    lcd_clrscr();

    ler_informacoes_salvas(&caixa);
    uart_init(BAUDRATE, OFF); // inicia a comunicação bluetooth com um ponteiro de flag para saber se chegou dado
    while (1)
    {

        char mensagem_bluetooth[50] = ""; // Estou sempre zerando essa variavel
        char resposta[50] = "";

        PORTD |= _BV(PORTD2);
        _delay_ms(10);
        PORTD &= ~_BV(PORTD2);

        uart_send_string("Teste Bluetooth\n");

        rotina_da_bomba();

        if (uart_read_count()) // Chegou mensagem Bluetooth
        {
            uart_get_string(&mensagem_bluetooth);
            if (!(strncmp(mensagem_bluetooth, "EV", 2)))
            { // Chegou mensagem_bluetooth de configuração
                trata_mensagem();
                salva_informacoes(&caixa);
                uart_send_string("evok\n");
            }
            else if (!(strncmp(mensagem_bluetooth, "LR", 2)))
            { // Chegou mensagem_bluetooth para ler as configurações
                monta_resposta();
            }
            else if (!(strncmp(mensagem_bluetooth, "BB", 2)))
            { // Chegou mensagem_bluetooth para mudar o estado da bomba
                encher = !encher;
                uart_send_string("bbok\n");
            }
        }

        monta_mensagem(&mensagem_lcd); // Montando  que será exibida no LCD
        lcd_clrscr();
        lcd_puts(mensagem_lcd);

        _delay_ms(500); // busy wait, 500ms
    }
    return 1;
}

ISR(TIMER1_CAP_VET)
{
    static long temp = 0;
    if (TCCR1B & _BV(ICES1))
        temp = ICR1;
    else
        dist = ((ICR1 - temp) * 1e6) / F_CPU / 5.8;
    TCCR1B ^= _BV(ICES1);
}

void monta_mensagem(char *mensagem)
{
    char aux[32] = "";
    sprintf(aux, "Mx:%d.%d\rMn:%d.%d\nAtl:%d.%d\r", caixa.altura_max / 10, caixa.altura_max % 10, caixa.altura_min / 10, caixa.altura_min % 10, caixa.altura_atual / 10, caixa.altura_atual % 10);
    strcpy(mensagem, aux);

    if (encher)
        strcat(mensagem, " ON ");
    else
        strcat(mensagem, " OFF");
}

void trata_mensagem(char *mensagem)
{
    int i = 3;
    int z = 0;
    int y = 0;
    int length = 0;
    for (int z = i; mensagem[z] != ',' || mensagem[z] != '\n'; z++)
        length++;
    for (y = 0; y < length; y++)
    {
        caixa.altura_instalada += mensagem[i] - '0' * 10 ^ length;
        i++;
        length--;
    }
    caixa.altura_max = mensagem[i] - '0';
    caixa.altura_min = mensagem[i] - '0';
}
void monta_resposta()
{
    char resposta[50] = "";
    sprintf(resposta, "ev,%d,%d,%d,%d,\r\n", caixa.altura_instalada, caixa.altura_max, caixa.altura_min, caixa.altura_atual);
    uart_send_string(resposta);
}

void rotina_da_bomba()
{
    caixa.altura_atual = (uint16_t)dist;
    if (caixa.altura_atual >= caixa.altura_max - 20) // Colocando um pouco de hysterese para dar tempo do MCU executar o comando e fehcar a bomba
    {

        encher = false;
        bomba_state(OFF);
        led_verde_state(ON);
        led_vermelho_state(OFF);
    }
    else if (caixa.altura_atual <= caixa.altura_min || encher)
    {
        encher = true;
        bomba_state(ON);
        led_verde_state(OFF);
        led_vermelho_state(ON);
    }
    else
    {
        encher = false;
        bomba_state(OFF);
        led_verde_state(ON);
        led_vermelho_state(OFF);
    }
}

void bomba_state(bool state)
{
    if (state)
        PORTD |= (1 << BOMBA); // Liga bomba
    else
        PORTD &= ~(1 << BOMBA); // desliga bomba
}

void led_verde_state(bool state)
{
    if (state)
        PORTD |= (1 << LDVD); // Liga LED
    else
        PORTD &= ~(1 << LDVD); // Apaga LED
}

void led_vermelho_state(bool state)
{
    if (state)
        PORTD |= (1 << LDVM); // Liga LED
    else
        PORTD &= ~(1 << LDVM); // Apaga LED
}