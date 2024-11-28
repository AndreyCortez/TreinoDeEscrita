#include "palavra.h"

palavra *palavra_criar(char *pal, float velocity)
{
    palavra *ret = malloc(sizeof(palavra));
    ret->pal = malloc(sizeof(char) * (strlen(pal) + 1));
    strcpy(ret->pal, pal);
    ret->velocity = velocity;
    ret->position = (Vector2){(float)rand()/(float)RAND_MAX, 0.0};
    ret->caracteres_corretos = 0;
}

void palavra_destruir(palavra **pal)
{
    free((*pal)->pal);
    (*pal)->pal = NULL;
    free(*pal);
    *pal = NULL;
}