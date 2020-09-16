#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include<arpa/inet.h>
#define PORTA 1789
int sock;
void *funzione(void* arg);
int main(void){
pthread_t t1;
struct sockaddr_in saddr;
char buff[250];

//1 CREO IL SOCKET//
sock=socket(AF_INET,SOCK_STREAM,0);
if(sock==-1){
perror("ERRORE DURANTE LA CREAZIONE DEL SOCKET\n");
exit(1);
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
//4 CREO UN THRED//
pthread_create(&t1,NULL,funzione,NULL);
//5 AL INFINITO PERMETTO DI INSERIRE COMANDI NELLA SOCK FINCHE NON E CLOSE//
while(1){
printf("Inserisci un comando\n");
fgets(buff,250,stdin);
buff[strlen(buff)-1]='\0';
write(sock,buff,strlen(buff)+1);
if(strncmp(buff,"close",5)==0){
break;
}
}
pthread_join(t1,NULL);
exit(0);
}
void *funzione(void* arg){
int lun=0;
char buff[250];
int id=0;
int rxid=0;
while(1){
sprintf(buff,"read");
write(sock,buff,strlen(buff)+1);
lun=read(sock,buff,250);
if(lun<0){
perror("ERRORE DURANTE LA COMUNICAZIONI\n");
break;
}
if(strncmp(buff,"OK",2)==0){
printf("IL MASSIMO:%s\n",buff);
printf("Inserisci un comando\n");
}
sscanf(buff,"%d %*s",&id);
if(id!=rxid){
printf("IL MASSIMO:%s\n",buff);
printf("Inserisci un comando\n");
rxid=id;
}
}
close(sock);
pthread_exit(0);
}
