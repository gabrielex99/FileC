//INSERISCO DEI NUMERI FINO A 10 INTERROGAZIONI,DOPO 10 INTERROGAZIONI STAMPO LA MEDIA CALCOLATAMO DAL PROCESSO FIGLIO TUTTO CIO VIENE ESEGUITA DA 2 PROCESSI//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

struct mess{
int valori[50];
float media;
};

void processo_figlio(int semid,struct mess *shm);
int sem_set(int semid,int valore);
int sem_up(int semid);
int sem_down(int semid);


int main(void){
int semid;
int shmid;
int j=0;
pid_t pid;
int a[50];
int i=0;
struct mess *shm;

//1 CREO IL SEMAFORO//
semid=semget((key_t)1234,1,0666|IPC_CREAT);
if(semid==-1){
perror("ERRORE DURANTE LA CREAZIONE DEL SEMAFORO\n");
exit(1);
}
sem_set(semid,1);
//3 CREO LA SHARED MEMORY//
shmid=shmget((key_t)1235,50,0666|IPC_CREAT);
if(shmid==-1){
perror("ERRORE DURANTE LA CREAZIONE DELLA SHM\n");
exit(1);
}
//4 FACCIO L'ATTACK//
shm=(struct mess*)shmat(shmid,NULL,0);
//5 CREO I FIGLI//
for(j=0;j<2;j++){
pid=fork();
switch(pid){
case -1:
perror("ERRORE NELLA CREAZIONE DEL FIGLIO\n");
exit(1);
case 0:
processo_figlio(semid,shm);
exit(0);
default:
break;
}
}
do{
for(j=0;j<2;j++){
sem_down(semid);
printf("Inserisci un valore\n");
scanf("%d",&a[i]);
shm->valori[i]=a[i];
i++;
sem_up(semid);
}
}while(i<3);//COSI PER 3 VOLTE MANDO AI 2 FIGLI I NUMERI//
//ASPETTO LA TERMINAZIONE DEL FIGLIO,ELIMINO IL SEMAFORO,FACCIO LA DT,ELIMINO LA SHM//
wait(NULL);
wait(NULL);
semctl(semid,0,IPC_RMID);
shmdt(shm);
shmctl(shmid,IPC_RMID,NULL);
printf("TUTTO ANDATO CORRETTAMENTE\n");
exit(0);
}

void processo_figlio(int semid,struct mess *shm){
sleep(1);
int somma;
float media;
int i=0;
do{
sem_down(semid);
if(i==3){
sem_up(semid);
break;
}
printf("Hai inserito:%d\n",shm->valori[i]);
somma=somma+shm->valori[i];
i++;
sem_up(semid);
}while(1);
shm->media=somma/i;
printf("LA MEDIA:%f\n",shm->media);
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

