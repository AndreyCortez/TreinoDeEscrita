#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "raylib.h"

typedef struct
{
    char *pal;
    float velocity;
    Vector2 position;
    int caracteres_corretos;
} palavra;

palavra *palavra_criar(char *pal, float velocity);
void palavra_destruir(palavra **pal);