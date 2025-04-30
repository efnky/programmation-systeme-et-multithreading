#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 5

pthread_once_t once = PTHREAD_ONCE_INIT;

void fonction_init(){
    printf("je suis executé\n");
}

void * fonction(){
    printf("dans le thread tid: %d\n",
    (int)pthread_self());
    pthread_once(&once,fonction_init);
    pthread_exit(NULL);
    return(NULL);
}

int main(){

    pthread_t tidthread[2]; // les threads

    for (int i = 0; i < 2; i++){
        if(pthread_create(&tidthread[i], NULL, fonction, NULL)){
            printf("erreur creation thread\n");
            exit(1);
        }
        pthread_join(tidthread[i],NULL);

        for(int i = 0; i < N; i++){
            printf("%d\n", i);
            sleep(1);
        }
    }

    fonction_init();
    return 0;
}