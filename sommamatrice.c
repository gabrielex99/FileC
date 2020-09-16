/*
Ho una variabile condivisa di tipo stringa
inserisco una stringa da tastiera
il thread calcola la lunghezza della stringa
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>

int a[2][2];
pthread_mutex_t thread;
pthread_mutex_t main1;
void *worker_thread(void *args);

int main(){
	pthread_t t;
	int res;
	void*risultato;
	int i,j;
	
	res=pthread_mutex_init(&thread,NULL);
	if(res==-1){
		perror("Errore mutex thread");
		exit(1);
	}
	res=pthread_mutex_init(&main1,NULL);
	if(res==-1){
		perror("Errore mutex main");
		exit(1);
	}
	
	pthread_mutex_lock(&thread);
	
	res=pthread_create(&t,NULL,worker_thread,NULL);
	if(res==-1){
		perror("Errore thread");
		exit(1);
	}
	
	for(i=0;i<2;i++){
		pthread_mutex_lock(&main1);
		for(j=0;j<2;j++){
			printf("Inserisci un numero: ");
			scanf("%d",&a[i][j]);
		}
		pthread_mutex_unlock(&thread);
	}
	
	res=pthread_join(t, &risultato);
	if(res==-1){
		perror("Errore join");
		exit(1);
	}
	
	res=pthread_mutex_destroy(&thread);
	if(res==-1){
		perror("Errore distruzione mutex thread");
		exit(1);
	}
	res=pthread_mutex_destroy(&main1);
	if(res==-1){
		perror("Errore distruzione mutex main");
		exit(1);
	}
	printf("ciaone\n");
	exit(0);
}

void *worker_thread(void *args){
	sleep(2);
	int i,j;
	int somma=0;
	
	for(i=0;i<2;i++){
		pthread_mutex_lock(&thread);
		for(j=0;j<2;j++){
			somma=somma+a[i][j];
		}
		printf("La somma della riga %d è: %d\n",i,somma);
		somma=0;
		pthread_mutex_unlock(&main1);
	}
	
	pthread_exit(0);
}
