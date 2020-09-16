/*netstat -tulpn | grep 7890*/
//SE CI SONO PROBLEMI METTERE QUESTO COMANDO//
//QUESTO E IL SERVER//
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define STRUDIM 50
#define STRMDIM 140
#define PORTA 1789

//OGNI MESSAGGIO HA UN ID,UN NOME DI CHI LO HA INVIATO,ED IL MESSAGGIO//

struct messaggio{
	int id;
	char nome[STRUDIM];
	char messaggio[STRUDIM];
};

//VARIABILI CONDIVISE//
struct messaggio ultimomessaggio;
pthread_mutex_t mutex;
void *worker_thread(void *args);


int main(){
	int serversock;//ID PRIMO SOCKET//
	int clientsock;//ID SOCKET DOPO L'ACCEPT//
	struct sockaddr_in saddr;//STRUTTURA DEGLI INDIRIZZI//
	pthread_t wtid;
	//INIZIALIZZO IL MUTEX//
	pthread_mutex_init(&mutex, NULL);
	//CREO IL SOCKET STEP 2//
	serversock=socket(AF_INET, SOCK_STREAM, 0); //CREO IL SOCKET//
	if(serversock==-1){
		perror("Errore serversock");
		exit(1);
	}
        //SETTO STRUCT DEGLI INDIRIZZI STEP 3 //
	//RICORDARE A MEMORIA//
	saddr.sin_family=AF_INET;//FAMIGLIA DI SOCKET//
	saddr.sin_port=htons(PORTA);//CONVERSIONE HOST TO NETWORK,SPECIFICO LA PORTA//
	saddr.sin_addr.s_addr=htonl(INADDR_ANY);//CONVERSIONE HOST TO NETWORK DA TUTTI GLI INDIRIZZI ACCETTO CONNESSIONI//
	
	bzero(&ultimomessaggio, sizeof(ultimomessaggio));//INIZIALIZZO A 0 UNA STRINGA//
	//FACCIO LA BIND COSI SPONSORIZZO IL SERVER STEP 4//
	if(bind(serversock, (struct sockaddr *)&saddr, sizeof(saddr))==-1){
		perror("Errore bind");//FACCIO LA BIND//
		exit(1);
	}
	//METTO IL SERVER IN LISTEN//
	listen(serversock, 5);
	
	while(1){//AL INFINITO ACCETTO CONNESSIONE//
		clientsock=accept(serversock, NULL, NULL);
		if(clientsock==-1){
			perror("Errore clientsock");
			exit(1);
		}
		//CREO IL THREAD CHE GESTIRA LA COMUNICAZIONI//
		pthread_create(&wtid, NULL, worker_thread, (void*)clientsock);
	}
	exit(0);
}

void *worker_thread(void *args){
	int sock=(int)args;
	char buff[250];
	int len;
	char nome[STRUDIM];
	
	pthread_detach(pthread_self());//SETTO IL THREAD DETACH COSI DA NON FARE LA JOIN//
	
	while(1){//AL INFINITO LEGGO DALLA SOCKET//
		len=read(sock, buff, 250);
		if(len==0){
			break;
		}
		else if(len<0){
			printf("Errore in ricezione\n");
			break;
		}//SE INSERISCO REGISTER,METTE IN NOME CONTENUTO DEL BUFFER//
		if(strncmp(buff, "register", 8)==0){
			strcpy(nome,&buff[9]);
			sprintf(buff, "Welcome %s\n",nome);//METTE NEL BUFFER WELCOME+NOME//
			write(sock, buff, strlen(buff)+1);//SCRIVO SULLA SOCKET//
		}
		else if(strncmp(buff, "read",4)==0){//SE INSERISCO READ//
			pthread_mutex_lock(&mutex);//ACCEDO ALLA STRUCT CONDIVISA//
			sprintf(buff,"%d %s: %s\n",ultimomessaggio.id, ultimomessaggio.nome, ultimomessaggio.messaggio);//INSERISCO NEL BUF I CAMPI DEL MIO MESSAGGIO;
			pthread_mutex_unlock(&mutex);
			write(sock, buff, strlen(buff)+1);//SCRIVO SULLA SOCKET//
		}
		else if(strncmp(buff, "send",4)==0){//SE INSERISCO SEND//
			pthread_mutex_lock(&mutex);
			ultimomessaggio.id++;//INCREMENTA L'ID DEL MESSAGGIO//
			strcpy(ultimomessaggio.nome, nome);
			strcpy(ultimomessaggio.messaggio, &buff[5]);
			sprintf(buff,"ok");//METTO NEL BUFF OK PER DIRE CHE E TUTTO CORRETTO//
                        pthread_mutex_unlock(&mutex);
			write(sock, buff, strlen(buff)+1);//SCRIVO SULLA SOCKET//
		}
		else if(strncmp(buff, "close",5)==0){
			break;
		}
		else{
			printf("comando %s non riconosciuto\n",buff);
		}
	}
	close(sock);
	exit(0);
}




