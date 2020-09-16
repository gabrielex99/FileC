#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#define buffer 50
pthread_mutex_t mutexMain, mutexThread;
char stringa[buffer];
int time_to_exit=0;

void *thread_function(void *arg) {
    while (strncmp(stringa,"ESCO",4)!=0) {
        pthread_mutex_lock(&mutexThread);//FACCIO IL LOCK SUL MUTEX THREAD//
        printf("THREAD FIGLIO\n");
        printf("La stringa:");
        puts(stringa);
        pthread_mutex_unlock(&mutexMain);//SBLOCCO IL MUTEX PRINCIPALE//
    }
   time_to_exit=1;
   pthread_exit(0);
}
 
int main() {
    pthread_t t1;
    void *risultato;
    int res;
    pthread_mutex_init(&mutexMain, NULL);
    pthread_mutex_init(&mutexThread, NULL);//INIZIALIZZO I 2 MUTEX//
    pthread_mutex_lock(&mutexThread);//FACCIO IL LOCK DEL MUTEX THREAD//
    pthread_create(&t1, NULL, thread_function, NULL);//CREO IL THREAD//
      do{
        pthread_mutex_lock(&mutexMain);//FACCIO IL LOCK SUL MUTEX PRINCIPALE//
        printf("MAIN THREAD\n");
        printf("Inserisci stringa\n");
        fgets(stringa,buffer,stdin);
        pthread_mutex_unlock(&mutexThread);//SBLOCCO IL MUTEX DEL THREAD//
    }while(time_to_exit!=1);

    printf("ASPETTO CHE THREAD FINISCE\n");
 
    res = pthread_join(t1, &risultato); //FACCIO LA JOIN METTENDO IN ATTESA CHE FINISCE//
 
    if (res != 0)
    {
        perror("JOIN  FALLITA\n");
        exit(1);
    }
 
    res = pthread_mutex_destroy(&mutexMain); //DISTRUGGO IL MUTEX CHE NON SERVE PIU//
    res = pthread_mutex_destroy(&mutexThread);
    if (res != 0)
    {
        perror("DISTRUZIONE  FALLITA\n");
        exit(1);
    }
 
    printf("TUTTO ANDATO CORRETTAMENTE\n");
    exit(0);
}
