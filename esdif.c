/* Un processo padre P genera 2 processi figli P1 e P2, ogni volta che il padre genera un figlio si salva il PID.
Faranno una staffetta, ogni figlio si mette in attesa di un segnale SIGUSR1, ogni volta che al figlio gli arriva questo segnale stampa a schermo il proprio PID e invia un segnale SIGUSR2 al padre.
Il padre ciclicamente interroga prima un figlio e poi l'altro inviandogli il segnale SIGUSR1, quando riceve SIGUSR2 stampa a video il proprio PID.
Dopo 10 interrogazioni, invia a tutti SIGQUIT attende la terminazione dei figli (che non terminano normalmente) stampa lo stato di terminazione e termina.
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

void f(int sig){
printf("IL PID DEL FIGLIO:%d\n",getpid());
fflush(stdout);
kill(getppid(),SIGUSR2);
}
void f2(int sig){
printf("IL PID DEL PADRE:%d\n",getpid());
fflush(stdout);
}

int main(){
pid_t p[2];
int i;
int valore;
pid_t figlio;
int j;
signal(SIGUSR1,f);
signal(SIGUSR2,f2);
printf("INIZIO DEL PROGRAMMA\n");
for(i=0;i<2;i++){
p[i]=fork();
switch(p[i]){
case -1:
perror("Fork non riuscita\n");
exit(1);
case 0:
printf("Sono il processo figlio:%d\n",getpid());
while(1){
pause();
}
exit(0);
default:
break;
}
}
for(j=0;j<10;j++){
for(i=0;i<2;i++){
kill(p[i],SIGUSR1);
sleep(1);
}
}


for(i=0;i<2;i++){
kill(p[i],SIGQUIT);
if(p[i]!=0){
figlio=wait(&valore);
if(WIFEXITED(valore))
printf("IL FIGLIO HA FINITO CORRETTAMENTE CON VALORE:%d\n",WEXITSTATUS(valore));
else
printf("IL FIGLIO NON HA FINITO CORRETTAMENTE\n");
}
}
}
