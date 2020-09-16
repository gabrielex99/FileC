#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define PORTA 1789 //VARIABILI CONDIVISE//
void*funzione(void *arg);

struct mess{
int id;
char nome[50];
int num1;
int num2;
int res;
};

pthread_mutex_t mutex;
struct mess messaggio;

int main(void){
int serversock;
int clientsock;
pthread_t t1;
struct sockaddr_in saddr;

//1 INIZIALIZZO IL MUTEX//
pthread_mutex_init(&mutex,NULL);
//2 creo socket//
serversock=socket(AF_INET,SOCK_STREAM,0);
if(serversock==-1){
perror("ERRORE DURANTE LA COMUNICAZIONE\n");
exit(1);
}
//3 SETTO LA STRUCT DEGLI INDIRIZZI//
saddr.sin_family=AF_INET;
saddr.sin_port=htons(PORTA);
saddr.sin_addr.s_addr=htonl(INADDR_ANY);
bzero(&messaggio,sizeof(messaggio));
//4 FACCIO LA BIND//
if(bind(serversock,(struct sockaddr*)&saddr,sizeof(saddr))==-1){
perror("ERRORE DURANTE LA BIND\n");
exit(1);
}
//5 FACCIO LA LISTEN//
listen(serversock,5);
//6 AL INFINITO ACCETTO CONESSIONI E CREO IL THREAD//
while(1){
clientsock=accept(serversock,NULL,NULL);
if(clientsock==-1){
perror("ERRORE DURANTE L'ACCEPT\n");
exit(1);
}
pthread_create(&t1,NULL,funzione,(void*)clientsock);
}
exit(0);
}
void *funzione(void *arg){
int sock=(int)arg;
char nome[50];
char buff[250];
int lun;
//CREO IL THREAD DETACH//
pthread_detach(pthread_self());
while(1){
lun=read(sock,buff,250);
if(lun<=0){
perror("ERRORE DURANTE LA LETTURA\n");
exit(1);
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
messaggio.num1=atoi(&buff[5]);
messaggio.num2=atoi(&buff[7]);
messaggio.res=messaggio.num1+messaggio.num2;
pthread_mutex_unlock(&mutex);
sprintf(buff,"ok");
write(sock,buff,strlen(buff)+1);
}
if(strncmp(buff,"read",4)==0){
pthread_mutex_lock(&mutex);
sprintf(buff,"%d %s %d %d %d",messaggio.id,messaggio.nome,messaggio.num1,messaggio.num2,messaggio.res);
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
