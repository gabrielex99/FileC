#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
void processo_figlio(int addr);
struct numero{
int a[20];
int b[20];
};

struct msgbuf{
long int mtype;
struct numero dato;
};

int main(void){
int msgid;
pid_t pid;
int i;
int j;
int c[]={1,2,3,4,5};
struct msgbuf messaggio;
//1 CREO LA CODA DI MESSAGGI//
msgid=msgget((key_t)1234,0666|IPC_CREAT);
if(msgid==-1){
perror("Errore durante la creazione della coda di messaggi\n");
exit(1);
}
//2 CREO I FIGLI//
pid=fork();
switch(pid){
case -1:
perror("ERRORE DURANTE LA CREAZIONE DEI FIGLI\n");
exit(1);
case 0:
processo_figlio(2);
exit(0);
default:
break;
}
srand(pid);
//CODICE PADRE//
for(i=0;1;i++){
j=rand()%5;
messaggio.mtype=2;
if(i==10){
messaggio.dato.a[i]=0;
msgsnd(msgid,&messaggio,sizeof(messaggio.dato),0);
break;
}
messaggio.dato.a[i]=c[j];
msgsnd(msgid,&messaggio,sizeof(messaggio.dato),0);
msgrcv(msgid,&messaggio,sizeof(messaggio.dato),1,0);
if(messaggio.dato.b[i]>messaggio.dato.a[i]){
printf("Il secondo dado ha vinto\n");
}
}
wait(NULL);
msgctl(msgid,IPC_RMID,NULL);
printf("TUTTO ANDATO CORRETTAMENTE\n");
exit(0);
}
void processo_figlio(int addr){
int msgid;
int i;
int k;
int dado[]={1,2,3,4,5};
struct msgbuf messaggio;
msgid=msgget((key_t)1234,0666|IPC_CREAT);
if(msgid==-1){
perror("Errore durante la creazione della coda di messaggi\n");
exit(1);
}
for(i=0;1;i++){
msgrcv(msgid,&messaggio,sizeof(messaggio.dato),addr,0);
if(messaggio.dato.a[i]==0){
break;
}
printf("IL PRIMO LANCIO DI DADI:%d\n",messaggio.dato.a[i]);
k=rand()%5;
messaggio.dato.b[i]=dado[k];
messaggio.mtype=1;
msgsnd(msgid,&messaggio,sizeof(messaggio.dato),0);
}
exit(0);
}
