#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
void f(int sig){
printf("E ARRIVATO IL SEGNALE:%d\n",sig);
printf("IL PID:%d\n",getpid());
}
void f2(int sig){
printf("E ARRIVATO IL SEGNALE:%d\n",sig);
printf("IL PID:%d\n",getpid());
}

int main(){
pid_t pid[3];
pid_t figlio;
int i;
int valore;
signal(SIGUSR1,f);
signal(SIGINT,f2);
printf("Inizio del programma\n");
for(i=0;i<3;i++){
pid[i]=fork();
switch(pid[i]){
case -1:
perror("Fork non riuscita\n");
exit(1);
case 0:
if(i==0){
printf("SONO IL PROCESSO:%d\n",i);
printf("IL MIO PID:%d\n",getpid());
kill(getppid(),SIGUSR1);
exit(0);
}
if(i==1){
printf("SONO IL PROCESSO:%d\n",i);
printf("IL MIO PID:%d\n",getpid());
execlp("bash2.sh","bash2.sh","/home/gabriele/Scrivania/esameso","2",NULL);
exit(0);
}
if(i==2){
printf("SONO IL PROCESSO:%d\n",i);
printf("IL MIO PID:%d\n",getpid());
kill(getppid(),SIGINT);
exit(0);
}
}
if(pid[i]!=0){
figlio=wait(&valore);
if(WIFEXITED(valore))
printf("IL FIGLIO E USCITO CORRETTAMENTE CON VALORE:%d\n",WEXITSTATUS(valore));
else
printf("IL FIGLIO NON E USCITO CORRETTAMENTE");
}
}
}
