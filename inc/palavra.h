#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "raylib.h"
#include "macros.h"

typedef struct
{
    char *pal;
    float velocity;
    Vector2 position;
    
    // Atualizamos a lógica desse ao comparar a palavra atual
    // com a palavra digitada
    int c_corretos; // Numero de caracteres corretos
    int c_errados; // Numero de cacarteres errados
    int c_restantes; // Numero de caracteres restantes 

    bool aprovada; // Se a palavra está aprovada ela pode ser adicionada a pontuação
    int tamanho; // Tamanho da palavra
} palavra;

palavra *palavra_criar(char *pal, float velocity, Vector2 pos);
void palavra_destruir(palavra **pal);

// Compara a palavra atual com a digitada
void palavra_checar(palavra *pal, char *pal_digitada);

// Renderiza as palavras
// AVISO: renderizar palavra apenas quando dentro de uma seção de drawing raylib
void palavra_renderizar(palavra *pal, int tam_fonte);