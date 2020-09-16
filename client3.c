#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define PORTA 1789
int sock;
void *funzione(void *arg);

int main(void){
pthread_t t1;
char buff[200];
struct sockaddr_in saddr;

//1 CREO IL SOCKET//
sock=socket(AF_INET,SOCK_STREAM,0);
if(sock==-1){
perror("Errore durante la creazione\n");
exit(1);
}
//SETTO LA STRUCT DEGLI INDIRIZZI//
saddr.sin_family=AF_INET;
saddr.sin_port=htons(PORTA);
saddr.sin_addr.s_addr=inet_addr("127.0.0.1");
//FACCIO LA CONNECT CREO IL THREAD//
if(connect(sock,(struct sockaddr*)&saddr,sizeof(saddr))==-1){
perror("ERRORE DURANTE LA CONNECT\n");
exit(1);
}
pthread_create(&t1,NULL,funzione,NULL);
//ALL INFINITO PERMETTO DI INSERIRE UN COMANDO FINCHE SIA CLOSE E MI FERMO//
while(1){
printf("Inserisci un comando\n");
fgets(buff,200,stdin);
buff[strlen(buff)-1]='\0';
write(sock,buff,strlen(buff)+1);
if(strncmp(buff,"close",5)==0){
break;
}
}
pthread_join(t1,NULL);
close(sock);
exit(0);
}
void *funzione(void *arg){
int lun;
char buff[200];
int id=0;
int rxid=0;
while(1){
sprintf(buff,"read");
write(sock,buff,strlen(buff)+1);
lun=read(sock,buff,250);
if(lun<0){
perror("ERRORE DURANTE LA COMUNICAZIONE\n");
break;
}

if(strncmp(buff,"ok",2)==0){
printf("Hai inserito:%s\n",buff);
printf("Inserisci un comando\n");
}
sscanf(buff,"%d %*s",&id);
if(id!=rxid){
printf("Hai inserito:%s\n",buff);
printf("Inserisci un comando\n");
rxid=id;
}
}
pthread_exit(0);
}
