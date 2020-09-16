//scriviamo un programma che permette di simulare una shell ed accetta comandi personalizzati(execve)//
//chiede di inserire comando//
//lo salva in una stringa//
//il padre fa la fork si mette in atessa del figlio//
//il processo figlio tramite l'exec//eseguira il comando digita da tastiera.
//COMANDI SONO ELENCO_FILE->CHIAMA IN EXEC LS//
//             ELENCO_PROCESSI->ps -a;
               //stampa_ciao->echo "ciao"//
 //4 esci->termina//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
int main(){
  pid_t pid;
  int valore;
  int exit_value=0;
  char *stringa;
  char *comando;
  printf("COMANDO DISPONIBILE 1:elenco_file\n");
  printf("COMANDO DISPONIBILE 2:elenco_processi\n");
  printf("COMANDO DISPONIBILE 3:stampa_ciao\n");
  printf("COMANDO DISPONIBILE 1:esci\n");
  printf("Inserisci un comando\n");
  scanf("%s",stringa);
  if(strcmp(stringa,"elenco_file")==0){
  comando="ls";
  valore=1;
  }
  if(strcmp(stringa,"stampa_ciao")==0){
  comando="echo";
  valore=2;
  }
  if(strcmp(stringa,"elenco_processi")==0){
    comando="ps";
    valore=3;
  }

  pid=fork();
  switch (pid) {
    case -1:
    perror("FORK NON RIUSCITA\n");
    exit(1);
    break;
    case 0://processo figlio//
    if(valore==1)
    execlp(comando,comando,NULL);//studiare la sintassi//
  if(valore==2)
   execlp(comando,comando,"ciao",NULL);//CIAO E UN PARAMETRO DEL PROGRAMMA ECHO//
  if(valore==3)
    execlp(comando,comando,"a",NULL); //COMANDO PS  A E UN PARAMETRO DEL PROGRAMMA PS//
  if(strcmp(stringa,"esci")==0){
    exit(0);
  }
    exit(exit_value);
    break;
    default://processo padre//
    break;
  }
  if(pid!=0){
    pid_t figlio;
    figlio=wait(&valore);
    if(WIFEXITED(valore)!=0)
    printf("Il processo figlio ha finito correttamente:%d\n",WEXITSTATUS(valore));
    else
    printf("FIGLIO NON HA FINITO correttamente");
  }
}
