#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
//PROGRAMMA CHE PERMETTE LA SIMULAZIONE DI COMANDI INVIATI AD DEI BRACCI MOTORI//

//QUEESTO LO COPIO DA SEM_CTL//
union semun {
    int              val;
    struct semid_ds *buf;    
    unsigned short  *array;
    struct seminfo  *__buf;
};

//PROTOTIPI DELLE FUNZIONI CHE MI SERVONO//
int sem_set(int semid, int val);
int sem_up(int semid);
int sem_down(int semid);

void processo_figlio(int semid,char *shm);

#define SEMKEY (key_t)3456
#define SHMKEY (key_t)4456
#define CMDSIZE 50

int main() {
    int semid;//ID SEM//
    int shmid;//ID SHM//
    char *shm;//CONTENUTO DELLA SHM ED E UNA STRINGA//
    int i, idx;//INDICI//
    pid_t pidf;//PID PER LA FORK//
    char *comandi[] = {"Avanti", "Indietro", "Sinistra", "Destra", "Stop"};
    
    semid = semget(SEMKEY, 1, 0666 | IPC_CREAT);//CREAZIONE SEMAFORO//
    sem_set(semid, 1);//INIZIALIZZAZIONE SEMAFORO//
    
    shmid = shmget(SHMKEY, CMDSIZE, 0666 | IPC_CREAT);//CREAZIONE SHM//
    
    shm = (char *)shmat(shmid, NULL, 0);//COLLEGAMENTO DELLA SHM//

//TUTTO CIO CHE RIGUARDA INIZIALIZZAZIONE DI SEMAFORI E SHARED MEMORY VA MESSO PRIMA DEL FORK O FOR CON LE VARIE FORK//
    
    for(i=0; i<2; i++) {
        pidf = fork();
        switch(pidf) {
            case 0:
                processo_figlio(semid,shm);
                exit(0);
            default:
                break;
        }
    }
    
    /** Codice del processo Padre */
    srand(pidf);
    
    for(i=0; i<20; i++) {
        idx = rand()%5;
        sem_down(semid);//BLOCCO IL SEMAFORO//
        printf("MANDO UN COMANDO AI 2 MOTORI\n");
        strcpy(shm, comandi[idx]);
        sem_up(semid);//RILASCIO IL SEMAFORO//
        sleep(2);
    }
    
    sem_down(semid);//BLOCCO IL SEMAFORO//
    strcpy(shm, "Termina");
    sem_up(semid);//RILASCIO IL SEMAFORO//
    //ASPETTO I 2 PROCESSI//
    wait(NULL);
    wait(NULL);
    //ELIMINO SEMAFORO,FACCIO LA DETACH ED CANCELLO LA SHARED MEMORY//
    semctl(semid, 0, IPC_RMID);
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
}

void processo_figlio(int semid, char* shm) {
    char cmdString[CMDSIZE];
    do{
        sem_down(semid);//BLOCCO SEMAFORO//
        if (strcmp(cmdString, shm) != 0) {
            strcpy(cmdString, shm);
            printf("IL MOTORE HA RICEVUTO IL SEGUENTE COMANDO%s\n", cmdString);
            fflush(stdout);
        }
        sem_up(semid);//SBLOCCO IL SEMAFORO//
        sleep(1);
    }while(strcmp(cmdString, "Termina") != 0);
}




int sem_set(int semid, int val) {
    union semun valore;
    valore.val = val;
    return semctl(semid, 0, SETVAL, valore);
}

int sem_up(int semid) {
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = 1;
    op.sem_flg = SEM_UNDO;
    return (semop(semid, &op, 1));
}

int sem_down(int semid) {
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = SEM_UNDO;
    return (semop(semid, &op, 1));
}
