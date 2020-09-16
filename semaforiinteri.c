#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <wait.h>
#include <sys/shm.h>
#include <unistd.h>

int sem_set(int semid,int valore);
int sem_down(int semid);
int sem_up(int semid);
int time_to_exit=1;
int processo_figlio(int semid,int *shm);
int i=0;

int main(void){
pid_t pid;
int semid;
int shmid;
int *shm;
int valore;

semid=semget((key_t)1234,1,0666|IPC_CREAT);
if(semid==-1){
perror("CREAZIONE DEL SEMAFORO NON RIUSCITA\n");
exit(1);
}
sem_set(semid,1);
shmid=shmget((key_t)1234,50,0666|IPC_CREAT);
if(shmid==-1){
perror("CREAZIONE DELLA SHMEMORY NON RIUSCITA\n");
exit(1);
}
shm=(int*)shmat(shmid,NULL,0);
//CREO IL FIGLIO//
pid=fork();
switch(pid){
case -1:
perror("CREAZIONE DEL FIGLIO NON RIUSCITA\n");
exit(1);
case 0:
processo_figlio(semid,shm);
exit(0);
default:
break;
}
//CODICE DEL PADRE//
do{
sem_down(semid);
if(i!=0 && *shm==0){
break;
}
if(i!=0){
printf("Il valore nuovo della shared memory:%d\n",*shm);
}
printf("Inserisci un numero\n");
scanf("%d",&valore);
*shm=valore;
i++;                                                                                                   
sem_up(semid);
}while(1);
//ELIMINO AL SEMAFORO,FACCIO LA DETACH,ELIMINO LA SHARED MEMORY//
wait(NULL);
semctl(semid,0,IPC_RMID);
shmdt(shm);
shmctl(shmid,IPC_RMID,NULL);
printf("TUTTO E ANDATO CORRETTAMENTE\n");
}

 union semun {
               int              val;    /* Value for SETVAL */
               struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
               unsigned short  *array;  /* Array for GETALL, SETALL */
               struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
           };

int processo_figlio(int semid,int *shm){
int valore;
do{
sem_down(semid);
if(*shm==0){
break;
}
printf("IL NUMERO INSERITO:%d\n",*shm);
printf("Inserisci valore da sommare\n");
scanf("%d",&valore);
*shm=*shm+valore;
sem_up(semid);
}while(1);
exit(0);
}

int sem_set(int semid,int valore){
union semun ciccio;
ciccio.val=valore;
return semctl(semid,0,SETVAL,ciccio);
}
int sem_down(int semid){
struct sembuf op;
op.sem_num=0;
op.sem_op=-1;
op.sem_flg=SEM_UNDO;
return semop(semid,&op,1);
}
int sem_up(int semid){
struct sembuf op;
op.sem_num=0;
op.sem_op=+1;
op.sem_flg=SEM_UNDO;
return semop(semid,&op,1);
}

