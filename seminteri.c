//UN PROGRAMMA IN CUI IL PADRE PASSA DEI VALORI INTERI ED L'ALTRO PROCESSO DETERMINA QUALE E IL MINORE RESTITUENDO AL PROCESSO PADRE//
//FINCHE NON INSERISCO IL VALORE 5 CICLA AL INFINITO//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

struct numeri{
	int num[10];
	float max;
};

void processo_figlio(int semid,struct numeri *info);
int sem_set(int semid,int valore);
int sem_up(int semid);
int sem_down(int semid);

int main(void){
int semid;
int i=0;
int shmid;
int j=0;
int a[50];
pid_t pid;
struct numeri *info;
//1 CREO IL SEMAFORO//
semid=semget((key_t)1234,1,0666|IPC_CREAT);
if(semid==-1){
perror("Errore nella creazione del semaforo\n");
exit(1);
}
sem_set(semid,1);

//CREO SHM//
shmid=shmget((key_t)1235,50,0666|IPC_CREAT);
if(shmid==-1){
perror("Errore durante la creazione\n");
exit(1);
}
info=(struct numeri *)shmat(shmid,NULL,0);

//CREO IL FIGLIO//
pid=fork();
switch(pid){
case -1:
perror("Errore durante la creazione\n");
break;
case 0:
processo_figlio(semid,info);
exit(0);
default:
break;
}
//CODICE PADRE/
do{
sem_down(semid);
printf("Inserisci un valore\n");
scanf("%d",&a[i]);
info->num[i]=a[i];
if(info->num[i]==5){
printf("IL MASSIMO:%f\n",info->max);
sem_up(semid);
break;
}
i++;
sem_up(semid);
}while(1);
//ASPETTO TERMINAZIONE DEL PROCESSO FIGLIO,ELIMINO LA SHM,IL SEMAFORO E FACCIO LA DT//
wait(NULL);
semctl(semid,0,IPC_RMID);
shmdt(info);
shmctl(shmid,IPC_RMID,NULL);
printf("TUTTO ANDATO CORRETTAMENTE\n");
exit(0);
}

void processo_figlio(int semid,struct numeri *info){
int i=0;
int max;
do{
sem_down(semid);
max=info->num[i];
if(info->num[i]==5){
sem_up(semid);
exit(0);
}
printf("Hai inserito:%d\n",info->num[i]);
if(info->num[i]>max){
max=info->num[i];
}
info->max=max;
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
