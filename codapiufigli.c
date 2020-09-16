//PROGRAMMA IN CUI UN PROCESSO PADRE MANDA DEI VALORI INTERI AL FIGLIO,IL FIGLIO CALCOLA IL MASSIMO E LO RESTITUISCE AL PADRE CHE LO STAMPA//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/wait.h>
void processo_figlio(int addr);
struct dato{
int a[20];
int max;
};

struct msgbuf{
long int mtype;
struct dato valore;
};

int main(void){
int msgid;
pid_t pid;
int val[20];
int i;
int j;
struct msgbuf messaggio;

//1 CREO LA CODA DI MESSAGGI//
msgid=msgget((key_t)1234,0666|IPC_CREAT);
if(msgid==-1){
perror("ERRORE DURANTE LA CREAZIONE\n");
exit(1);
}
//FACCIO I FIGLI//
for(j=0;j<2;j++){
pid=fork();
switch(pid){
case -1:
perror("ERRORE DURANTE LA CREAZIONE\n");
exit(1);
case 0:
processo_figlio(j+2);
exit(0);
default:
break;
}
//CODICE PADRE//
for(i=0;i<3;i++){
printf("Inserisci un valore\n");
scanf("%d",&val[i]);
messaggio.valore.a[i]=val[i];
messaggio.mtype=j+2;
msgsnd(msgid,&messaggio,sizeof(struct dato),0);
msgrcv(msgid,&messaggio,sizeof(struct dato),1,0);
printf("IL MASSIMO:%d\n",messaggio.valore.max);
}
}
wait(NULL);
wait(NULL);
msgctl(msgid,IPC_RMID,NULL);
printf("TUTTO E ANDATO CORRETTAMENTE\n");
exit(0);
}

void processo_figlio(int addr){
int msgid;
int i;
int max;
struct msgbuf messaggio;
msgid=msgget((key_t)1234,0);
if(msgid==-1){
perror("ERRORE DURANTE LA CREAZIONE\n");
exit(1);
}
for(i=0;i<3;i++){
msgrcv(msgid,&messaggio,sizeof(struct dato),addr,0);
printf("PROCESSO:%d\n",addr-2);
printf("Ho ricevuto il seguente messaggio:%d\n",messaggio.valore.a[i]);
if(messaggio.valore.a[i]>max){
max=messaggio.valore.a[i];
}
messaggio.valore.max=max;
messaggio.mtype=1;
msgsnd(msgid,&messaggio,sizeof(struct dato),0);
}
exit(0);
}
