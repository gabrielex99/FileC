//MANDA A 5 PROCESSI DIFFERENTI PER 5 VOLTE UNA STRINGA CASUALE IN BASE AL COMANDO INSERITO VIENE RESTITUTO AL PADRE QUALCOSA//
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdio.h>
void processo_figlio(int addr);
struct msgbuf {
               long mtype;       /* message type, must be > 0 */
               char dati[100];    /* message data */
           };


int main(void){
int msgid;
pid_t pid;
int i,j,k;//INDICI//
struct msgbuf messaggio;
char *comandi[]={"AVANTI","INDIETRO","SINISTRA","DESTRA"};
//1 CREO LA CODA DI MESSAGGI//
msgid=msgget((key_t)1234,0666|IPC_CREAT);
if(msgid==-1){
perror("ERRORE DURANTE LA CREAZIONE\n");
exit(1);
}
//2 CREO I FIGLI//
for(i=0;i<5;i++){
pid=fork();
switch(pid){
case -1:
perror("ERRORE DURANTE LA CREAZIONE DELLA CODA DI MESSAGGI\n");
exit(1);
case 0:
processo_figlio(i+2);
exit(0);
default:
break;
}
}
//CODICE DEL PADRE//
srand(pid);
for(k=0;k<5;k++){
for(i=0;i<5;i++){
if(i==5){
strcpy(messaggio.dati,"TERMINA");
msgsnd(msgid,&messaggio,sizeof(messaggio),0);
break;
}
j=rand()%4;
messaggio.mtype=i+2;
strcpy(messaggio.dati,comandi[j]);
msgsnd(msgid,&messaggio,sizeof(messaggio),0);
msgrcv(msgid,&messaggio,sizeof(messaggio),1,0);
printf("Ho ricevuto il seguente messaggio:%s\n",messaggio.dati);
}
}
wait(NULL);
wait(NULL);
msgctl(msgid,IPC_RMID,NULL);
printf("TUTTO E ANDATO CORRETTAMENTE\n");
exit(0);
}

void processo_figlio(int addr){
int i;
int msgid;
int lun=0;
int k;
struct msgbuf messaggio;
msgid=msgget((key_t)1234,0);
if(msgid==-1){
perror("ERRORE DURANTE LA CREAZIONE\n");
exit(1);
}
for(k=0;k<5;k++){
msgrcv(msgid,&messaggio,sizeof(messaggio),addr,0);
printf("PROCESSO:%d\n",addr-2);
if(strncmp(messaggio.dati,"TERMINA",7)==0){
exit(0);
}
lun=strlen(messaggio.dati)-1;
printf("Ho ricevuto il seguente messaggio:%s\n",messaggio.dati);
printf("La sua lunghezza:%d\n",lun);
messaggio.mtype=1;
if(strncmp(messaggio.dati,"AVANTI",6)==0){
strcpy(messaggio.dati,"SONO ANDATO AVANTI");
}
if(strncmp(messaggio.dati,"INDIETRO",8)==0){
strcpy(messaggio.dati,"SONO ANDATO INDIETRO");
}
if(strncmp(messaggio.dati,"SINISTRA",8)==0){
strcpy(messaggio.dati,"SONO ANDATO A SINISTRA");
}
if(strncmp(messaggio.dati,"DESTRA",6)==0){
strcpy(messaggio.dati,"SONO ANDATO A DESTRA");
}
msgsnd(msgid,&messaggio,sizeof(messaggio),0);
}
exit(0);
}
