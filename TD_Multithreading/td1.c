#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int M = 10;
int N = 10;

/* fonction realisant le thread */
void * fonction(void *arg) { 
    int i=0;
    for (i = 0; i < N; i++) {
    printf("et mon courroux\n");
    sleep(1);
    }
    return (NULL);
}


int main(int argc, char *argv[]) {
    int i=0;

    if (argc == 3) {
        M = atoi(argv[1]); // Convert first argument to integer for M
        N = atoi(argv[2]); // Convert second argument to integer for N
    } else {
        printf("Usage: %s <M> <N>\n", argv[0]);
        printf("Using default values: M = %d, N = %d\n", M, N);
    }

    pthread_t tid;
    // creation thread
    if(pthread_create(&tid, NULL, fonction,NULL)){
    printf("erreur create\n"); exit(0);
    }

    for (i = 0; i < M; i++) {
    printf("coucou\n");
    sleep(1);
    }

    // td 1.3.1: utilisation pthread_exit()
    //pthread_exit(NULL);

    // td 1.3.2: Destruction join
    // printf("avant join\n");
    // pthread_join(tid, NULL);
    // printf("après join\n");
    // pthread_exit(NULL);

    // td 1.3.3: Destruction cancel
    pthread_cancel(tid);
    pthread_exit(NULL);
    return (0);


    return (0);

}
