#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
int a[5];
int b[5];
void*funzione(void *arg);
pthread_mutex_t mmutex,tmutex;
int main(void){
int res;
int i;
pthread_t t1;
void *risultato;
res=pthread_mutex_init(&mmutex,NULL);//INIALIZZO I MUTEX//
res=pthread_mutex_init(&tmutex,NULL);//INIALIZZO I MUTEX//
if(res!=0){
perror("INIZIALIZZAZIONE ERRATA\n");
exit(1);
}
pthread_mutex_lock(&tmutex);//BLOCCO IL THREAD MUTEX COSI LUI NON ESEGUE//
res=pthread_create(&t1,NULL,funzione,NULL);//CREO IL THREAD//
if(res!=0){
perror("CREAZIONE ERRATA\n");
exit(1);
}
for(i=0;i<5;i++){
pthread_mutex_lock(&mmutex);//COSI INSERISCO IL NUMERO BLOCCANDO LA RISORSA//
printf("Inserisci numero\n");
scanf("%d",&a[i]);
printf("Inserisci numero\n");
scanf("%d",&b[i]);
pthread_mutex_unlock(&tmutex);//RILASHIO IL MUTEX THREAD,COSI PARTE IL THREAD//
}
res=pthread_join(t1,&risultato);//FACCIO LA JOIN//
pthread_mutex_destroy(&mmutex);//DISTRUGGO I MUTEX//
pthread_mutex_destroy(&tmutex);
printf("TUTTO E ANDATO CORRETTAMENTE\n");
exit(0);
}
void*funzione(void *arg){
sleep(2);//COSI NON PARTE PRIMA//
int i;
int c=0;
for(i=0;i<5;i++){
pthread_mutex_lock(&tmutex);//BLOCCO IL THREAD DEL MUTEX COSI PUO LAVORARE//
c=a[i]+b[i];
printf("IL NUMERO INSERITO:%d\n",c);
pthread_mutex_unlock(&mmutex);//RILASCIO IL MUTEX DEL MAIN PERCHE E FINITO IL PROGRAMMA//
}
pthread_exit(0);
}
