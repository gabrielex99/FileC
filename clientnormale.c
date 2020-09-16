#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>

#define PORT 6789



int main(void){
  int serversock;
  struct sockaddr_in saddr;
  int num1,num2,res;
  printf("Inserisci numero 1\n");
  scanf("%d",&num1);
  printf("Inserisci numero 2\n");
  scanf("%d",&num2);
  //1 CREO IL SOCKET//
  serversock = socket(AF_INET, SOCK_STREAM, 0);
  if(serversock==-1){
   perror("ERRORE DURANTE LA CREAZIONE");
   exit(1);
   }
   //2 SETTO LA STRUCT DEGLI INDIRIZZI//
     saddr.sin_family = AF_INET;
  saddr.sin_port = htons(PORT);
  saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
   //3 FACCIO LA CONNECT//
  if(connect(serversock, (struct sockaddr*)&saddr, sizeof(saddr))==-1){
    perror("ERRORE DURANTE LA CONNECT\n");
    exit(0);
    }
  write(serversock, &num1, sizeof(int));
  write(serversock, &num2, sizeof(int));
  read(serversock, &res, sizeof(int));

  printf("\nrisultato: %d\n",res);

  return 0;
}
