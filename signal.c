//Imagine um administrador de sistemas enviando sinais para pausar um serviço de backup.

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

void signal_handler(int signum) {
    printf("Serviço recebeu o sinal %d e está pausando\n", signum);
    pthread_exit(NULL);
}

void servico(voidarg) {
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);

    printf("Serviço ativo e aguardando sinal para pausa...\n");
    pause();  // Espera por um sinal
    return NULL;
}

int main() {
    pthread_t thread;
    pthread_create(&thread, NULL, servico, NULL);
    sleep(1);

    printf("Administrador enviando sinal de pausa ao serviço\n");
    pthread_kill(thread, SIGUSR1);

    pthread_join(thread, NULL);
    return 0;
}