#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
void *funzione(void *arg);
//VARIABILI CONDIVISE//
struct msgbuf{
int id;
char nome[50];
char messaggio[250];
};
//VARIABILI CONDIVISE//
pthread_mutex_t mutex;
struct msgbuf messaggio;
#define PORTA 1789
int main(void){
int serversock;
int clientsock;
pthread_t t1;
struct sockaddr_in saddr;
//1 inizializzo il mutex//
pthread_mutex_init(&mutex,NULL);
//2 creo il socket
serversock=socket(AF_INET,SOCK_STREAM,0);
if(serversock==-1){
perror("ERRORE DURANTE LA COMUNICAZIONE\n");
exit(1);
}
//3 SETTO LA STRUCT DEGLI INDIRIZZI//
saddr.sin_family=AF_INET;
saddr.sin_port=htons(PORTA);
saddr.sin_addr.s_addr=htonl(INADDR_ANY);
//4 FACCIO LA BIND//
if(bind(serversock,(struct sockaddr*)&saddr,sizeof(saddr))==-1){
perror("ERRORE DURANTE LA BIND\n");
exit(1);
}
//5 FACCIO LA LISTEN//
listen(serversock,5);
//AL INFINITO ACCETTO CONNESSIONI E CREO THREAD//
while(1){
clientsock=accept(serversock,NULL,NULL);
if(clientsock==-1){
perror("ERRORE DURANTE LA ACCEPT\n");
break;
}
pthread_create(&t1,NULL,funzione,(void*)clientsock);
}
//DISTRUGGO IL THREAD ED ESCO//
pthread_mutex_destroy(&mutex);
exit(0);
}

void *funzione(void *arg){
char nome[50];
char buff[350];
char stringa[400];
int lun;
int sock=(int)arg;
pthread_detach(pthread_self());
while(1){
lun=read(sock,buff,250);
if(lun<0){
perror("ERRORE DURANTE LA COMUNICAZIONE\n");
break;
}
if(strncmp(buff,"register",8)==0){
strcpy(nome,&buff[9]);
sprintf(buff,"WELCOME:%s\n",nome);
write(sock,buff,strlen(buff)+1);
}
if(strncmp(buff,"send",4)==0){
pthread_mutex_lock(&mutex);
messaggio.id++;
strcpy(messaggio.nome,nome);
strcpy(stringa,&buff[5]);
strcat(stringa,"DIO");
strcpy(messaggio.messaggio,stringa);
pthread_mutex_unlock(&mutex);
sprintf(buff,"OK");
write(sock,buff,strlen(buff)+1);
}
if(strncmp(buff,"read",4)==0){
pthread_mutex_lock(&mutex);
sprintf(buff,"%d %s %s\n",messaggio.id,messaggio.nome,messaggio.messaggio);
pthread_mutex_unlock(&mutex);
write(sock,buff,strlen(buff)+1);
}
if(strncmp(buff,"close",5)==0){
break;
}
}
close(sock);
pthread_exit(0);
}
