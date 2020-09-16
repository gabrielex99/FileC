#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

char campo[3][3]={{' ',' ',' '},{' ',' ',' '},{' ',' ',' '}};//IL CAMPO//
pthread_mutex_t mmt, mg1, mg2;//3 MUTEX,UNO THREAD PRINCIPALE E ALTRI 2 PER GLI ALTRI THREAD//
void *thread_giocatori(void *args);//FUNZIONE ESEGUITA DAI THREAD//
char controlla_vittoria();
void stampa_campo();

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
		pthread_mutex_lock(&mmt);//BLOCCO IL MUTEX MAIN THREAD//	
		stampa_campo();
		char c=controlla_vittoria();
		if(c=='X' || c=='O'){
			printf("Ha vinto il giocatore %c\n",c);
			break;
		}
		else if(c=='p'){
			printf("Parità\n");
			break;
		}
		
		if(turno==1){//SE SIAMO NEL TURNO 1 MA CHE HA GIA ESEGUITO//
			pthread_mutex_unlock(&mg1);//RILASCIAMO IL MUTEX//
			turno=2;//PASSIAMO LA PALLA AL GIOCATORE 2//
		}
		else{
			pthread_mutex_unlock(&mg2);
			turno=1;
		}
	}


        
	
	/*cancello i thread*/
	fflush(stdout);
	pthread_cancel(g1);
	pthread_cancel(g2);
	
        //FACCIO L'UNLOCK DEI MUTEX //
        pthread_mutex_unlock(&mg1);
	pthread_mutex_unlock(&mg2);
	
	/*attendo la cancellazione*/
	pthread_join(g1, NULL);
	pthread_join(g2, NULL);
    
       /*Distruggo< i mutex*/
	pthread_mutex_destroy(&mmt);
	pthread_mutex_destroy(&mg1);
	pthread_mutex_destroy(&mg2);

	printf("Gioco finito\n");
     exit(0);
}

void stampa_campo(){
        //STAMPO LA PRIMA RIGA//
	printf("%c | %c | %c\n",campo[0][0],campo[0][1],campo[0][2]);
	printf("---------\n");
        //STAMPO LA SECONDA RIGA//
	printf("%c | %c | %c\n",campo[1][0],campo[1][1],campo[1][2]);
	printf("---------\n");
        //STAMPO LA TERZA RIGA//
	printf("%c | %c | %c\n",campo[2][0],campo[2][1],campo[2][2]);
}

char controlla_vittoria(){
	int r;
	int c;
	/*controllo vincita righe*/
	for(r=0;r<3;r++)
		if(campo[r][0]==campo[r][1] && campo[r][0]==campo[r][2] && campo[r][0]!=' ')
			return campo[r][0];
	/*controllo vincita colonne*/
	for(c=0;c<3;c++)
		if(campo[0][c]==campo[1][c] && campo[0][c]==campo[2][c] && campo[0][c]!=' ')
			return campo[0][c];
	/*controllo vincita diagonale1*/
	if(campo[0][0]==campo[1][1] && campo[1][1]==campo[2][2] && campo[0][0]!=' ')
			return campo[0][0];
	/*controllo vincita diagonale2*/
	if(campo[0][2]==campo[1][1] && campo[0][2]==campo[2][0] && campo[0][2]!=' ')
			return campo[0][2];
	/*controllo caselle vuote*/
	for(r=0;r<3;r++)
		for(c=0;c<3;c++)
			if(campo[r][c]==' ')
				return ' ';
	/*allora parità*/
	return 'p';	
}

void *thread_giocatori(void *args){
	int g=(int)args; /*cast del giocatore in int*/
	char sym; /*simbolo*/
	int r; /*righe*/
	int c; /*colonne*/
	pthread_mutex_t *mymutex; /*puntatore su cui salverò gli insidirizzi dei giocatori*/
	if(g==1){
		sym='X';
		mymutex=&mg1;
	}
	else{
		sym='O';
		mymutex=&mg2;
	}
	
	while(1){
		pthread_mutex_lock(mymutex); /*con puntatore ma mymutex è già puntatore E FACCIO IL LOCK DEL MUTEX CHE ESEGUE*/
		if(g==1){
			printf("Giocatore 1\n");
		}
		else{
			printf("Giocatore 2\n");
		}
		
		do{
			/*inserisco riga e colonna e fflush*/
			printf("Inserisci riga: ");
			scanf("%d",&r);
			printf("Inserisci colonna: ");
			scanf("%d",&c);
			if(r<0 || r>2 || c<0 || c>2 || campo[r][c]!=' ')
      				printf("non va bene\n");			
		}while(r<0 || r>2 || c<0 || c>2 || campo[r][c]!=' ');
		campo[r][c]=sym;
		pthread_mutex_unlock(&mmt);
	}
}
