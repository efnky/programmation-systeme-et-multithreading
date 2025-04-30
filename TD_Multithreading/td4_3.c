#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_key_t key;
pthread_once_t once = PTHREAD_ONCE_INIT;

void fonction_init(){
    if (pthread_key_create(&key, NULL) != 0){
        printf("erreur creation key\n");
        exit(-1);
    }
    printf("initialisation de la cle \n");
}

int increm(){
    int *globalcpt;
    if (!(globalcpt = (int *)pthread_getspecific(key))){
        printf("erreur zone mémoire NULL\n");
        exit(-1);
    }
    return ((*globalcpt)++);
}

void* fonction(void *arg){
    int i;
    int *globale;
    if (pthread_once(&once, fonction_init)){
        printf("erreur initialisation\n");
        exit(-1);
    }
    globale = malloc(sizeof(int));
    if (!globale){
        printf("erreur allocation mémoire\n");
        exit(-1);
    }
    *globale = 0; // Initialize to 0 instead of thread ID
    if (pthread_setspecific(key, (void *)globale)){
        printf("erreur accrochage memoire -cle\n");
        free(globale); // Free memory if setspecific fails
        exit(-1);
    }
    for (i = 0; i < 5; i++){
        printf("thread %d, valeur du compteur: %d\n", (int)pthread_self(), increm());
        sleep(1);
    }
    free(globale); // Free memory before thread exits
    pthread_exit(NULL);
    return NULL;
}

int main(){
    int i;
    pthread_t tidthread[2];
    printf("programme manipulant once et key\n");
    for (i = 0; i < 2; i++){
        if (pthread_create(&tidthread[i], NULL, fonction, NULL)){
            printf("erreur creation thread\n");
            exit(-1);
        }
    }
    pthread_exit(NULL);
    return 0;
}
