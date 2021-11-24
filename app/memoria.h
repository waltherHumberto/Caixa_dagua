#define ALTMAX0 0x30
#define ALTMAX1 0x31

#define ALTMIN0 0x32
#define ALTMIN1 0x33

#define ALTINST0 0x34
#define ALTINST1 0x35

#define ALTURA_DEFAULT 250
#define MAX_DEFAULT 180
#define MIN_DEFAULT 100

typedef struct
{
    uint16_t altura_instalada;
    uint16_t altura_max;
    uint16_t altura_min;
    uint16_t altura_atual;

} caixa_dagua;

void ler_informacoes_salvas(caixa_dagua *informacoes);

void salva_informacoes(caixa_dagua *informacoes);