#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>

int sem_set(int semid,int valore);
int sem_down(int semid);
int sem_up(int semid);
void processo_figlio(int semid,char *shm);
#define SIZE 50
int main(void){
int semid;
int shmid;
char *shm;
char stringa[SIZE];
pid_t pid;
int i;
semid=semget((key_t)1234,1,0666|IPC_CREAT);
if(semid==-1){
perror("CREAZIONE NON RIUSCITA\n");
exit(1);
}
sem_set(semid,1);
shmid=shmget((key_t)1235,SIZE,0666|IPC_CREAT);
if(shmid==-1){
perror("CREAZIONE NON RIUSCITA\n");
exit(1);
}
shm=(char*)shmat(shmid,NULL,0);
//CREO IL FIGLIO//
pid=fork();
switch(pid){
case -1:
perror("ERRORE CREAZIONE FIGLIO\n");
exit(1);
case 0:
processo_figlio(semid,shm);
exit(0);
default:
break;
}
//CODICE PADRE//
for(i=0;i<5;i++){
sem_down(semid);
printf("Inserisci una stringa\n");
fgets(stringa,SIZE,stdin);
strcpy(shm,stringa);
sem_up(semid);
}
wait(NULL);
semctl(semid,0,IPC_RMID);
shmdt(shm);
shmctl(shmid,IPC_RMID,NULL);
printf("TUTTO E ANDATO CORRETTAMENTE\n");
exit(0);
}
void processo_figlio(int semid,char *shm){
int i;
int c;
for(i=0;i<5;i++){
sem_down(semid);
c=strlen(shm)-1;
printf("HO LETTO LA STRINGA:%s\n",shm);
printf("HO LETTO UN NUMERO DI CARATTERI:%d\n",c);
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
union semun a;
a.val=valore;
return semctl(semid,0,SETVAL,valore);
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

