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
void monta_mensagem(char *mensagem);

caixa_dagua caixa;

bool encher = false;

int main(void)
{
    char mensagem_lcd[33] = " ";
    // LED VERMELHO // LED VERDE   // BOMBA
    DDRD = _BV(BOMBA) | _BV(LDVD) | _BV(LDVM); // Configura Saida do LED e da Bomba
    lcd_init(LCD_DISP_ON);
    lcd_puts("   Iniciando      Equipamento");
    _delay_ms(2000); // busy wait, 500ms
    lcd_clrscr();

    lcd_puts("  Lab:Sistemas    Embarcados");
    _delay_ms(2500); // busy wait, 500ms
    lcd_clrscr();

    ler_informacoes_salvas(&caixa);
    init_bluetooth(); // inicia a comunicação bluetooth com um ponteiro de flag para saber se chegou dado
    init_ultrassom();

    while (1)
    {
        char mensagem_bluetooth[50] = ""; // Estou sempre zerando essa variavel

        if (!verifica_bluetooth()) // Sem comando bluetooth recebido ele fica monitorando a caixa dagua
        {
            rotina_da_bomba();
        }

        else
        {
            get_message_bt(&mensagem_bluetooth);

            if (!(strncmp(mensagem_bluetooth, "EV", 2))) // Chegou mensagem_bluetooth de configuração
                salva_informacoes(&caixa);

            if (!(strncmp(mensagem_bluetooth, "LR", 2))) // Chegou mensagem_bluetooth para ler as configurações
                lcd_puts("TUDO");

            if (!(strncmp(mensagem_bluetooth, "BB", 2))) // Chegou mensagem_bluetooth para acionar a bomba
                encher = true;
        }
        monta_mensagem(&mensagem_lcd); // Montando  que será exibida no LCD
        lcd_clrscr();
        lcd_puts(mensagem_lcd);

        _delay_ms(500); // busy wait, 500ms
    }
    return 1;
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

void rotina_da_bomba()
{
    caixa.altura_atual = get_distancia_ult();
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