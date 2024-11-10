//Em uma equipe de desenvolvimento, todos devem aguardar a conclusão de uma etapa antes de prosseguir para a próxima. Cada membro trabalha na sua parte, mas precisa esperar todos chegarem a um checkpoint antes de continuar

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_MEMBROS 5

pthread_barrier_t barreira;

void trabalhar(voidarg) {
    int id = (int)arg;
    free(arg);

    printf("Membro %d completando fase de desenvolvimento...\n", id);
    sleep(rand() % 3 + 1);  // Simula o trabalho

    printf("Membro %d aguardando na barreira\n", id);
    pthread_barrier_wait(&barreira);

    printf("Membro %d prosseguindo para a próxima fase\n", id);
    sleep(rand() % 3 + 1);  // Simula trabalho adicional

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_MEMBROS];
    pthread_barrier_init(&barreira, NULL, NUM_MEMBROS);

    for (int i = 0; i < NUM_MEMBROS; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&threads[i], NULL, trabalhar, id);
    }

    for (int i = 0; i < NUM_MEMBROS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barreira);
    return 0;
}