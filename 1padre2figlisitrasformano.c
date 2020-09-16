//SCRIVERE UN PROGRAMMA IN CUI UN PADRE GENERA N FIGLI 2 DI QUESTI SI TRASFORMANO IN PROCESSI E GLI ALTRI STAMPANO IL PROPRIO PID E QUELLO DEL PADRE//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define N 4
int main(){
  pid_t p[N];
  pid_t figli[N];
  int valore;
  int i;
  printf("Inizio del programma\n");
  for(i=0;i<N;i++){
  p[i]=fork();
  switch(p[i]){
    case -1:
    perror("Fork non riuscita\n");
    exit(1);
    case 0:
  if(i==0){
    printf("Sono il processo figlio:%d\n",i);
    printf("IL MIO PID:%d\n",getpid());
    printf("IL PID DI MIO PADRE:%d\n",getppid());
    sleep(2);
  }
  if(i==1){
    printf("Sono il processo figlio:%d\n",i);
    printf("IL MIO PID:%d\n",getpid());
    printf("IL PID DI MIO PADRE:%d\n",getppid());
    sleep(2);
  }

    if(i==2){
      execlp("ls","ls","-l",NULL);
    }
    if(i==3){
      execlp("ps","ps","-a",NULL);
    }
    exit(0);
    break;
    default:
    break;//IL PADRE NON FA NULLA ASPETTA SOLO CHE I FIGLI TERMINANO//
  }
  }
  for(i=0;i<N;i++){
    if(p[i]!=0){
      figli[i]=wait(&valore);
      if(WIFEXITED(valore)!=0){
      printf("Il figlio ha finito correttamente con valore:%d\n",WEXITSTATUS(valore));
    sleep(2);
    }
      else
      printf("Il figlio non ha finito correttamente");
    }
  }
}
