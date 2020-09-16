#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *funzione(void *arg);
//VARIABILI CONDIVISE//
pthread_mutex_t mmutex,tmutex;
int valore[50];
int time_to_exit=0;
int i=0;
int main(void){
pthread_t t1;
int res;
int a[50];
//1 INIZIALIZZO IL MUTEX//
res=pthread_mutex_init(&mmutex,NULL);
if(res!=0){
perror("ERRORE DURANTE L'INIZIALIZZAZIONE\n");
exit(1);
}
res=pthread_mutex_init(&tmutex,NULL);
if(res!=0){
perror("ERRORE DURANTE L'INIZIALIZZAZIONE\n");
exit(1);
}
//2 CREO IL THREAD//
pthread_create(&t1,NULL,funzione,NULL);
//3 BLOCCO IL MUTEX2//
pthread_mutex_lock(&tmutex);
do{
pthread_mutex_lock(&mmutex);
if(*valore==5){
pthread_mutex_unlock(&tmutex);
break;
}
printf("Inserisci un valore\n");
scanf("%d",&a[i]);
*valore=a[i];
i++;
pthread_mutex_unlock(&tmutex);
}while(time_to_exit!=1);
//ELIMINO I MUTEX E TERMINO//
pthread_join(t1,NULL);
pthread_mutex_destroy(&tmutex);
pthread_mutex_destroy(&mmutex);
printf("Tutto e andato correttamente\n");
exit(0);
}

void *funzione(void *arg){
int max=0;
do{
pthread_mutex_lock(&tmutex);
if(*valore==5){
pthread_mutex_unlock(&mmutex);
break;
}
printf("Hai inserito:%d\n",*valore);
if(*valore>max){
max=*valore;
}
*valore=max;
printf("IL massimo:%d\n",*valore);
pthread_mutex_unlock(&mmutex);
}while(1);
time_to_exit=1;
pthread_exit(0);
}
