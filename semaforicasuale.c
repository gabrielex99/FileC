//PROGRAMMA IN CUI UN THREAD MANDA DEI COMANDI CASUALMENTE,AD 2 ALTRI  PROCESSI FIGLI DOPO 20 INTERROGAZIONI SI COPIA TERMINA ED ESSO FINISCE,SI ASPETTA LA TERMINAZIONE DEL PROCESSO//
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <string.h>
#define KEY (key_t)1234
#define KEY1 (key_t)1235
void processo_figlio(int semid,char *shm);
int sem_set(int semid,int valore);
int sem_down(int semid);
int sem_up(int semid);

int main(void){
int semid;
int shmid;
int j=0;
int i;
int k=0;
char *shm;
char *comandi[]={"AVANTI","DESTRA","SINISTRA","INDIETRO","STOP"};
pid_t pid;

//1 CREO IL SEMAFORO//
semid=semget(KEY,1,0666|IPC_CREAT);
if(semid==-1){
perror("ERRORE DURANTE LA CREAZIONE DEL SEMAFORO\n");
exit(1);
}
sem_set(semid,1);
//3 CREO LA SHARED MEMORY//
shmid=shmget(KEY1,50,0666|IPC_CREAT);
if(shmid==-1){
perror("ERRORE DURANTE LA CREAZIONE DELLA SHM\n");
exit(1);
}
//4 FACCIO L'ATTACK//
shm=(char*)shmat(shmid,NULL,0);
//5 CREO I FIGLI//
for(i=0;i<2;i++){
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
}
//CODICE DEL PADRE//
srand(pid);
do{
j=rand()%5;
sem_down(semid);
if(k==20){
strcpy(shm,"TERMINA");
sem_up(semid);
break;
}
strcpy(shm,comandi[j]);
k++;
sem_up(semid);
}while(1);
wait(NULL);
wait(NULL);
semctl(semid,0,IPC_RMID);
shmdt(shm);
shmctl(shmid,IPC_RMID,NULL);
printf("TUTTO ANDATO CORRETTAMENTE\n");
exit(0);
}

void processo_figlio(int semid,char *shm){
int lun;
int k;
do{
sem_down(semid);
if(strncmp(shm,"TERMINA",7)==0){
sem_up(semid);
break;
}
printf("Il mio pid:%d\n",getpid());
lun=strlen(shm)-1;
printf("La stringa inserita:%s\n",shm);
printf("La sua lunghezza:%d\n",lun);
k++;
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

