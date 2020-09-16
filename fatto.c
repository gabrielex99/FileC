#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
//VARIABILE CONDIVISA//
int valore;
int a;
pthread_mutex_t mmain,mstanza1,mstanza2;
void *funzione(void *arg);
int main(void){
pthread_t t1;
pthread_t t2;
int res;
int turno=1;
//1 INIZIALIZZO I MUTEX//
pthread_mutex_init(&mmain,NULL);
pthread_mutex_init(&mstanza1,NULL);
pthread_mutex_init(&mstanza2,NULL);
//2 LOCKO I MUTEX//
pthread_mutex_lock(&mstanza1);
pthread_mutex_lock(&mstanza2);
//3 CREO I 2 THREAD//
pthread_create(&t1,NULL,funzione,(void*)1);
pthread_create(&t2,NULL,funzione,(void*)2);

while(1){
pthread_mutex_lock(&mmain);
if(valore==1){
printf("Ha vinto il giocatore 1\n");
break;
}
if(valore==2){
printf("Ha vinto il giocatore 2\n");
break;
}
printf("Inserisci un valore\n");
scanf("%d",&a);

//FACCIO I TURNI//
if(turno==1){
pthread_mutex_unlock(&mstanza1);
turno=2;
}
else{
pthread_mutex_unlock(&mstanza2);
turno=1;
}
}

//FACCIO LA CANCEL//
pthread_cancel(t1);
pthread_cancel(t2);
//RILASCIO I MUTEX//
pthread_mutex_unlock(&mstanza1);
pthread_mutex_unlock(&mstanza2);
//FACCIO LA JOIN//
pthread_join(t1,NULL);
pthread_join(t2,NULL);
//ELIMINO I MUTEX//
pthread_mutex_destroy(&mstanza1);
pthread_mutex_destroy(&mstanza2);
pthread_mutex_destroy(&mmain);
}

void *funzione(void *arg){
int tim=(int)arg;
int sim;
pthread_mutex_t *mymutex;
if(tim==1){
mymutex=&mstanza1;
sim=1;
}

if(tim==2){
mymutex=&mstanza2;
sim=2;
}

while(1){
pthread_mutex_lock(mymutex);
      if((valore==1)||(valore==2))
              break;
if(tim==1){
printf("Thread 1\n");
}
if(tim==2){
printf("Thread 2\n");
}
printf("IL VALORE:%d\n",a);
if(a>20){
   valore=sim;
   break;
}
pthread_mutex_unlock(&mmain);
}
pthread_mutex_unlock(&mmain);
pthread_exit(0);
}
