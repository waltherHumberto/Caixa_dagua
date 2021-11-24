#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>
#include "ultrassom.h"
#include "bluetooh.h"
#include "display.h"
#include "memoria.h"
#include "pcf8574.h"
#include "lcdpcf8574.h"

#define ON 1
#define OFF 0
#define BOMBA DDD7
#define LDVD DDD6
#define LDVM DDD5

void rotina_da_bomba();
void bomba_state(bool state);
void led_verde_state(bool state);
void led_vermelho_state(bool state);

caixa_dagua caixa;

bool encher = false;

int main(void)
{

    // LED VERMELHO // LED VERDE   // BOMBA
    DDRD = _BV(BOMBA) | _BV(LDVD) | _BV(LDVM); // Configura Saida do LED e da Bomba
    lcd_init(LCD_DISP_ON);
    ler_informacoes_salvas(&caixa);
    init_bluetooth(); // inicia a comunicação bluetooth com um ponteiro de flag para saber se chegou dado
    init_ultrassom();

    while (1)
    {
        char mensagem[50] = ""; // Estou sempre zerando essa variavel

        lcd_puts(" Max:18 Min: 14 ");
        lcd_puts("Atl:18 Bomba:ON");

        if (!verifica_bluetooth()) // Sem comando bluetooth recebido ele fica monitorando a caixa dagua
        {
            rotina_da_bomba();
        }
        else
        {
            get_message_bt(&mensagem);

            if (!(strncmp(mensagem, "EV", 2))) // Chegou mensagem de configuração
                salva_informacoes(&caixa);

            if (!(strncmp(mensagem, "LR", 2))) // Chegou mensagem para ler as configurações
                put_mensage("TUDO");

            if (!(strncmp(mensagem, "BB", 2))) // Chegou mensagem para acionar a bomba
                encher = true;
        }
        _delay_ms(500); // busy wait, 500ms
    }
    return 1;
}

void rotina_da_bomba()
{
    if (get_distancia_ult() >= caixa.altura_max - 2) // Colocando um pouco de hysterese para dar tempo do MCU executar o comando e fehcar a bomba
    {
        encher = false;
        bomba_state(OFF);
        led_verde_state(ON);
        led_vermelho_state(OFF);
    }
    else if (get_distancia_ult() < caixa.altura_min || encher)
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