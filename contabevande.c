//INSERIAMO DA TASTIERA DELLE STRINGHE DI MARCHI DI BEVANDE ED IL PROGRAMMA PER OGNI VISUALIZZAZIONE VISUALIZZA IL NUMERO DELLA STESSA BEVANDA//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void *funzione(void*arg);

//VARIABILE CONDIVISE//
pthread_mutex_t mmutex,mthread;
char stringa[200];
int i,f,j=0;
int time_to_exit=0;

int main(void){
int res;//INTERO PER VEDERE SE LE FUNZIONI SONO CORRETTE//
void *risultato;//PER CHIAMARE LA JOIN//
pthread_t t1; //THREAD//
char stringa1[200];
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
//CREO IL THREAD//
res=pthread_create(&t1,NULL,funzione,NULL);
if(res!=0){
printf("ERRORE DURANTE LA CREAZIONE DEL THREAD\n");
exit(1);
}
pthread_mutex_lock(&mthread);
do{
pthread_mutex_lock(&mmutex);
if(time_to_exit==1){
pthread_mutex_unlock(&mthread);
break;
}
if(i!=0){
printf("IL NUMERO DI COCA INSERITE:%d\n",j);
printf("IL NUMERO DI FANTA INSERITE:%d\n",f);
}
printf("Inserisci una stringa\n");
fgets(stringa1,50,stdin);
strcpy(stringa,stringa1);
i++;
pthread_mutex_unlock(&mthread);
}while(time_to_exit!=1);
//CHIAMARE LA JOIN ED ELIMINO I MUTEX//
pthread_join(t1,&risultato);
pthread_mutex_destroy(&mmutex);
pthread_mutex_destroy(&mthread);
printf("TUTTO E ANDATO CORRETTAMENTE\n");
exit(0);
}
void *funzione(void*arg){
sleep(2);
do{
pthread_mutex_lock(&mthread);
if(i==6){
pthread_mutex_unlock(&mmutex);
break;
}
printf("E STATA INSERITA:%s\n",stringa);
if(strncmp(stringa,"coca",4)==0){
j++;
}
if(strncmp(stringa,"fanta",5)==0){
f++;
}
pthread_mutex_unlock(&mmutex);
}while(i<=5);
time_to_exit=1;
pthread_exit(0);
}
