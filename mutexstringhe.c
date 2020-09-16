#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
void *funzione(void *arg);
//VARIABILE CONDIVISE//
pthread_mutex_t mmutex,tmutex;
char stringa[50];
int time_to_exit=0;
int i=0;

int main(void){
pthread_t t1;
int res;
char str[50];
//1 INIZIALLIZZO I 2 MUTEX//
res=pthread_mutex_init(&mmutex,NULL);
if(res!=0){
perror("ERRORE DURANTE INIZIALIZZAZIONE\n");
exit(1);
}
res=pthread_mutex_init(&tmutex,NULL);
if(res!=0){
perror("ERRORE DURANTE INIZIALIZZAZIONE\n");
exit(1);
}
//2 CREO IL THREAD//
pthread_create(&t1,NULL,funzione,NULL);
//3 BLOCCO IL MUTEX//
pthread_mutex_lock(&tmutex);
//4 FACCIO IL CICLO DEL THREAD PRINCIPALE//
do{
pthread_mutex_lock(&mmutex);
if(time_to_exit==1){
pthread_mutex_unlock(&tmutex);
break;
}
if(i!=0){
printf("La stringa modificata:%s\n",stringa);
}
printf("Inserisci una stringa\n");
fgets(str,50,stdin);
strcpy(stringa,str);
pthread_mutex_unlock(&tmutex);
}while(time_to_exit!=1);

pthread_join(t1,NULL);
pthread_mutex_destroy(&tmutex);
pthread_mutex_destroy(&mmutex);
printf("TUTTO E ANDATO CORRETTAMENTE\n");
exit(0);
}

void *funzione(void *arg){
int lun;
do{
pthread_mutex_lock(&tmutex);
if(strncmp(stringa,"ESCI",4)==0){
pthread_mutex_unlock(&mmutex);
break;
}
stringa[strlen(stringa)-1]='\0';
lun=strlen(stringa);
printf("Hai inserito la seguente stringa:%s\n",stringa);
printf("LA TUA STRINGA E LUNGA:%d\n",lun);
strcat(stringa,"dio");
i++;
pthread_mutex_unlock(&mmutex);
}while(1);
time_to_exit=1;
pthread_exit(0);
}
