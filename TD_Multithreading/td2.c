
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  
float *px = NULL;
const int max = 5;
float x = 1;

/* fonction realisant le thread */
void *fonction(void *max) {
    int  i;
    for (i = 0; i < *(int *)max; i++) {
        if (pthread_mutex_lock(&mutex)) {
            printf( "erreur lock\n");
            exit(- 1);
        } 

        printf( "t: x=%f\n", *px);

        if (pthread_mutex_unlock(&mutex)) {
            printf( "erreur unlock\n");
            exit(- 1);
        } 
    } 

    pthread_exit( NULL);
    return NULL;
}


int main() {
    int  i;
    pthread_t thread; 
    printf( " programme creant 2 threads et manipulant un mutex\n");

    //initialisation du pointeur sur une adresse valide
    px = &x;

    // creation du thread 
    if (pthread_create(&thread, NULL, fonction, (void *)&max)) {
    printf( "erreur create\n");
    exit(- 1);
    } 

    for (i = 0; i < max; i++) {
        if (pthread_mutex_lock(&mutex)) {
            printf( "erreur lock\n");
            exit(- 1);
        } 

        px = NULL;
        printf( "i=%d\n", i);
        px = &x; 

        if (pthread_mutex_unlock(&mutex)) {
            printf( "erreur unlock\n");
            exit(- 1);
        } 
    } 

    pthread_exit( NULL);
    return 0;
}