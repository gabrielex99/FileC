#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void *funzione(void *arg);
pthread_mutex_t mmutex,mthread;
int time_to_exit=0;
char stringa[50];
int i=0;

int main(void){
pthread_t t1;
int res;
char stringa1[50];
void *risultato;
//1 INIZIALIZZO I MUTEX//
res=pthread_mutex_init(&mmutex,NULL);
if(res!=0){
perror("ERRORE DURANTE INIZIALIZZAZIONE\n");
exit(1);
}
res=pthread_mutex_init(&mthread,NULL);
if(res!=0){
perror("ERRORE DURANTE INIZIALIZZAZIONE\n");
exit(1);
}
//2  CREO IL THREAD//
res=pthread_create(&t1,NULL,funzione,NULL);
if(res!=0){
perror("ERRORE DURANTE INIZIALIZZAZIONE\n");
exit(1);
}
//3 FACCIO IL LOCK//
pthread_mutex_lock(&mthread);
//4 VADO CON IL CICLO//
do{
pthread_mutex_lock(&mmutex);
if(time_to_exit==1){
break;
}
if(i!=0){
printf("LA STRINGA MODIFICATA:%s\n",stringa);
}
printf("Inserisci una stringa\n");
fgets(stringa1,50,stdin);
strcpy(stringa,stringa1);
i++;
pthread_mutex_unlock(&mthread);
}while(time_to_exit!=1);
printf("ASPETTO TERMINAZIONE DEL THREAD\n");
res=pthread_join(t1,&risultato);
if(res!=0){
perror("JOIN NON RIUSCITA\n");
exit(1);
}
pthread_mutex_destroy(&mmutex);
pthread_mutex_destroy(&mthread);
printf("TUTTO E ANDATO CORRETTAMENTE\n");
}

void *funzione(void *arg){
int c=0; //METTO LE VARIABILI CHE MI SERVONO//
do{//VADO CON IL CICLO//
pthread_mutex_lock(&mthread);
c=strlen(stringa)-1;
if(strncmp(stringa,"ESCI",4)==0){
pthread_mutex_unlock(&mmutex);
break;
}
printf("HO LETTO LA SEGUENTE STRINGA:%s\n",stringa);
printf("IL NUMERO DI CARATTERI LETTI:%d\n",c);
strcat(stringa,"DIO");
pthread_mutex_unlock(&mmutex);
}while(1);
time_to_exit=1;
pthread_exit(0);
}
