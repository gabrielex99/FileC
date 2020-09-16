//SCRIVERE PROGRAMMA IN C IN CUI UN PADRE,CREA UN PROCESSO FIGLIO,IL PROCESSO FIGLIO DEVE STAMPARE AL INFINITO UNA STRINGA SONO IL PROCESSO FIGLIO,DOPO 10 SECONDI SI INVIA UN SEGNALE E SI TERMINA,IL PROCESSO PADRE ATTENDE LA SUA TERMINAZIONE//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(){
pid_t pid;
pid_t figlio;
int valore;
printf("Inizio del programma\n");
pid=fork();
switch(pid){
case -1:
perror("Fork non riuscita\n");
exit(1);
case 0:
alarm(5);
while(1){
printf("Sono il processo padre\n");
}
exit(0);
default:
printf("Sono il processo padre:%d\n",getpid());
}
if(pid!=0){
figlio=wait(&valore);
if(WIFEXITED(valore))
printf("IL FIGLIO HA FINITO CON VALORE:%d\n",WEXITSTATUS(valore));
else
printf("IL FIGLIO NON HA FINITO CORRETTAMENTE");
}


}
