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
void acoes_com_a_bomba(bool state);
void monta_mensagem(char *mensagem);
void monta_resposta();

caixa_dagua caixa;

bool encher = false;
int dist;

int main(void)
{
    char mensagem_lcd[33] = " ";

    TCCR1A = 0;
    //Timer 1 configurado para Captura
    TCCR1B = _BV(ICES1) | _BV(ICNC1) | _BV(CS10);
    TIMSK1 = _BV(ICIE1); // Habilita interrupçao
    // Configura Saidas
    DDRD = _BV(BOMBA) | _BV(LDVD) | _BV(LDVM) | _BV(TRIGGER);

    lcd_init(LCD_DISP_ON);
    lcd_puts("   Iniciando      Equipamento");
    _delay_ms(500); // busy wait, 500ms
    lcd_clrscr();

    lcd_puts("  Lab:Sistemas    Embarcados");
    _delay_ms(500); // busy wait, 500ms
    lcd_clrscr();

    ler_informacoes_salvas(&caixa);

    uart_init(BAUDRATE, OFF); // inicia a comunicação bluetooth com um ponteiro de flag para saber se chegou dado
    sei();
    uart_send_string("Iniciando Bluetooth\n");
    while (1)
    {
        char mensagem_bluetooth[50] = ""; // Estou sempre zerando essa variavel
        char resposta[50] = "";

        PORTD |= _BV(TRIGGER);
        _delay_ms(10);
        PORTD &= ~_BV(TRIGGER);

        rotina_da_bomba(); // Monitora a altura da agua  e toma as decisões

        if (uart_read_count()) // Chegou mensagem Bluetooth
        {
            uart_get_string(&mensagem_bluetooth);
            if (!(strncmp(mensagem_bluetooth, "ev", 2)))
            { // Chegou mensagem_bluetooth de configuração
                trata_mensagem(&mensagem_bluetooth);
                salva_informacoes(&caixa);
                uart_send_string("evok\n");
            }
            else if (!(strncmp(mensagem_bluetooth, "lr", 2)))
            { // Chegou mensagem_bluetooth para ler as configurações
                monta_resposta();
            }
            else if (!(strncmp(mensagem_bluetooth, "bb", 2)))
            { // Chegou mensagem_bluetooth para mudar o estado da bomba
                encher = !encher;
                uart_send_string("bbok\n");
            }
            else
            {
                uart_send_string("error\n");
            }
        }

        monta_mensagem(&mensagem_lcd); // Montando  que será exibida no LCD
        lcd_clrscr();
        lcd_puts(mensagem_lcd);

        _delay_ms(500); // busy wait, 500ms
    }
    return 1;
}

ISR(TIMER1_CAPT_vect)
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

    if (encher) // Estado da Bomba
        strcat(mensagem, " ON ");
    else
        strcat(mensagem, " OFF");
}

void trata_mensagem(char *mensagem)
{
    // O bluetooth sempre responde em tamanho de 3 com virgula separado as alturas
    caixa.altura_instalada = ((mensagem[3] - '0') * 100) + ((mensagem[4] - '0') * 10) + (mensagem[5] - '0');
    caixa.altura_max = ((mensagem[7] - '0') * 100) + ((mensagem[8] - '0') * 10) + (mensagem[9] - '0');
    caixa.altura_min = ((mensagem[11] - '0') * 100) + ((mensagem[12] - '0') * 10) + (mensagem[13] - '0');
}
void monta_resposta()
{
    char resposta[50] = "";
    sprintf(resposta, "lr,%d,%d,%d,%d,\r\n", caixa.altura_instalada, caixa.altura_max, caixa.altura_min, caixa.altura_atual);
    uart_send_string(resposta);
}

void rotina_da_bomba()
{
    if (dist >= 0) // Tentando limpas alguns ruidos de sinal
        caixa.altura_atual = caixa.altura_instalada - (int16_t)dist;

    if (caixa.altura_atual >= caixa.altura_max - 20)
    // Colocando um pouco de hysterese para dar tempo do MCU executar o comando e fehcar a bomba
    {
        encher = false;
        acoes_com_a_bomba(OFF);
    }
    else if (caixa.altura_atual <= caixa.altura_min || encher)
    //Caso alura esteja abaixo do minimo ou houve um comando para encher
    {
        acoes_com_a_bomba(ON);
    }
    else
    //Altura está entre o max e o min
    {
        encher = false;
        acoes_com_a_bomba(OFF);
    }
}

void acoes_com_a_bomba(bool state)
{
    if (state)
    {
        PORTD |= (1 << BOMBA); // Liga Bomba
        PORTD |= (1 << LDVM);  // Liga LED Vermelho
        PORTD &= ~(1 << LDVD); // Apaga LED Verde
    }
    else
    {
        PORTD &= ~(1 << BOMBA); // desliga Bomba
        PORTD |= (1 << LDVD);   // Liga LED Verde
        PORTD &= ~(1 << LDVM);  // Apaga LED Vermelho
    }
}
