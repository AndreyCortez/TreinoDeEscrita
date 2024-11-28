#ifndef LISTA_ENCADEADA_H
#define LISTA_ENCADEADA_H

#include <stddef.h>

// Estrutura de um nó da lista encadeada
typedef struct no {
    void *dado;           // Ponteiro genérico para os dados
    struct no *proximo;   // Ponteiro para o próximo nó
} no_lista;

// Estrutura da lista encadeada
typedef struct lista_encadeada {
    no *cabeca;         
    size_t tamanho;       
} lista_encadeada;


#endif // LISTA_ENCADEADA_H
