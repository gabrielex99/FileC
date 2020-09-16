//SVILLUPARE UN CLIENT CHE PERMETTA DI INSERIRE DEI COMANDI E VISUALIZZARE QUELLO CHE IL SERVER RISPONDE//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#define PORTA 1789

int sock;
void *funzione(void *arg);

int main(void){
pthread_t t1;
char buff[300];
struct sockaddr_in saddr;
//1 CREO IL SOCK//
sock=socket(AF_INET,SOCK_STREAM,0);
if(sock==-1){
perror("ERRORE DURANTE LA CREAZIONE DEL SOCKET\n");
exit(0);
}
//2 SETTO LA STRUCT DEGLI INDIRIZZI//
saddr.sin_family=AF_INET;
saddr.sin_port=htons(PORTA);
saddr.sin_addr.s_addr=inet_addr("127.0.0.1");
//3 FACCIO LA CONNECT//
if(connect(sock,(struct sockaddr*)&saddr,sizeof(saddr))==-1){
perror("ERRORE DURANTE LA CONNECT\n");
exit(1);
}
//4 CREO IL THREAD//
pthread_create(&t1,NULL,funzione,NULL);
//5 AL INFINITO IMMETTENDO COMANDI NEL SERVER FINCHE NON METTO CLOSE//
while(1){
printf("Inserisci un comando\n");
fgets(buff,300,stdin);
buff[strlen(buff)-1]='\0';
write(sock,buff,strlen(buff)+1);
if(strncmp(buff,"close",5)==0){
break;
}
}

pthread_join(t1,NULL);
exit(0);
}
void *funzione(void*arg){
char buff[300];
int lun;
int id=0;
int rxid=0;

while(1){
sprintf(buff,"leggi");
write(sock,buff,strlen(buff)+1);
lun=read(sock,buff,300);
if(lun<0){
perror("ERRORE DURANTE LA COMUNICAZIONE\n");
exit(0);
}
if(strncmp(buff,"OK",2)==0){
printf("HO RICEVUTO:%s\n",buff);
}
sscanf(buff,"%d %*s",&id);
if(id!=rxid){
printf("Ho ricevuto:%s\n",buff);
rxid=id;
}
}
close(sock);
pthread_exit(0);
}
