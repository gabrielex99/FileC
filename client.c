#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORTA 1789
int sock;
void *funzione(void *arg);

int main(void){
pthread_t t1;
char buff[250];
struct sockaddr_in saddr;
//1 CREO IL SOCK//
sock=socket(AF_INET,SOCK_STREAM,0);
if(sock==-1){
perror("Errore durante la creazione del socket\n");
exit(1);
}
//2 SETTO LA STRUCT //
saddr.sin_family=AF_INET;
saddr.sin_port=htons(PORTA);
saddr.sin_addr.s_addr=inet_addr("127.0.0.1");
//3 FACCIO LA CONNECT//
if(connect(sock,(struct sockaddr*)&saddr,sizeof(saddr))==-1){
perror("Errore durante la connect\n");
exit(1);
}
//4 CREO UN THREAD//
pthread_create(&t1,NULL,funzione,NULL);
//5 AL INFINITO INSERISCO COMANDI FINCHE NON LEGGO CLOSE//
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
close(sock);
exit(0);
}
void *funzione(void *arg){
int lun;
char buff[250];
//PER STAMPARE IL MESSAGGIO UNA VOLTA SOLA//
int id=0;
int rxid=0;
//AL INFINITO METTO READ NELLA SOCKET E VEDO COSA MI RESTITUISCE IL SERVER//
while(1){
sprintf(buff,"read");
write(sock,buff,strlen(buff)+1);
lun=read(sock,buff,250);
if(lun<0){
perror("ERRORE DURANTE LA COMUNICAZIONE\n");
break;
}
if(strncmp(buff,"OK",2)==0){
printf("E STATO INSERITO:%s\n",buff);
printf("Inserisci un comando\n");
}
sscanf(buff,"%d %*s",&id);//MI PRELEVO DAL BUFF UN INTERO ID//
if(rxid!=id){
printf("E STATO INSERITO:%s\n",buff);
printf("Inserisci un comando\n");
rxid=id;
}
}
pthread_exit(0);
}
