
#include <avr/eeprom.h>
#include "memoria.h"

void ler_informacoes_salvas(caixa_dagua *informacoes)
{
    informacoes->altura_max = eeprom_read_byte((uint8_t *)ALTMAX0) << 8 | eeprom_read_byte((uint8_t *)ALTMAX1);
    informacoes->altura_min = eeprom_read_byte((uint8_t *)ALTMIN0) << 8 | eeprom_read_byte((uint8_t *)ALTMIN1);
    informacoes->altura_instalada = eeprom_read_byte((uint8_t *)ALTINST0) << 8 | eeprom_read_byte((uint8_t *)ALTINST1);
    if (informacoes->altura_max == 0xffff)
        informacoes->altura_max = ALTURA_DEFAULT;
    if (informacoes->altura_min == 0xffff)
        informacoes->altura_min = MAX_DEFAULT;
    if (informacoes->altura_instalada == 0xffff)
        informacoes->altura_instalada = MIN_DEFAULT;
}

void salva_informacoes(caixa_dagua *informacoes)
{
    eeprom_write_byte((uint8_t *)ALTMAX0, ((uint16_t)informacoes->altura_max >> 8) & 0xff);
    eeprom_write_byte((uint8_t *)ALTMAX1, (uint16_t)informacoes->altura_max & 0xff);

    eeprom_write_byte((uint8_t *)ALTMIN0, ((uint16_t)informacoes->altura_min >> 8) & 0xff);
    eeprom_write_byte((uint8_t *)ALTMIN1, (uint16_t)informacoes->altura_min & 0xff);

    eeprom_write_byte((uint8_t *)ALTINST0, ((uint16_t)informacoes->altura_instalada >> 8) & 0xff);
    eeprom_write_byte((uint8_t *)ALTINST1, (uint16_t)informacoes->altura_instalada & 0xff);
}