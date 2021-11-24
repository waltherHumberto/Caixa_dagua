#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "include/ultrassom.h"
#include "include/bluetooh.h"
#include "include/display.h"

#define ON 1
#define OFF 0
#define BOMBA DDD7
#define LDVD DDD6
#define LDVM DDD5

void bomba_state(bool state);
void led_verde_state(bool state);
void led_vermelho_state(bool state);

int16_t altura_max = 18;
int16_t altura_min = 14;
bool encher = false;

int main(void)
{
    //bool flag_bluetooth = false;
    int16_t altura_atual = 0;

    // LED VERMELHO // LED VERDE   // BOMBA
    DDRD = _BV(BOMBA) | _BV(LDVD) | _BV(LDVM); // Configura Saida do LED e da Bomba
    init_bluetooth();                          // inicia a comunicação bluetooth com um ponteiro de flag para saber se chegou dado
    init_ultrassom();
    while (1)
    {
        char mensagem[20] = ""; // Estou sempre zerando essa variavel

        put_mensage("Max: 18 cm  Min: 14 cm ");
        put_mensage("Altura: 15 cm ");

        if (!verifica_bluetooth()) // Sem comando bluetooth recebido ele fica monitorando a caixa dagua
        {
            if (get_distancia_ult() >= altura_max - 2) // Colocando um pouco de hysterese para dar tempo do MCU executar o comando e fehcar a bomba
            {
                encher = false;
                bomba_state(OFF);
                led_verde_state(ON);
                led_vermelho_state(OFF);
            }
            else if (get_distancia_ult() < altura_min || encher)
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
        else
        {
            get_message_bt(&mensagem);
            if (!(strncmp(mensagem, "EV", 2))) // Chegou mensagem de configuração
                salva_flash(mensagem);
            if (!(strncmp(mensagem, "LR", 2))) // Chegou mensagem para ler as configurações
                envia_salvados();
            if (!(strncmp(mensagem, "BB", 2))) // Chegou mensagem para acionar a bomba
                encher = true;
        }
        _delay_ms(500); // busy wait, 500ms
    }
    return 1;
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