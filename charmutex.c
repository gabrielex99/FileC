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

char stringa[50];
int time_to_exit=0;
pthread_mutex_t thread;
pthread_mutex_t main1;
void *worker_thread(void *args);

int main(){
	pthread_t t;
	int res;
	void*risultato;
	
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
	
	do{	
		pthread_mutex_lock(&main1);
		if(time_to_exit==1)
			break;
		printf("Inserisci una stringa: ");
		fgets(stringa,50,stdin);
		pthread_mutex_unlock(&thread);
	}while(time_to_exit!=1);
	
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
	printf("TUTTO ANDATO CORRETTAMENTE\n");
	exit(0);
}

void *worker_thread(void *args){
	int len;
	sleep(2);
	do{
		pthread_mutex_lock(&thread);
                if(strncmp(stringa,"ESCI",4)==0){
               break;
               }
		stringa[strlen(stringa)-1]='\0';
		len=strlen(stringa);
		printf("La lunghezza di %s è %d\n",stringa,len);
		pthread_mutex_unlock(&main1);
	}while(strncmp(stringa,"ESCI",4)!=0);
        pthread_mutex_unlock(&main1);
        printf("MATTIA HA IL CAZZO PICCOLO\n");
	time_to_exit=1;
	pthread_exit(0);
}
