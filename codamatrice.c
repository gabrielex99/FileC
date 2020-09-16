#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#define DIM 5
//QUESTO PROGRAMMA FUNZIONA SIA SE HO QUELLO VERSIONE AD UN ARRAY SIA QUESTO PER LA MATRICE//
struct dato{
int array[DIM][DIM];
};
struct mess{
long int mtype;
struct dato valore;
};
void processo_figlio(int addr);

int main(void){
int msgid;
pid_t pid;
int i;
int j;
int a[DIM][DIM];
struct mess messaggio;
//CREO LA CODA//
msgid=msgget((key_t)1234,0666|IPC_CREAT);
if(msgid==-1){
perror("CREAZIONE NON RIUSCITA\n");
exit(1);
}
//CREO I FIGLI//
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
for(i=0;i<DIM;i++){
for(j=0;j<DIM;j++){
printf("Inserisci un valore\n");
scanf("%d",&a[i][j]);
messaggio.valore.array[i][j]=a[i][j];
messaggio.mtype=2;
msgsnd(msgid,&messaggio,sizeof(struct dato),0);
msgrcv(msgid,&messaggio,sizeof(struct dato),1,0);
printf("HO RICEVUTO DAL FIGLIO IL SEGUENTE MESSAGGIO:%d\n",messaggio.valore.array[i][j]);
}
}
//ASPETTO IL FIGLIO//
printf("Aspetto la terminazione del figlio\n");
wait(NULL);
msgctl(msgid,IPC_RMID,NULL);
printf("PROGRAMMA FINITO:\n");
exit(0);
}
//CODICE FIGLIO//

void processo_figlio(int addr){
sleep(1);
int i;
int j;
int msgid;
struct mess messaggio;
msgid=msgget((key_t)1234,0666|IPC_CREAT);
if(msgid==-1){
perror("CREAZIONE NON RIUSCITA\n");
exit(1);
}
for(i=0;i<DIM;i++){
for(j=0;j<DIM;j++){
msgrcv(msgid,&messaggio,sizeof(struct dato),addr,0);
printf("Ho ricevuto il seguente messaggio:%d\n",messaggio.valore.array[i][j]);
messaggio.mtype=1;//MANDO AL PADRE//
messaggio.valore.array[i][j]=messaggio.valore.array[i][j]+5;
msgsnd(msgid,&messaggio,sizeof(struct dato),0);
}
}
exit(0);
}
