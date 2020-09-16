//UN PROGRAMMA PER 3 VOLTE MANDA A 2 FIGLI DELLE STRINGHE ESSI CALCOLANO LA LUNGHEZZA E STAMPANO STRINGA CORTA STRINGA LUNGA,IN BASE A QUANTO SIA LUNGA TALE STRINGA//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
void processo_figlio(int semid,char *shm);
int sem_set(int semid,int valore);
int sem_up(int semid);
int sem_down(int semid);

int main(void){
int j=0;
int semid;
int shmid;
char *shm;
pid_t pid;
int i=0;
semid=semget((key_t)1234,1,0666|IPC_CREAT);
if(semid==-1){
perror("ERRORE DURANTE LA CREAZIONE\n");
exit(1);
}
sem_set(semid,1);
shmid=shmget((key_t)1235,50,0666|IPC_CREAT);
if(shmid==-1){
perror("ERRORE DURANTE LA CREAZIONE\n");
exit(1);
}
shm=(char*)shmat(shmid,NULL,0);
//CREO I FIGLI//
for(j=0;j<2;j++){
pid=fork();
switch(pid){
case -1:
perror("ERRORE DURANTE LA CREAZIONE\n");
exit(1);
case 0:
processo_figlio(semid,shm);
exit(0);
default:
break;
}
for(i=0;i<3;i++){
sem_down(semid);
printf("Inserisci una stringa\n");
fgets(shm,50,stdin);
sem_up(semid);
}
}
wait(NULL);
wait(NULL);
semctl(semid,0,IPC_RMID);
shmdt(shm);
shmctl(shmid,IPC_RMID,NULL);
printf("TUTTO E ANDATO CORRETTAMENTE\n");
exit(0);
}
void processo_figlio(int semid,char *shm){
int i=0;
int lun=0;
do{
sem_down(semid);
if(i==3){
sem_up(semid);
break;
}
lun=strlen(shm)-1;
printf("IL PID:%d\n",getpid());
printf("LA STRINGA INSERITA:%s\n",shm);
if(lun>5){
printf("STRINGA LUNGA\n");
}
else{
printf("STRINGA CORTA\n");
}
i++;
sem_up(semid);
}while(1);
exit(0);
}
 union semun {
               int              val;    /* Value for SETVAL */
               struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
               unsigned short  *array;  /* Array for GETALL, SETALL */
               struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
           };

int sem_set(int semid,int valore){
union semun ciccio;
ciccio.val=valore;
return semctl(semid,0,SETVAL,ciccio);
}
int sem_up(int semid){
struct sembuf op;
op.sem_num=0;
op.sem_flg=SEM_UNDO;
op.sem_op=1;
return semop(semid,&op,1);
}
int sem_down(int semid){
struct sembuf op;
op.sem_num=0;
op.sem_flg=SEM_UNDO;
op.sem_op=-1;
return semop(semid,&op,1);
}

