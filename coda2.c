//IL PROGRAMMA MANDA DELLE STRINGHE ATTRAVERSO LA CODA DI MESSAGGI,IL PROCESSO FIGLIO DI VOLTA IN VOLTA CALCOLA LA LUNGHEZZA E LA RESTITUISCE AL PADRE//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/wait.h>
void processo_figlio(int addr);
struct dato{
char stringa[200];
int lun;
};

struct msgbuf{
long int mtype;
struct dato valore;
};

int main(void){
int msgid;
pid_t pid;
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
for(i=0;1;i++){
printf("Inserisci una stringa\n");
fgets(messaggio.valore.stringa,200,stdin);
messaggio.mtype=j+2;
if(strncmp(messaggio.valore.stringa,"ESCI",4)==0){
msgsnd(msgid,&messaggio,sizeof(struct dato),0);
break;
}
msgsnd(msgid,&messaggio,sizeof(struct dato),0);
msgrcv(msgid,&messaggio,sizeof(struct dato),1,0);
printf("LA LUNGHEZZA:%d\n",messaggio.valore.lun);
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
int lun;
struct msgbuf messaggio;
msgid=msgget((key_t)1234,0);
if(msgid==-1){
perror("ERRORE DURANTE LA CREAZIONE\n");
exit(1);
}
for(i=0;1;i++){
msgrcv(msgid,&messaggio,sizeof(struct dato),addr,0);
if(strncmp(messaggio.valore.stringa,"ESCI",4)==0){
break;
}
printf("PROCESSO:%d\n",addr-2);
printf("Ho ricevuto il seguente messaggio:%s\n",messaggio.valore.stringa);
lun=strlen(messaggio.valore.stringa)-1;
messaggio.valore.lun=lun;
messaggio.mtype=1;
msgsnd(msgid,&messaggio,sizeof(struct dato),0);
}
exit(0);
}
