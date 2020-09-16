//1 PROCESSO PADRE,CREA 5 FIGLI,CHE ACCEDONO CONTEMPORANEAMENTE ALLA SHM,INOLTRE AL INFINITO LEGGONO DAL PADRE LA STRINGA MANDATA AL INFINITO,QUANDO VIENE DIGITATO IL COMANDO CONCATENA,CONCATENA UNA STRINGA//
//DAMIANO 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
void *processo_figlio(int semid,char *shm);
int sem_set(int semid,int valore);
int sem_down(int semid);
int sem_up(int semid);

int main(void){
int semid;
int shmid;
int i;
char stringa1[200];
char *shm;
pid_t pid;

//1 CREO IL SEMAFORO//
semid=semget((key_t)1234,1,0666|IPC_CREAT);
if(semid==-1){
perror("ERRORE DURANTE LA CREAZIONE DEL SEMAFORO/n");
exit(1);
}
//2 INIZIALIZZO//
sem_set(semid,1);
//CREO LA SHM//
shmid=shmget((key_t)1246,50,0666|IPC_CREAT);
if(shmid==-1){
perror("Errore durante la creazione della shm\n");
exit(1);
}
//FACCIO L AT//
shm=(char*)shmat(shmid,NULL,0);
//CREO I FIGLI//
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
//CODICE PADRE//
do{
sem_down(semid);
if(strncmp(shm,"ESCI",4)==0){
sem_up(semid);
break;
}
printf("Inserisci un comando\n");
fgets(stringa1,200,stdin);
strcpy(shm,stringa1);
sem_up(semid);
}while(1);
//ELIMINO IL SEM,LA SHM,FACCIO,DT E ASPETTO IL FIGLIO//
wait(NULL);
wait(NULL);
semctl(semid,0,IPC_RMID);
shmdt(shm);
shmctl(shmid,IPC_RMID,NULL);
printf("TUTTO E ANDATO CORRETTAMENTE\n");
exit(0);
}

void *processo_figlio(int semid,char *shm){
sleep(1);
int lun=0;
int i=0;
char stringa2[200];
do{
sem_down(semid);
if(strncmp(shm,"ESCI",4)==0){
sem_up(semid);
break;
}
lun=strlen(shm)-1;
printf("IL COMANDO INSERITO:%s\n",shm);
printf("LA SUA LUNGHEZZA:%d\n",lun);
if(strncmp(shm,"CONCATENA",9)==0){
printf("Inserisci una stringa\n");
fgets(stringa2,200,stdin);
strcat(shm,stringa2);
printf("La stringa modificata:%s\n",shm);
}
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
