#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>

int a[5]={1,1,1,1,1};
pthread_mutex_t thread;
pthread_mutex_t main1;
int time_to_exit=0;
int i=0;
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
                if(time_to_exit==1){
                    break;
                }
		printf("Inserisci un numero: ");
		scanf("%d",&a[i]);
		pthread_mutex_unlock(&thread);
	}while(time_to_exit!=1);
        res=pthread_join(t, &risultato);
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
       printf("TUTTO E ANDATO CORRETTAMENTE\n");
	exit(0);
}

void *worker_thread(void *args){
        int c;
	do{
		pthread_mutex_lock(&thread);
                c=a[i];
                if(c==0){
                break;
                }
                printf("IL numero letto: %d\n",a[i]);
		a[i]=a[i]+2;
		printf("Il nuovo numero è: %d\n",a[i]);
                i++;
		pthread_mutex_unlock(&main1);
	}while(i<5);//IN TAL MODO SODDISFO 2 CONDIZIONI OVVERO SIA SE MINORE DI 5 OPPURE SE E VERICATA UNA DATA CONDIZIONE
     time_to_exit=1;
     pthread_mutex_unlock(&main1);
     pthread_exit(0);
}
