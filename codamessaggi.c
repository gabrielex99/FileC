#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
void processo_figlio(int addr);
struct dato{
char stringa[50];
int valore;
};

struct msgbuf {
               long mtype;       /* message type, must be > 0 */
               struct dato valore;   /* message data */
           };

int main(void){
int msgid;
int i=0;
struct msgbuf messaggio;
pid_t pid;
char stringa[50];
msgid=msgget((key_t)1234,0666|IPC_CREAT);
if(msgid==-1){
perror("CREAZIONE CODA DI MESSAGGI NON RIUSCITA\n");
exit(1);
}
pid=fork();
switch(pid){
case -1:
perror("CREAZIONE NON RIUSCITA\n");
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
if(strncmp(stringa,"ESCI",4)==0){
strcpy(messaggio.valore.stringa,stringa);
messaggio.mtype=2;//MANDO I MESSAGGI AL FIGLIO//
msgsnd(msgid,&messaggio,sizeof(struct dato),0);
break;
}
messaggio.mtype=2;//MANDO I MESSAGGI AL FIGLIO//
strcpy(messaggio.valore.stringa,stringa);
msgsnd(msgid,&messaggio,sizeof(struct dato),0);
msgrcv(msgid,&messaggio,sizeof(struct dato),1,0);
printf("HO RICEVUTO DAL FIGLIO IL SEGUENTE MESSAGGIO:%d\n",messaggio.valore.valore);
 }
//ASPETTO TERMINAZIONE DEL PROCESSO FIGLIO,ELIMINO LA CODA DI MESSAGGI//
wait(NULL);
msgctl(msgid,IPC_RMID,NULL);
printf("TUTTO E ANDATO CORRETTAMENTE\n");
exit(0);
}
void processo_figlio(int addr){
sleep(1);
int i=0;
int c=0;
int msgid;
struct msgbuf messaggio;
msgid=msgget((key_t)1234,0666|IPC_CREAT);
if(msgid==-1){
perror("CREAZIONE CODA DI MESSAGGI NON RIUSCITA\n");
exit(1);
}
for(i=0;i<5;i++){
msgrcv(msgid,&messaggio,sizeof(struct dato),addr,0);
if(strncmp(messaggio.valore.stringa,"ESCI",4)==0){
break;
}
printf("HO RICEVUTO IL SEGUENTE MESSAGGIO:%s\n",messaggio.valore.stringa);
c=strlen(messaggio.valore.stringa)-1;
if(c>=4){
messaggio.valore.valore=c;
}
else{
messaggio.valore.valore=0;
}
messaggio.mtype=1;//mando i messaggi al padre//
msgsnd(msgid,&messaggio,sizeof(struct dato),0);
}
exit(0);
}
