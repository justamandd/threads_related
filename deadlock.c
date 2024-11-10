//Num escritório, se uma função de impressão bloqueia a impressora e outra bloqueia o scanner, ambas aguardando o recurso do outro, ocorre deadlock.

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t impressora = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t scanner = PTHREAD_MUTEX_INITIALIZER;

void imprimir(voidarg) {
    pthread_mutex_lock(&impressora);
    printf("Início da impressão. Bloqueando impressora...\n");
    sleep(1);

    pthread_mutex_lock(&scanner);
    printf("Usando scanner e impressora para impressão completa.\n");
    pthread_mutex_unlock(&scanner);
    pthread_mutex_unlock(&impressora);

    pthread_exit(NULL);
}

void digitalizar(voidarg) {
    pthread_mutex_lock(&scanner);
    printf("Início da digitalização. Bloqueando scanner...\n");
    sleep(1);

    pthread_mutex_lock(&impressora);
    printf("Usando scanner e impressora para digitalização completa.\n");
    pthread_mutex_unlock(&impressora);
    pthread_mutex_unlock(&scanner);

    pthread_exit(NULL);
}

int main() {
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, imprimir, NULL);
    pthread_create(&thread2, NULL, digitalizar, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}