//TALE PROGRAMMA PERMETTE AD UN PADRE DI GENERARE UN N PROCESSI FIGLI//
//IL PROCESSO PADRE OGNI VOLTA CHE GENERA UN FIGLIO INCREMENTA SOLO UNA VARIABILE A//
//IL FIGLIO SE LA VARIABILE A E PARI ESCE CON 0 SENNO ESCE CON 1//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int a = 0;
    pid_t pid;
    int valore;
    int i = 0;
    int exit_value;
    printf("INIZIO DEL PROGRAMMA\n");
    for (i = 0; i < 5; i++)
    {
        pid = fork();
        switch (pid)
        {
        case -1:
            perror("Fork non riuscita\n");
            exit(1);
            break;
        case 0:
            printf("IL PID DEL PROCESSO:%d\n", getpid());
            printf("IL PID DEL PROCESSO PADRE:%d\n", getppid());
            printf("LA VARIABILE A:%d\n", a);
            sleep(1);
            if (a % 2 == 0)
                exit_value = 0;
            else
                exit_value = 1;
                exit(exit_value);
            break;
        default:
            a++;
            break;
        }
    }
    pid_t figlio;
    if(pid!=0){
    while ((figlio = wait(&valore)) > 0) // QUI DEVI METTERE UN OPERATORE, POICHÉ ANCHE -1(valore di ritorno della funzione wait
    // in caso in cui non ci fossero figli sarebbe vera)
    {
        if (WIFEXITED(valore) != 0)
            printf("IL FIGLIO E USCITO CORRETTAMENTE CON:%d\n", WEXITSTATUS(valore));
        else
        {
            printf("IL FIGLIO NON E USCITO CORRETTAMENTE");
        }
}
    }
          
}
