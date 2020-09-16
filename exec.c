#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
  pid_t pid;
  char *const argomenti[]={"ls","-l",NULL};
  printf("Inizio del programma\n");
  pid=fork();
  switch (pid) {
    case -1:
    perror("Fork non riuscita correttamente");
    exit(1);
    break;
    case 0://processo figlio//
    //SONO QUELLE CON GLI ARGOMENTI PASSATI COME LISTA//
    //execl("/bin/date","date",NULL);//FUNZIONA correttamente//
    //execlp("date","date",NULL);//FUNZIONA correttamente//
    //execl("/bin/echo","echo","ciao",NULL);//FUNZIONA correttamente E PRENDE UN PARAMETRO CHE SAREBBE IL CIAO//
    //SONO QUELLE CON GLI ARGOMENTI PASSATI COME VETTORI//
    //execv("/bin/ls",argomenti);//
    execvp("ls",argomenti);
    break;
    default:
    printf("Sono il processo padre con pid:%d\n",getpid());
    printf("Il mio figlio ha pid:%d\n",pid);
    break;
  }

  if(pid!=0){
  pid_t figlio;
  int valore;
  figlio=wait(&valore);
  if(WIFEXITED(valore)!=0)
  printf("IL FIGLIO HA FINITO correttamente CON VALORE:%d\n",WEXITSTATUS(valore));
  else
  printf("IL FIGLIO NON HA FINITO correttamente");
}
exit(0);
}
