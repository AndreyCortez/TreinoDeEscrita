#include "semaphore.h"
#include <stdlib.h>

// Inicializa o semáforo
int semaphore_init(semaphore_t* sem, int value) {
    if (value < 0) return EINVAL; // Valor inicial inválido

    sem->value = value;

    if (pthread_mutex_init(&sem->mutex, NULL) != 0) return ENOMEM;
    if (pthread_cond_init(&sem->cond, NULL) != 0) {
        pthread_mutex_destroy(&sem->mutex);
        return ENOMEM;
    }

    return 0; // Sucesso
}

// Destroi o semáforo
int semaphore_destroy(semaphore_t* sem) {
    pthread_mutex_destroy(&sem->mutex);
    pthread_cond_destroy(&sem->cond);
    return 0; // Sucesso
}

// Decrementa o semáforo (aguarda recurso)
int semaphore_wait(semaphore_t* sem) {
    pthread_mutex_lock(&sem->mutex);

    while (sem->value <= 0) {
        pthread_cond_wait(&sem->cond, &sem->mutex); // Bloqueia até o recurso estar disponível
    }
    sem->value--; // Decrementa o valor

    pthread_mutex_unlock(&sem->mutex);
    return 0; // Sucesso
}

// Incrementa o semáforo (libera recurso)
int semaphore_post(semaphore_t* sem) {
    pthread_mutex_lock(&sem->mutex);

    sem->value++; // Incrementa o valor
    pthread_cond_signal(&sem->cond); // Sinaliza que o recurso está disponível

    pthread_mutex_unlock(&sem->mutex);
    return 0; // Sucesso
}

// Obtém o valor do semáforo
int semaphore_getvalue(semaphore_t* sem, int* sval) {
    pthread_mutex_lock(&sem->mutex);

    *sval = sem->value;

    pthread_mutex_unlock(&sem->mutex);
    return 0; // Sucesso
}
