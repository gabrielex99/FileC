//INSERISCI UN ARRAY DI INTERI DA TASTIERA NEL MAIN THREAD,IL THREAD FIGLIO RICEVE E SOMMA A TALE ARRAY UN NUOVO ARRAY INSERITO DA TASTIERA IL MAIN THREAD VISUALIZZA IL RISULTATO DEL OPERAZIONE//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

int a[5]={0,0,0,0,0};
int time_to_exit=0;
int i=0;
pthread_mutex_t mmain,mthread;
void *funzione(void *arg);

int main(void){
void *risultato;
int res;
pthread_t t1;

res=pthread_mutex_init(&mmain,NULL);
if(res!=0){
perror("INIZIALIZZAZIONE MUTEX NON RIUSCITA\n");
exit(1);
}
res=pthread_mutex_init(&mmain,NULL);
if(res!=0){
perror("INIZIALIZZAZIONE MUTEX NON RIUSCITA\n");
exit(1);
}
pthread_create(&t1,NULL,funzione,NULL);
pthread_mutex_lock(&mthread);
do{
pthread_mutex_lock(&mmain);
if(time_to_exit==1){
break;
}

if(i!=0){
printf("IL NUOVO VALORE:%d\n",a[i-1]);
}
printf("Inserisci un valore\n");
scanf("%d",&a[i]);
pthread_mutex_unlock(&mthread);
}while(time_to_exit!=1);
printf("ASPETTO TERMINAZIONE DEL ALTRO THREAD\n");
pthread_join(t1,&risultato);
pthread_mutex_destroy(&mthread);
pthread_mutex_destroy(&mmain);
printf("TUTTO E ANDATO CORRETTAMENTE\n");
exit(0);
}
void *funzione(void *arg){
int b[5];
do{
pthread_mutex_lock(&mthread);
if(a[i]==0){
pthread_mutex_unlock(&mmain);
break;
}
printf("Inserisci un numero\n");
scanf("%d",&b[i]);
printf("HO LETTO IL SEGUENTE VALORE:%d\n",a[i]);
a[i]=a[i]+b[i];
i++;
pthread_mutex_unlock(&mmain);
}while(1);
time_to_exit=1;
pthread_exit(0);
}
