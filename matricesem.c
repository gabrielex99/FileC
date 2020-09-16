#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/shm.h>


int sem_set(int semid,int valore);
int sem_down(int semid);
int sem_up(int semid);
void processo_figlio(int semid, int *shm,int *shm2);



int main(){
int semid;
int shmid;
int shmid2;
int *shm;
int *shm2;
int i=0;
pid_t pid;
semid=semget((key_t)1234,1,0666|IPC_CREAT);
if(semid==-1){
perror("Creazione semaforo non riuscita\n");
exit(1);
}
sem_set(semid,1);
shmid=shmget((key_t)1233,50,0666|IPC_CREAT);
if(shmid==-1){
perror("Creazione shm non riuscita\n");
exit(1);
}
shmid2=shmget((key_t)1235,50,0666|IPC_CREAT);
if(shmid2==-1){
perror("CREAZIONE SHM2 NON RIUSCITA\n");
exit(1);
}
shm2=(int*)shmat(shmid, NULL, 0);
shm=(int*)shmat(shmid, NULL, 0);//ATTACK DELLA SHM//
for(i=0;i<5;i++){
sem_down(semid);
shm[i]=shm[i]+1;
sem_up(semid);
}
//POSSO CREARE IL PROCESSO FIGLIO//
pid=fork();
switch(pid){
case -1:
perror("Creazione errata del figlio\n");
exit(1);
case 0:
processo_figlio(semid,shm,shm2);
exit(0);
default:
break;
}
//CODICE DEL PADRE//

for(i=0;i<5;i++){
sem_down(semid);
printf("LA SOMMA VALE:%d\n",shm2[i]);
sem_up(semid);
}

wait(NULL);//ASPETTO LA FINE DEL PROCESSO//
semctl(semid, 0, IPC_RMID);
    shmdt(shm);
    shmdt(shm2);
    shmctl(shmid, IPC_RMID, NULL);
}

void processo_figlio(int semid, int *shm,int *shm2){
sleep(1);
int j=0;
int somma=0;
for(j=0;j<5;j++){
sem_down(semid);
shm2[j]=shm2[j]+shm[j];
printf("La MEMORIA CONDIVISA VALE:%d\n",shm2[j]);
sem_up(semid);
}
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
union semun pippo;
pippo.val=valore;
return semctl(semid,0,SETVAL,pippo);
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

