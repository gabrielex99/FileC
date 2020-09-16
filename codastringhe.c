 #include <sys/types.h>
 #include <sys/ipc.h>
 #include <sys/msg.h>
 #include <string.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <sys/wait.h>
 //ABBIAMO 2 PROCESSI IL PADRE INSERISCE UNA STRINGA DA TASTIERA LA MANDA AL FIGLIO,IL FIGLIO CONTA I CARATTERI LETTI,LI MANDA AL PADRE CHE
  //STAMPA TALE VALORE//
#define dim 50
 struct dato{
 char stringa[dim];
 int valore;
 };
 struct mess{ //SE NON RICORDO FARE MAN MSGRCV//
 long int mtype;
 struct  dato dati; //QUESTA IN BASE AL PROGRAMMA LA POSSO PERSONALIZZARE A MIO PIACIMENTO CON UNA STRUCT//
 };
 void processo_figlio(int addr);//ADDR MI SERVE PER MANDARE E RICEVERE MESSAGGI METTERE SEMPRE I+2 SE HO PIU PROCESSI 2 SE HO SOLO 2 PROCESSI//

 int main(void){
 int msgid;
 char stringa[dim];
 pid_t pid;
 int i;
 struct mess messaggio;
 msgid=msgget((key_t)1234,0666|IPC_CREAT);//CREO LA CODA DI MESSAGGI//
 if(msgid==-1){
 perror("CREAZIONE NON RIUSCITA\n");
 exit(1);
 }
 //CREO IL FIGLIO//
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
 //CODICE DEL PADRE//
  for(i=0;i<3;i++){
  printf("Inserisci la stringa\n");
  fgets(stringa,dim,stdin);
  strcpy(messaggio.dati.stringa,stringa);
  messaggio.dati.valore=0;
  messaggio.mtype=2;
  msgsnd(msgid,&messaggio,sizeof(struct dato),0);
  msgrcv(msgid,&messaggio,sizeof(struct dato),1,0);
  printf("HO RICEVUTO IL SEGUENTE MESSAGGIO:%d\n",messaggio.dati.valore);
}
  printf("Aspetto la terminazione del figlio\n");
  wait(NULL);
  msgctl(msgid,IPC_RMID,NULL);
  printf("PROGRAMMA FINITO:\n");
 }
 void processo_figlio(int addr){
 int msgid;
 int c=0;
 int i;
 struct mess messaggio;
 msgid=msgget((key_t)1234,0666|IPC_CREAT);
 if(msgid==-1){
 perror("CREAZIONE NON RIUSCITA\n");
 exit(1);
 }
 for(i=0;i<3;i++){
 msgrcv(msgid,&messaggio,sizeof(struct dato),addr,0);
 printf("HO RICEVUTO IL SEGUENTE MESSAGGIO DAL PADRE:%s\n",messaggio.dati.stringa);
 c=strlen(messaggio.dati.stringa);
 messaggio.mtype=1;
 messaggio.dati.valore=c;
 msgsnd(msgid,&messaggio,sizeof(struct dato),0);
 }
 exit(0);
}
