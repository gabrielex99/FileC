#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/shm.h>
void processo_figlio(int semid,int *shm);
int sem_set(int semid,int valore);
int sem_up(int semid);
int sem_down(int semid);


int main(void){
int semid;
int shmid;
int *shm;
pid_t pid;
int i;
int j;
//1 CREO IL SEMAFORO//
semid=semget((key_t)1234,1,0666|IPC_CREAT);
if(semid==-1){
perror("ERRORE DURANTE LA CREAZIONE DEL SEMAFORO\n");
exit(1);
}
sem_set(semid,1);
//2 CREO LA SHM//
shmid=shmget((key_t)1235,50,0666|IPC_CREAT);
if(shmid==-1){
perror("ERRORE DURANTE LA CREAZIONE DEL SHM\n");
exit(1);
}
shm=(int*)shmat(shmid,NULL,0);
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
//PROCESSO PADRE//
for(i=0;i<3;i++){
sem_down(semid);
printf("Inserisci un valore:\n");
scanf("%d",&shm[i]);
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

void processo_figlio(int semid,int *shm){
int i;
int max;
do{
sem_down(semid);
if(i==3){
sem_up(semid);
break;
}
printf("IL PID:%d\n",getpid());
printf("IL VALORE INSERITO:%d\n",shm[i]);
if(shm[i]>max){
max=shm[i];
}
i++;
sem_up(semid);
}while(1);
printf("IL MASSIMO:%d\n",max);
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
