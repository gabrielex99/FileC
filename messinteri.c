#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

void processo_figlio(int addr);
struct msgbuf{
long int mtype;
int valore[50];
 };

int main(void){
int msgid;
pid_t pid;
int i;
int a[50];
struct msgbuf messaggio;
//1 CREO LA CODA DI MESSAGGI//
msgid=msgget((key_t)1234,0666|IPC_CREAT);
if(msgid==-1){
perror("ERRORE DURANTE LA CREAZIONE DELLA CODA\n");
exit(1);
}
//2 CREO I FIGLI//
pid=fork();
switch(pid){
case -1:
perror("ERRORE DURANTE LA CREAZIONE DEL FIGLIO\n");
exit(0);
case 0:
processo_figlio(2);
exit(0);
default:
break;
}
//CODICE PADRE//
for(i=0;1;i++){
printf("Inserisci un valore:\n");
scanf("%d",&a[i]);
messaggio.mtype=2;
messaggio.valore[i]=a[i];
if(messaggio.valore[i]==5){
msgsnd(msgid,&messaggio,sizeof(messaggio),0);
break;
}
msgsnd(msgid,&messaggio,sizeof(messaggio),0);
msgrcv(msgid,&messaggio,sizeof(messaggio),1,0);
printf("IL MASSIMO:%d\n",messaggio.valore[i]);
}
//ELIMINO LA CODA DI MESSAGGI//
msgctl(msgid,IPC_RMID,NULL);
printf("TUTTO E ANDATO CORRETTAMENTE\n");
exit(0);
}
void processo_figlio(int addr){
int msgid;
int max=0;
int i;
struct msgbuf messaggio;
msgid=msgget((key_t)1234,0);
if(msgid==-1){
perror("ERRORE DURANTE LA CREAZIONE DELLA CODA\n");
exit(1);
}
for(i=0;1;i++){
msgrcv(msgid,&messaggio,sizeof(messaggio),addr,0);
if(messaggio.valore[i]==5){
break;
}
printf("Ho ricevuto il seguente messaggio:%d\n",messaggio.valore[i]);
if(messaggio.valore[i]>max){
max=messaggio.valore[i];
}
messaggio.valore[i]=max;
messaggio.mtype=1;
msgsnd(msgid,&messaggio,sizeof(messaggio),0);
}
exit(0);
}
