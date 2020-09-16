#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <sys/shm.h>

void processo_figlio(int semid,char *shm);
int sem_set(int semid,int valore);
int sem_down(int semid);
int sem_up(int semid);

int main(void){
int i=0;
int semid;
int shmid;
char *shm;
char stringa[50];
pid_t pid;
semid=semget((key_t)1234,1,0666|IPC_CREAT);
if(semid==-1){
perror("CREAZIONE SEMAFORO NON RIUSCITA\n");
exit(1);
}
sem_set(semid,1);
shmid=shmget((key_t)1235,50,0666|IPC_CREAT);
if(shmid==-1){
perror("CREAZIONE SHARED MEMORY NON RIUSCITA\n");
exit(1);
}
shm=(char*)shmat(shmid,NULL,0);
//CREO I FIGLI//
pid=fork();
switch(pid){
case -1:
perror("CREAZIONE FIGLIO NON RIUSCITA\n");
exit(1);
case 0:
processo_figlio(semid,shm);
exit(0);
default:
break;
}
//CODICE DEL PROCESSO PADRE//
do{
sem_down(semid);
if(strncmp(stringa,"ESCI",4)==0){
sem_up(semid);
break;
}
if(i!=0){
printf("LA STRINGA MODIFICATA:%s\n",shm);
}
printf("Inserisci una stringa\n");
fgets(stringa,50,stdin);
strcpy(shm,stringa);
i++;
sem_up(semid);
}while(1);
wait(NULL);
semctl(semid,0,IPC_RMID);
shmdt(shm);
shmctl(shmid,IPC_RMID,NULL);
printf("TUTTO E ANDATO CORRETTAMENTE\n");
}
void processo_figlio(int semid,char *shm){
int c=0;
int i=0;
do{
sem_down(semid);
if(strncmp(shm,"ESCI",4)==0){
sem_up(semid);
break;
}
c=strlen(shm)-1;
i++;
printf("HO LETTO LA SEGUENTE STRINGA:%s\n",shm);
printf("ESSA E LUNGA:%d\n",c);
strcat(shm,"PORCO");
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
op.sem_op=+1;
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
