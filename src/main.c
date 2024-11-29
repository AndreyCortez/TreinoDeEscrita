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
#include "macros.h"

semaphore_t *semaforo_escrita;

#define TAM_MAX_PALAVRA 100
#define QTD_MAX_PALAVRA 100

#define RESX 800
#define RESY 800


// Parâmetros globais
semaphore_t calc_semaforo; // Semáforo para cálculo de estatísticas
char palavra_digitada[TAM_MAX_PALAVRA];
int indice_palavra = 0;

pthread_t threads[QTD_MAX_PALAVRA];
palavra *palavras[QTD_MAX_PALAVRA];
int num_palavras = 0;

int score;
int vidas;

float sample_time = 5.0;
float cpm;
int char_count;

float wpm;
int word_count;

float tempo_elapsado;
float dificuldade;
float tempo_aumento_dificuldade = 120;

void jogo();
void game_over();

// Função de pausa compatível com Windows e Linux
void sleep(int ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;              
    ts.tv_nsec = (ms % 1000) * 1000000; 
    nanosleep(&ts, NULL);
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
        sleep(sample_time * 1000);
    }
}

void renderizar_estatisticas()
{
    DrawRectangle(0, 0, 180, 130, (Color){ 130, 130, 130, 50});
    char aux_text[100] = "\0";
    sprintf(aux_text, "SCORE: %03d\nWPM: %.2f\nCPM: %.2f\nVIDAS: %d", score, wpm, cpm, vidas);
    DrawText(aux_text, 0, 0, 30, GREEN);
}

void game_over()
{
    for (int i = 0; i < QTD_MAX_PALAVRA; i++)
            palavra_destruir(&(palavras[i]));

    while (!WindowShouldClose()) 
    {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
        {
            jogo();
        }

        BeginDrawing();

        ClearBackground(BLACK);

        char message[] = "VOCE PERDEU APERTE ENTER OU ESPAÇO PARA REINICIAR";
        float dist = MeasureText(message, 20);
        DrawText(message, (RESX - dist) / 2, RESY/2, 20, WHITE);

        renderizar_estatisticas();

        EndDrawing();
    }
}

void jogo() {

    // Seta as variáveis iniciais do jogo
    tempo_elapsado = 0.0;
    dificuldade = 1.0;

    vidas = 10;
    score = 0;

    wpm = 0;
    word_count = 0;
    
    cpm = 0;
    char_count = 0;

    pthread_t stat_thread;
    pthread_create(&stat_thread, NULL, calcular_estatisticas, NULL);

    while (!WindowShouldClose()) {
        
        float delta = GetFrameTime();
        tempo_elapsado += delta;

        dificuldade = 1.0 + tempo_elapsado / tempo_aumento_dificuldade;

        if (RANDF() < 0.01 * dificuldade)
        {
            for (int i = 0; i < QTD_MAX_PALAVRA; i++)
            {
                if (palavras[i] == NULL)
                {
                    palavras[i] = palavra_criar(lista_palavras_br[RANDINT(200000)],  10 + RANDF() * 10 * dificuldade, (Vector2){RANDF() * RESX * 0.7, 0.0});
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
            
            for (int i = 0; i < QTD_MAX_PALAVRA; i++)
            {
                if (palavras[i] != NULL)
                {
                    if (palavras[i]->aprovada)
                    {
                        semaphore_wait(&calc_semaforo); 
                        
                        score += palavras[i]->tamanho;
                        char_count += palavras[i]->tamanho;
                        word_count += 1;

                        semaphore_post(&calc_semaforo); 

                        palavra_destruir(&(palavras[i]));
                    }
                }
            }
            
            printf("%s\n", palavra_digitada);

            indice_palavra = 0;
            palavra_digitada[indice_palavra] = '\0';
        }

        for (int i = 0; i < QTD_MAX_PALAVRA; i++)
        {
            if (palavras[i] != NULL)
            {
                palavras[i]->position.y += (palavras[i]->velocity) * delta; 
                palavra_checar(palavras[i], palavra_digitada);

                if (palavras[i]->position.y > RESY)
                {
                    palavra_destruir(&(palavras[i]));

                    vidas--;
                    if (vidas <= 0)
                    {
                        game_over();
                        return;
                    }
                }

            }     
        }

        BeginDrawing();

        ClearBackground(BLACK); 

        for (int i =0; i < QTD_MAX_PALAVRA; i++)
        {
            if (palavras[i] != NULL)
                palavra_renderizar(palavras[i], 20);
        }

        renderizar_estatisticas();

        EndDrawing();
    }

    pthread_cancel(stat_thread);

    return;
}

int main() {
    // Inicializa a janela
    const int screenWidth = RESX;
    const int screenHeight = RESY;
    InitWindow(screenWidth, screenHeight, "Raylib - Pêndulos Acoplados");

    // Inicializa semáforo
    semaphore_init(&calc_semaforo, 1);

    SetTargetFPS(60); 
    jogo();

    semaphore_destroy(&calc_semaforo);
    CloseWindow();

    return 0;
}
