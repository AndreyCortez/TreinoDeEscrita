#ifndef SEMAPHORE_CUSTOM_H
#define SEMAPHORE_CUSTOM_H

#include <pthread.h> // Para mutexes e condições
#include <errno.h>   // Para códigos de erro

typedef struct {
    int value;              // Valor atual do semáforo
    pthread_mutex_t mutex;  // Mutex para exclusão mútua
    pthread_cond_t cond;    // Variável de condição para sincronização
} semaphore_t;

// Inicializa o semáforo
int semaphore_init(semaphore_t* sem, int value);

// Destroi o semáforo
int semaphore_destroy(semaphore_t* sem);

// Decrementa o semáforo (aguarda recurso)
int semaphore_wait(semaphore_t* sem);

// Incrementa o semáforo (libera recurso)
int semaphore_post(semaphore_t* sem);

// Obtém o valor do semáforo
int semaphore_getvalue(semaphore_t* sem, int* sval);

#endif // SEMAPHORE_CUSTOM_H
