#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void f_figlio2(int sig){
printf("IL PID DEL PADRE:%d\n",getpid());
printf("HO RICEVUTO IL SEGNALE NUMERO:%d\n",sig);
}
void f_figlio1(int sig){
printf("IL PID DEL PADRE:%d\n",getpid());
printf("HO RICEVUTO IL SEGNALE NUMERO:%d\n",sig);
}

void f_figlio3(int sig){
printf("IL PID DEL PADRE:%d\n",getpid());
printf("HO RICEVUTO IL SEGNALE NUMERO:%d\n",sig);

}

//UN PADRE GENERA 3 FIGLI ESSI MANDANDO UN SEGNALE SIGUSR1 AL PADRE,OGNI VOLTA CHE IL PADRE LO RICEVE STAMPA IL NUMERO DEL SEGNALE RICEVUTO,E ASPETTA CHE FINISCONO TUTTI E 3//

int main(){
pid_t p[3];
pid_t figlio;
int i;
int valore;
signal(SIGUSR2,f_figlio2);
signal(SIGUSR1,f_figlio1);
signal(SIGINT,f_figlio3);
printf("Inizio del programma\n");
for(i=0;i<3;i++){
p[i]=fork();
switch(p[i]){
case -1:
perror("Fork non riuscita\n");
exit(1);
case 0://SIAMO NEL FIGLIO//
printf("Sono il processo:%d\n",i);
printf("IL MIO PID:%d\n",getpid());
sleep(1);
if(i==0){
kill(getppid(),SIGUSR2);
}
if(i==1){
kill(getppid(),SIGUSR1);
}
if(i==2){
kill(getppid(),SIGINT);
}
exit(0);
default:
break;
}
}

for(i=0;i<3;i++){
if(p[i]!=0){
figlio=wait(&valore);
if(WIFEXITED(valore)!=0)
printf("IL FIGLIO HA FINITO CORRETTAMENTE:%d\n",WEXITSTATUS(valore));
else
printf("Il figlio non ha finito correttamente");
}
}
}
