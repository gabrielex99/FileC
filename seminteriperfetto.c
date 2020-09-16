//PERMETTE DI INSERIRE DEI NUMERI SE TALE NUMERO E PARI FA LA MOLTIPLICAZIONE PER UNA COSTANTE SE INVECE TALE NUMERO E DISPARI LO DIVIDE PER TALE COSTANTE E RESTITUISCE IL RISULTATO AL PROCESSO PRINCIPALE//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>


void processo_figlio(int semid,int *shm);
int sem_set(int semid,int valore);
int sem_up(int semid);
int sem_down(int semid);

int main(void){
int semid;
int shmid;
int *shm;
int i=0;
int b[50];
pid_t pid;

//1 CREO IL SEMAFORO//
semid=semget((key_t)1234,1,0666|IPC_CREAT);
if(semid==-1){
perror("ERRORE DURANTE LA CREAZIONE DEL SEMAFORO\n");
exit(1);
}
sem_set(semid,1);
//2 CREO LA SHAREDMEMORY//
shmid=shmget((key_t)1235,50,0666|IPC_CREAT);
if(shmid==-1){
perror("ERRORE DURANTE LA CREAZIONE DELLA SHAREDMEMORY");
exit(1);
}
//3 FACCIO L'ATTACK//
shm=(int*)shmat(shmid,NULL,0);
//CREO IL FIGLIO/FIGLI//
pid=fork();
switch(pid){
case -1:
perror("ERRORE DURANTE LA CREAZIONE DEI FIGLI\n");
exit(1);
case 0:
processo_figlio(semid,shm);
exit(0);
default:
break;
}
//CODICE PADRE//
do{
sem_down(semid);
if(i==5){
sem_up(semid);
break;
}

if(i!=0){
printf("LA I VALE:%d\n",i);
printf("IL NUMERO MODIFICATO:%d\n",shm[i-1]);
}

printf("Inserisci un numero\n");
scanf("%d",&b[i]);
shm[i]=b[i];
i++;
sem_up(semid);
}while(1);
//ASPETTO TERMINAZIONE FIGLIO,ELIMINO SEM,FACCIO LA DT ELIMINO SHM E FINISCO//
wait(NULL);
semctl(semid,0,IPC_RMID);
shmdt(shm);
shmctl(shmid,IPC_RMID,NULL);
printf("TUTTO E ANDATO CORRETTAMENTE\n");
exit(0);
}

void processo_figlio(int semid,int *shm){
int i=0;
do{
sem_down(semid);
if(i==5){
sem_up(semid);
break;
}
printf("Hai inserito il seguente valore:%d\n",*shm);
if(shm[i] %2==0){
printf("IL NUMERO E PARI\n");
shm[i]=shm[i]*2;
}
else{
printf("IL NUMERO E DISPARI\n");
shm[i]=shm[i]/2;
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
union semun pippo;
pippo.val=valore;
return semctl(semid,0,SETVAL,pippo);
}
int sem_up(int semid){
struct sembuf op;
op.sem_num=0;
op.sem_op=1;
op.sem_flg=SEM_UNDO;
return semop(semid,&op,1);
}
int sem_down(int semid){
struct sembuf op;
op.sem_num=0;
op.sem_op=-1;
op.sem_flg=SEM_UNDO;
return semop(semid,&op,1);
}
