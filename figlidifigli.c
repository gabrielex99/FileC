//UN PADRE GENERA UN FIGLIO,TALE FIGLIO GENERA 2 PROCESSI FIGLI CHE DIVENTERANNO 2 NUOVI PROGRAMMI//
//I 2 FIGLI MANDANO UN SEGNALE SIGUSR1 AL PROPRIO PADRE CHE UNA VOLTA RICEVUTO SCRIVE CHE SI SONO TRASFORMATI E MANDA UN SIGNALE SIGUSR2 AL PADRE che conferma che tutto è andato CORRETTAMENTE//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
void f_figlio(int sig){
  printf("IL FIGLIO SI E TRASFORMATO IN PROGRAMMA");
}
void f_padre(int sig){
  printf("TUTTO E ANDATO CORRETTAMENTE\n");
  printf("IL PADRE DI TUTTI HA PID:%d\n",getpid());
}

int main(){
  pid_t pid;
  pid_t figli[2];
  int i=0;
  int valore;
  signal(SIGUSR1,f_figlio);
  signal(SIGUSR2,f_padre);
  printf("Inizio del programma\n");
  pid=fork();//CREO IL PRIMO PROCESSO//
  switch (pid) {
    case -1:
    perror("Fork non riuscita\n");
    exit(1);
    case 0://SIAMO NELLA PARTE DEL FIGLIO//
    for(i=0;i<2;i++){
      figli[i]=fork();
      switch (figli[i]) {
        case -1:
        perror("Fork non riuscita CORRETTAMENTE");
        exit(1);
        case 0://SIAMO NEI 2 FIGLI//
        if(i==0){
          kill(getppid(),SIGUSR1);
          execlp("ls","ls",NULL);
          exit(0);
        }
        if(i==1){
          kill(getppid(),SIGUSR1);
          execlp("ps","ps",NULL);
          exit(0);
        }
        default:
        printf("IL PADRE DEI 2 FIGLI HA PID:%d\n",getpid());
        sleep(1);
        kill(getppid(),SIGUSR2);
        pid_t figlin[i];
        int valore2;
        figlin[i]=wait(&valore2);
        if(WIFEXITED(valore2)!=0){
        printf("IL figlio ha pid:%d\n",figlin[i]);
        printf("Il figlio è uscito con successo:%d\n",WEXITSTATUS(valore2));
      }
        else
        printf("il figlio non è uscito con successo");
              }
    }
    default:
  break;
  }
  if(pid!=0){
    pid_t children;
    children=wait(&valore);
    if(WIFEXITED(valore)!=0)
      printf("Il figlio è uscito con successo:%d\n",WEXITSTATUS(valore));
      else
      printf("IL FIGLIO NON HA FINITO CORRETTAMENTE");
  }
}
