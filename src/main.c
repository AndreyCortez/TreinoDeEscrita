#include <pthread.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <string.h>
#include <ctype.h>

#include "raylib.h"
#include "palavra.h"
#include "raymath.h"
#include "lista_palavras.h"

semaphore_t *semaforo_escrita;

#define TAM_MAX_PALAVRA 100
#define QTD_MAX_PALAVRA 100

#define RESX 800
#define RESY 800

#define RANDF() (float)rand()/(float)RAND_MAX
#define RANDINT(X) (int)(RANDF() * X)

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Parâmetros globais
semaphore_t calc_semaforo; // Semáforo para cálculo de estatísticas
char palavra_digitada[TAM_MAX_PALAVRA];
int indice_palavra = 0;

pthread_t threads[QTD_MAX_PALAVRA];
palavra *palavras[QTD_MAX_PALAVRA];
int id_palavras[QTD_MAX_PALAVRA];
int num_palavras = 0;

int score = 0;

float sample_time = 5.0;
float cpm = 0;
int char_count = 0;

float wpm = 0;
int word_count = 0;


// Função de pausa compatível com Windows e Linux
void sleep(int ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;              
    ts.tv_nsec = (ms % 1000) * 1000000; 
    nanosleep(&ts, NULL);
}

void *checar_palavra(void* arg)
{
    int id = *(int*)arg;
    if (palavras[id] == NULL)
        return NULL;
    
    if (strcmp(palavras[id]->pal, palavra_digitada) == 0)
    {
        score += strlen(palavras[id]->pal);

        semaphore_wait(&calc_semaforo); 

        char_count += strlen(palavras[id]->pal);
        word_count += 1;

        semaphore_post(&calc_semaforo); 

        palavra_destruir(&(palavras[id]));
        printf("palavra destruida\n");
    }

    return NULL;
}

void *calcular_estatisticas(void *arg)
{   
    while (!WindowShouldClose())
    {
        semaphore_wait(&calc_semaforo); 

        cpm = (float)char_count * 60.0 / sample_time;
        wpm = (float)word_count * 60.0 / sample_time;

        char_count = 0;
        word_count = 0;

        semaphore_post(&calc_semaforo); 
        printf("%f %f\n", cpm, wpm);
        sleep(sample_time * 1000);
    }
}


void* jogo(void* arg) {
    
    for (int i = 0; i < QTD_MAX_PALAVRA; i++)
        id_palavras[i] = i;

    pthread_t stat_thread;
    pthread_create(&stat_thread, NULL, calcular_estatisticas, NULL);

    while (!WindowShouldClose()) {
        
        float delta = GetFrameTime();

        if (RANDF() < 0.01)
        {
            for (int i = 0; i < QTD_MAX_PALAVRA; i++)
            {
                if (palavras[i] == NULL)
                {
                    palavras[i] = palavra_criar(lista_palavras_br[RANDINT(200000)], 20);
                    palavras[i]->position.x *= (RESX * 0.7);
                    printf("palavra criada\n");
                    break;
                }
            }
        }


        char c;
        while (c = GetCharPressed())
        {
            if (isalpha(c))
            {
                c = tolower(c);
                indice_palavra += 1;

                if (indice_palavra >= TAM_MAX_PALAVRA)
                    indice_palavra = TAM_MAX_PALAVRA;
                
                palavra_digitada[indice_palavra - 1] = c;
                palavra_digitada[indice_palavra] = '\0';
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE) && indice_palavra > 0)
        {
            indice_palavra -= 1;
            palavra_digitada[indice_palavra] = '\0';        
        }

        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
        {
            printf("checar palavra!\n");
            
            for (int i = 0; i < QTD_MAX_PALAVRA; i++)
                pthread_create(&threads[i], NULL, checar_palavra, &id_palavras[i]);

            for (int i = 0; i < QTD_MAX_PALAVRA; i++)
                pthread_join(threads[i], NULL);

            printf("%s\n", palavra_digitada);

            indice_palavra = 0;
            palavra_digitada[indice_palavra] = '\0';
        }

        for (int i = 0; i < QTD_MAX_PALAVRA; i++)
        {
            if (palavras[i] != NULL)
            {
                palavras[i]->position.y += (palavras[i]->velocity) * delta;      
                if (palavras[i]->position.y > RESY)
                    palavra_destruir(&(palavras[i]));
            }     
        }


        BeginDrawing();
        ClearBackground(BLACK); 

        for (int i =0; i < QTD_MAX_PALAVRA; i++)
        {
            if (palavras[i] != NULL)
            {
                char *c = palavras[i]->pal;
                int correct = 0;
                int incorrect = 0;
                char cor[100] = "\0";
                char inc[100] = "\0";
                char rest[100] = "\0";
                char extra[100] = "\0";

                int tam_pal = strlen(palavras[i]->pal);
                int tam_pal_digitada = strlen(palavra_digitada);

                while (*c != '\0')
                {
                    if (*c == *((palavra_digitada) + correct))
                    {
                        cor[correct] = *c;
                        cor[correct + 1] = '\0';
                        correct += 1;
                    }
                    else 
                    {
                        incorrect = MIN(tam_pal_digitada - correct, tam_pal - correct);

                        for (int j = 0; j < incorrect; j++)
                        {
                            inc[j] = *c;
                            inc[j + 1] = '\0';
                            c += 1;
                        }

                        for (int j = 0; j < tam_pal - (incorrect + correct); j++)
                        {
                            rest[j] = *c;
                            rest[j + 1] = '\0';
                            c += 1;
                        }     

                        break;
                    }
                    c += 1;
                }
                
                if (tam_pal_digitada > tam_pal && correct >= tam_pal)
                {
                    cor[0] = '\0';
                    strcpy(extra, palavras[i]->pal);
                }

                
                DrawText(cor, palavras[i]->position.x, palavras[i]->position.y, 20, GREEN);
                int dist = MeasureText(cor, 20);
                DrawText(inc, palavras[i]->position.x + dist, palavras[i]->position.y, 20, RED);
                dist = MeasureText(cor, 20) + MeasureText(inc, 20);
                DrawText(rest, palavras[i]->position.x + dist, palavras[i]->position.y, 20, WHITE);
                DrawText(extra, palavras[i]->position.x, palavras[i]->position.y, 20, YELLOW);
            }
        }

        char aux_text[100];

        sprintf(aux_text, "SCORE: %03d\nWPM: %.2f\nCPM: %.2f\n", score, wpm, cpm);
        DrawText(aux_text, 0, 0, 30, GREEN);

        EndDrawing();
    }

    pthread_cancel(stat_thread);

    return NULL;
}

int main() {
    // Inicializa a janela
    const int screenWidth = RESX;
    const int screenHeight = RESY;
    InitWindow(screenWidth, screenHeight, "Raylib - Pêndulos Acoplados");

    // Inicializa semáforo
    semaphore_init(&calc_semaforo, 1);

    SetTargetFPS(60); 
    jogo(NULL);

    semaphore_destroy(&calc_semaforo);
    CloseWindow();

    return 0;
}
