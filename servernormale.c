#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<pthread.h>

#define PORT 6789

void *funzione (void* args);

int main(void){
  int server;
  int client;
  struct sockaddr_in saddr;
  pthread_t t1;
  
  //1 CREO IL SOCKET//

  server = socket(AF_INET, SOCK_STREAM, 0);
  if(server == -1){
    perror("error creating socket: ");
    exit(1);
  }
   //2 SETTO LA STRUCT DEGLI INDIRIZZI//
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(PORT);
  saddr.sin_addr.s_addr = htonl(INADDR_ANY);
  //3 FACCIO LA BIND//
  if(bind(server,(struct sockaddr*)&saddr, sizeof(saddr)) == -1){
    perror("error binding: ");
    exit(1);
  }
  //4 FACCIO LA LISTEN//
  listen(server, 5);
  //5 AL INFINITO ACCETTO CONNESSIONI//
  while(1){
    client = accept(server,NULL,NULL);
    if(client==-1){
     perror("ERRORE DURANTE L'ACCEPT\n");
     exit(1);
    }
    pthread_create(&t1,NULL,funzione,(void*)client);
  }
  exit(0);
}


void *funzione (void* args){
  int num1, num2, res;
  pthread_detach(pthread_self());//COSI LO RENDO DETACH E NON DEVO CHIAMARE LA JOIN//
  int sock = (int)args;
  read(sock, &num1, sizeof(int));
  read(sock, &num2, sizeof(int));
  res = num1 + num2;
  write(sock, &res, sizeof(int));
}
