//PERMETTE DI INSERIRE DEI NUMERI SE TALE NUMERO E PARI FA LA MOLTIPLICAZIONE PER UNA COSTANTE SE INVECE TALE NUMERO E DISPARI LO DIVIDE PER TALE COSTANTE E RESTITUISCE IL RISULTATO AL THREAD PRINCIPALE//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *funzione(void *arg);

//VARIABILI CONDIVISE//
int a[50];
pthread_mutex_t mutex,thread;
int i=0;
int time_to_exit=0;

int main(void){
int res;
int b[50];
void *risultato;
pthread_t t1;

//1 INIZIALIZZO I MUTEX//
res=pthread_mutex_init(&mutex,NULL);
if(res!=0){
perror("ERRORE DURANTE L INIZIALIZZAZIONE\n");
exit(1);
}
res=pthread_mutex_init(&mutex,NULL);
if(res!=0){
perror("ERRORE DURANTE L INIZIALIZZAZIONE\n");
exit(1);
}
pthread_create(&t1,NULL,funzione,NULL);
pthread_mutex_lock(&thread);

do{
pthread_mutex_lock(&mutex);
if(time_to_exit==1){
pthread_mutex_unlock(&thread);
break;
}
if(i!=0){
printf("IL VALORE MODIFICATO:%d\n",a[i-1]);
}
printf("Inserisci un numero\n");
scanf("%d",&b[i]);
a[i]=b[i];
pthread_mutex_unlock(&thread);
}while(time_to_exit!=1); //FACCIO LA JOIN ED ELIMINO I MUTEX E FINISCO//
printf("ASPETTO TERMINAZIONE DEL ALTRO THREAD\n");
pthread_join(t1,&risultato);
pthread_mutex_destroy(&mutex);
pthread_mutex_destroy(&thread);
printf("TUTTO ANDATO CORRETTAMENTE\n");
exit(0);
}

void *funzione(void *arg){
do{
pthread_mutex_lock(&thread);
if(i==5){
pthread_mutex_unlock(&mutex);
break;
}
printf("IL VALORE INSERITO:%d\n",a[i]);
if(a[i]%2==0){
printf("NUMERO PARI\n");
a[i]=a[i]*2;
}
else{
a[i]=a[i]/2;
}
i++;
pthread_mutex_unlock(&mutex);
}while(1);
time_to_exit=1;
pthread_exit(0);
}
