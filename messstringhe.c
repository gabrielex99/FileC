#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/wait.h>
void processo_figlio(int addr);
  struct msg {
               long int  mtype;       /* message type, must be > 0 */
               char dati[50];    /* message data */
           };



int main(void){
int msgid;
pid_t pid;
int i=0;
char stringa[50];
struct msg messaggio;
//1 CREO LA CODA DI MESSAGGI//
msgid=msgget((key_t)1234,0666|IPC_CREAT);
if(msgid==-1){
perror("ERRORE DURANTE LA CREAZIONE DELLA CODA DI MESSAGGI\n");
exit(1);
}
pid=fork();
switch(pid){
case -1:
perror("ERRORE DURANTE LA CREAZIONE DEL FIGLIO\n");
exit(1);
case 0:
processo_figlio(2);
exit(0);
default:
break;
}
//CODICE PADRE//
for(i=0;i<5;i++){
printf("Inserisci una stringa\n");
fgets(stringa,50,stdin);
messaggio.mtype=2;
strcpy(messaggio.dati,stringa);
if(strncmp(messaggio.dati,"ESCI",4)==0){
msgsnd(msgid,&messaggio,sizeof(messaggio),0);
break;
}
msgsnd(msgid,&messaggio,sizeof(messaggio),0);
msgrcv(msgid,&messaggio,sizeof(messaggio),1,0);
printf("Ho ricevuto il seguente messaggio:%s\n",messaggio.dati);
}
//ASPETTO LA TERMINAZIONE DEL FIGLIO,ELIMINO LA CODA DI MESSAGGI//
wait(NULL);
msgctl(msgid,IPC_RMID,NULL);
printf("TUTTO E ANDATO CORRETTAMENTE\n");
exit(0);
}

void processo_figlio(int addr){
int msgid;
int lun;
int i=0;
struct msg messaggio;
//ACCEDDO ALLA CODA DI MESSAGGI//
msgid=msgget((key_t)1234,0);
if(msgid==-1){
perror("ERRORE DURANTE LA CREAZIONE DELLA CODA DI MESSAGGI\n");
exit(1);
}
for(i=0;i<5;i++){
msgrcv(msgid,&messaggio,sizeof(messaggio),addr,0);
if(strncmp(messaggio.dati,"ESCI",4)==0){
break;
}
messaggio.dati[strlen(messaggio.dati)-1]='\0';
lun=strlen(messaggio.dati);
printf("Ho ricevuto il seguente valore:%s\n",messaggio.dati);
printf("ESSO E LUNGO:%d\n",lun);
if(lun>5){
strcpy(messaggio.dati,"STRINGA LUNGA");
}
else{
strcpy(messaggio.dati,"STRINGA CORTA");
}
messaggio.mtype=1;
msgsnd(msgid,&messaggio,sizeof(messaggio),0);
}
exit(0);
}
