#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include <string.h>

pthread_mutex_t mmt, mg1, mg2;//3 MUTEX,UNO THREAD PRINCIPALE E ALTRI 2 PER GLI ALTRI THREAD//
void *thread_giocatori(void *args);//FUNZIONE ESEGUITA DAI THREAD//
int valore=0;//VARIABILE CONDIVISA A CUI I THREAD ACCEDONO E LA MODIFICANO//
int a;
int main(){
	pthread_t g1,g2;//2 THREAD//
	int turno=1;
	int num;
	
	/*inizializzo i mutex del main, giocatore1 giocatore2*/
	pthread_mutex_init(&mmt,NULL);
	pthread_mutex_init(&mg1,NULL);
	pthread_mutex_init(&mg2,NULL);
	
	/*lock dei due giocatori così parte il thread del main*/
	pthread_mutex_lock(&mg1);
	pthread_mutex_lock(&mg2);
	
	/*creo i giocatori*/
	num=1;
	pthread_create(&g1, NULL, thread_giocatori, (void *)num);
	num=2;
	pthread_create(&g2, NULL, thread_giocatori, (void *)num);
	
	while(1){
		pthread_mutex_lock(&mmt);//BLOCCO IL MUTEX MAIN THREAD/
		if(valore==1){
                 printf("HA VINTO IL GIOCATORE 1\n");
                 break;
                }
                if(valore==2){
                 printf("Ha vinto il giocatore 2\n");
                 break;
                 }
                 printf("Inserisci una temperatura\n");
                 scanf("%d",&a);
              if(turno==1){//SE SIAMO NEL TURNO 1 MA CHE HA GIA ESEGUITO//
			pthread_mutex_unlock(&mg1);//RILASCIAMO IL MUTEX//
			turno=2;//PASSIAMO LA PALLA AL GIOCATORE 2//
		}
		else{
			pthread_mutex_unlock(&mg2);
			turno=1;
		}
	}
    //FACCIO LA CANCEL//
     pthread_cancel(g1);
     pthread_cancel(g2);
    //SBLOCCO I MUTEX//
      pthread_mutex_unlock(&mg1);
      pthread_mutex_unlock(&mg2);
    //FACCIO LA JOIN//
    pthread_join(g1,NULL);
    pthread_join(g2,NULL);
     //ELIMINO I MUTEX//
     pthread_mutex_destroy(&mg1);
     pthread_mutex_destroy(&mg2);
     pthread_mutex_destroy(&mmt);
     printf("TUTTO ANDATO CORRETTAMENTE\n");
     exit(0);
}



void *thread_giocatori(void *args){
	int g=(int)args; /*cast del giocatore in int*/
        int sym;
        int lun;
	pthread_mutex_t *mymutex; /*puntatore su cui salverò gli insidirizzi dei giocatori*/
	if(g==1){
                sym=1;
		mymutex=&mg1;
	}
	else{   
                sym=2;
		mymutex=&mg2;
	}

	while(1){
		pthread_mutex_lock(mymutex); /*con puntatore ma mymutex è già puntatore E FACCIO IL LOCK DEL MUTEX CHE ESEGUE*/
        if((valore==1)||(valore==2)){
              pthread_mutex_unlock(&mmt);
              break;
                }
		if(g==1){
			printf("Thread 1\n");
		}
		else{
			printf("Thread 2\n");
		}
		       //FUNZIONE DA ESEGUIRE//
                        printf("Hai inserito la seguente temperatura:%d\n",a);
                        if(a>=20){
                        valore=sym;
                        break;
                        }
                        pthread_mutex_unlock(&mmt);
	}
                        pthread_mutex_unlock(&mmt);
pthread_exit(0);
}
