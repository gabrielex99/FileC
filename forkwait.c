#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(){
pid_t pid;
int n;
char*message;
int val;
int exit_value;
printf("Inizio del programma\n");
pid=fork();
switch(pid){
case -1:
perror("Fork non ha avuto successo");
exit(1);
case 0:
message="Sono il processo figlio";
n=3;
exit_value=37;
break;
default:
message="Sono il processo padre";
n=5;
exit_value=52;
break;
}
for(;n>0;n--){
puts(message);
printf("IL PID:%d\n",getpid());
sleep(1);
}
if(pid!=0){ //VIENE ESEGUITO SOLTANTO DAL PADRE//
pid_t  child_pid;
child_pid=wait(&val);  /* chiamata della wait() -  è bloccante */
printf("IL FIGLIO HA FINITO: PID= %d\n",child_pid);
if (WIFEXITED(val)!=0)
printf("IL CODICE DI USCITA DEL FIGLIO È %d\n",WEXITSTATUS(val)); 
else
printf("FIGLIO NON HA FINTIO CORETTAMENTE\n"); 
} /* fine codice eseguito solo dal padre */
exit(exit_value); 
}





