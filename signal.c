//FARE UN PROGRAMMA IN CUI IL PADRE MANDA UN SEGNALE SIGUSR1 AL FIGLIO ESSO APPENA LO RICEVE STAMPA IL PROPRIO PID E MANDA AL PADRE UN SEGNALE SIGURS2,APPENA RICEVUTO TALE SEGNALE VERRA STAMPATO IL PID E BONU CHIU//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
void f_figlio(int sig){
  printf("Sono il figlio con pid:%d\n",getpid());
  kill(getppid(),SIGUSR2);//OVVERO IL FIGLIO MANDA UN SEGNALE AL PADRE SIGUSR2
}

void f_padre(int sig){
  printf("Sono il padre,con pid:%d\n",getpid());
}

int main(){
pid_t pid;
int valore;
pid=fork();
signal(SIGUSR1,f_figlio);//QUANDO RICEVO SIGUSR1 INVOCO LA FUNZIONE FIGLIO CHE MANDA UN SIGUSR2 E STAMPA IL PID//
signal(SIGUSR2,f_padre);//QUANDO RICEVO SIGUSR2 CHIAMA LA FUNZIONE PADRE//
switch(pid){
case -1:
perror("Fork non riuscita\n");
exit(1);
case 0://Sono nel processo figlio//
pause();//BLOCCA IL PROCESSO ASPETTANDO L'ARRIVO DI UN SEGNALE//
break;
default://Sono nel processo padre//
printf("Il mio pid:%d\n",getpid());
printf("IL PID DEL FIGLIO:%d\n",pid);
kill(pid,SIGUSR1);//MANDO AL FIGLIO UN SEGNALE SIGUSR1//
}
pid_t figlio;
if(pid!=0){
figlio=wait(&valore);
if(WIFEXITED(valore)!=0)
printf("Il figlio è uscito correttamente con valore:%d\n",WEXITSTATUS(valore));
else{
printf("Il figlio non è uscito correttamente");
}
}
}
