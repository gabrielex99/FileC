//IL MAIN THREAD MANDA UNA STRINGA,AL THREAD FIGLIO,ESSO PRENDE TALE STRINGA,LA LEGGE STAMPA LA PROPRIA LUNGHEZZA ED IN BASE ALLA STRINGA,SE INSERISCO TERMINA IL PROGRAMMA FINISCE//IN BASE ALLA STRINGA INSERITA  ESEGUE UN DIVERSO COMANDO//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
void *funzione(void *arg);
//VARIABILI CONDIVISE//
pthread_mutex_t mmutex,tmutex;
char stringa[200];
int time_to_exit=0;
int i=0;
int main(void){
pthread_t t1;
int res;
char stringa1[200];
//1 Inizializzo il mutex//
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
//3 BLOCCO MUTEX//
pthread_mutex_lock(&tmutex);
do{
pthread_mutex_lock(&mmutex);
if(strncmp(stringa,"TERMINA",7)==0){
pthread_mutex_unlock(&tmutex);
break;
}
if(strncmp(stringa,"COPIA",5)==0){
printf("La stringa modificata:%s\n",stringa);
}
if(i!=0 && strncmp(stringa,"CONCATENA",7)==0){
printf("La stringa modificata:%s\n",stringa);
}
printf("Inserisci una stringa\n");
fgets(stringa1,200,stdin);
strcpy(stringa,stringa1);
i++;
pthread_mutex_unlock(&tmutex);
}while(time_to_exit!=1);
//ELIMINO I 2 MUTEX,FACCIO LA JOIN E FINISCO//
pthread_join(t1,NULL);
pthread_mutex_destroy(&tmutex);
pthread_mutex_destroy(&mmutex);
printf("TUTTO E ANDATO CORRETTAMENTE\n");
exit(0);
}

void *funzione(void *arg){
int lun=0;
char s1[200];
char s2[200];
do{
pthread_mutex_lock(&tmutex);
if(strncmp(stringa,"TERMINA",7)==0){
pthread_mutex_unlock(&mmutex);
break;
}
stringa[strlen(stringa)-1]='\0';
lun=strlen(stringa);
printf("Hai inserito la seguente stringa:%s\n",stringa);
printf("LA SUA LUNGHEZZA:%d\n",lun);
if(strncmp(stringa,"COPIA",5)==0){
printf("Inserisci la stringa da copiare\n");
fgets(s1,200,stdin);
strcpy(stringa,s1);
}
if(strncmp(stringa,"CONCATENA",3)==0){
printf("Inserisci la stringa da concatenare\n");
fgets(s2,200,stdin);
strcat(stringa,s2);
}
pthread_mutex_unlock(&mmutex);
}while(1);
time_to_exit=1;
pthread_exit(0);
}
