#include "palavra.h"

palavra *palavra_criar(char *pal, float velocity, Vector2 pos)
{
    palavra *ret = malloc(sizeof(palavra));
    
    ret->pal = malloc(sizeof(char) * (strlen(pal) + 1));
    strcpy(ret->pal, pal);

    ret->velocity = velocity;
    ret->position = pos;
    ret->tamanho = strlen(pal);

    ret->c_corretos = 0;
    ret->c_errados = 0;
    ret->c_restantes = ret->tamanho;

    return ret;
}

void palavra_checar(palavra *pal, char *pal_digitada)
{
    char *c = pal->pal;

    pal->c_corretos = 0;
    pal->c_errados = 0;
    pal->c_restantes = pal->tamanho;


    int tam_pal_digitada = strlen(pal_digitada);

    while (*c != '\0')
    {
        if (*c == *(pal_digitada + pal->c_corretos))
            pal->c_corretos += 1;
        else 
            break;

        c += 1;
    }

    pal->c_errados =  MIN(tam_pal_digitada - pal->c_corretos, pal->tamanho - pal->c_corretos);
    
    pal->c_restantes = pal->tamanho - (pal->c_corretos + pal->c_errados);
    if (pal->c_restantes == 0)
        pal->c_restantes = pal->tamanho - tam_pal_digitada;

    // printf("%d %d %d\n", pal->c_corretos, pal->c_errados, pal->c_restantes);

    // Checa se a palavra pode ser aceita ao apertar enter
    if (pal->c_corretos == pal->tamanho && pal->c_corretos == tam_pal_digitada)
        pal->aprovada = true;
    else pal->aprovada = false;

}


void palavra_renderizar(palavra *pal, int tam_fonte)
{
    char palavra[100] = "\0";

    DrawRectangle(pal->position.x, pal->position.y, MeasureText(pal->pal, tam_fonte), tam_fonte, GRAY);

    if (pal->c_corretos == pal->tamanho && pal->c_restantes < 0)
    {
        DrawText(pal->pal, pal->position.x, pal->position.y, tam_fonte, YELLOW);
        return;
    }

    strncpy(palavra, pal->pal, pal->c_corretos);
    DrawText(palavra, pal->position.x, pal->position.y, tam_fonte, GREEN);
    float dist = MeasureText(palavra, tam_fonte);

    strncpy(palavra, (pal->pal) + pal->c_corretos, pal->c_errados);
    palavra[pal->c_errados] = '\0';
    DrawText(palavra, pal->position.x + dist, pal->position.y, tam_fonte, RED);
    dist += MeasureText(palavra, tam_fonte);

    int rest =  MAX(pal->c_restantes, 0);
    palavra[rest] = '\0';
    strncpy(palavra, &((pal->pal)[pal->c_corretos + pal->c_errados]), rest);
    DrawText(palavra, pal->position.x + dist, pal->position.y, tam_fonte, WHITE);
    
}

void palavra_destruir(palavra **pal)
{
    if (*pal == NULL)
        return;
    free((*pal)->pal);
    (*pal)->pal = NULL;
    free(*pal);
    *pal = NULL;
}