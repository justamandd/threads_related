//Um cenário onde este problema pode surgir é em sistemas bancários, onde múltiplos funcionários precisam consultar os dados de um cliente (leitura), enquanto um gerente pode realizar atualizações (escrita) ao saldo do cliente. É essencial permitir consultas simultâneas, mas com exclusividade nas atualizações.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_CONSULTAS 5
#define NUM_ATUALIZACOES 2

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_consultas = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_atualizacoes = PTHREAD_COND_INITIALIZER;
int consultas = 0;
int atualizacoes = 0;
int aguardando_atualizacoes = 0;
int registro_cliente = 0;

void *consultar(void *arg) {
    int id = *(int *)arg;
    free(arg);

    pthread_mutex_lock(&mutex);
    while (atualizacoes > 0 || aguardando_atualizacoes > 0) {
        pthread_cond_wait(&cond_consultas, &mutex);
    }
    consultas++;
    pthread_mutex_unlock(&mutex);

    printf("Funcionário %d consultou o saldo do cliente: %d\n", id, registro_cliente);
    sleep(1);  // Simula o tempo de consulta

    pthread_mutex_lock(&mutex);
    consultas--;
    if (consultas == 0 && aguardando_atualizacoes > 0) {
        pthread_cond_signal(&cond_atualizacoes);
    }
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

void *atualizar(void *arg) {
    int id = *(int *)arg;
    free(arg);

    pthread_mutex_lock(&mutex);
    aguardando_atualizacoes++;
    while (consultas > 0 || atualizacoes > 0) {
        pthread_cond_wait(&cond_atualizacoes, &mutex);
    }
    aguardando_atualizacoes--;
    atualizacoes++;
    pthread_mutex_unlock(&mutex);

    registro_cliente += 100;  // Simula uma atualização no saldo do cliente
    printf("Gerente %d atualizou o saldo do cliente para: %d\n", id, registro_cliente);
    sleep(2);  // Simula o tempo de atualização

    pthread_mutex_lock(&mutex);
    atualizacoes--;
    if (aguardando_atualizacoes > 0) {
        pthread_cond_signal(&cond_atualizacoes);
    } else {
        pthread_cond_broadcast(&cond_consultas);
    }
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    pthread_t consultas_threads[NUM_CONSULTAS];
    pthread_t atualizacoes_threads[NUM_ATUALIZACOES];

    for (int i = 0; i < NUM_CONSULTAS; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&consultas_threads[i], NULL, consultar, id);
    }

    for (int i = 0; i < NUM_ATUALIZACOES; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&atualizacoes_threads[i], NULL, atualizar, id);
    }

    for (int i = 0; i < NUM_CONSULTAS; i++) {
        pthread_join(consultas_threads[i], NULL);
    }

    for (int i = 0; i < NUM_ATUALIZACOES; i++) {
        pthread_join(atualizacoes_threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_consultas);
    pthread_cond_destroy(&cond_atualizacoes);

    return 0;
}