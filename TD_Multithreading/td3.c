#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_RESOURCE 4

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  
unsigned int resources= MAX_RESOURCE;
int tab[3] = {1,2,3}; // arguments de threads

/* fonction realisant le thread */
void get_resources(int amount) {

    // verrouillage du mutex de gestion des ressources
    if (pthread_mutex_lock(&mutex)) {
        printf( "erreur lock\n");
        exit(- 1);
    }         

    // attente losrque ressource < amount
    while(resources < amount){
        if(pthread_cond_wait(&cond, &mutex)){
            printf("erreur wait\n");
            exit(-1);
        }
    }

    // decrementation de resources
    resources -= amount;

    // deverrouillage
    if (pthread_mutex_unlock(&mutex)) {
        printf( "erreur unlock\n");
        exit(- 1);
    } 
}

void free_resources(int amount){

    // verrouillage du mutex de gestion des ressources
    if (pthread_mutex_lock(&mutex)) {
        printf( "erreur lock\n");
        exit(- 1);
    }         

    // incrementation de resources
    resources += amount;

    // signalisation si des threads sont en attente 
    if(pthread_cond_broadcast(&cond)){
        printf("erreur broadcast\n");
            exit(-1);
    }

    // deverrouillage
    if (pthread_mutex_unlock(&mutex)) {
        printf( "erreur unlock\n");
        exit(- 1);
    } 
}

void* fonction(void *arg){ 
    int res=*(int*)arg;
    printf("thread %d, je demande %d ressources\n",(int)pthread_self(),res);
    get_resources(res);
    printf("thread %d, j'ai %d ressources\n",(int)pthread_self(),res);
    sleep(1);
    printf("thread %d, je libere %d ressources\n",(int)pthread_self(),res);
    free_resources(res);
    pthread_exit(NULL);
    return NULL;
  }


int main() {
    pthread_t tidthread[2]; // les threads
    int i;

    printf(" programme creant 2 threads et manipulant une variable condition\n");

    for(i=0; i<2; i++){
        // creation du thread
        if(pthread_create(&tidthread[i], NULL, fonction, &tab[i])){
            printf("erreur creation thread\n");
            exit(1);
        }
    }

    fonction(&tab[2]);
    pthread_exit(NULL);
    return 0;
}