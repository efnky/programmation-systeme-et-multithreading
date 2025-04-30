#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#define MAX 6

void handler_nettoyage(void * mut){
    printf("nettoyage pour thread %lu\n", pthread_self());
    if(pthread_mutex_unlock((pthread_mutex_t*)mut)){
        printf("erreur unlock\n");
        exit(-1);
    }
}

void *fonction(){

    if(pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL)){
        printf("erreur desactivation cancel\n");
        exit(-1);
    }

    if (pthread_mutex_lock(&mutex)){
        printf("erreur lock\n");
        exit(-1);
    }

    pthread_cleanup_push(handler_nettoyage,&mutex);
    if(pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)){
        printf("erreur activation cancel\n");
        exit(-1);
    }

    printf("j'ai le mutex\n");

    for (int i = 0; i < MAX; i++){
        printf("tid: %lu\n", pthread_self());
        sleep(1);
    }
    
    pthread_cleanup_pop(1);
    return NULL;
}

int main(){

    pthread_t tid[2];

    for(int i = 0; i < 2; i++){
        if(pthread_create(&tid[i], NULL,fonction ,NULL)){
            printf("erreur creation thread\n");
            exit(-1);
        }

        for(int j = 0; j< MAX/3; j++){
            printf("tid: %lu\n", pthread_self());
            sleep(1);
        }
        pthread_cancel(tid[i]);
        sleep(1);
    }
}