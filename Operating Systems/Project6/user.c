#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h> 

void ProgramSignal_Close(int sig);
void ProgramClose();

void SharedClock_Setup();
#define CLOCKVAR 0
#define SharedMemName "/classes/OS/kaur/kaur.6/navTemp.txt"
int ClkSharedMemId;
int* ClkSharedPointer;

void MsgQueue_Setup();
#define QUEUEVAR 0
#define MsgQueueName "/classes/OS/kaur/kaur.6/navTemp2.txt"
int MsgQueue_ID;

struct MsgBuffer { 
    long mtype;
    pid_t pid;
    int Location;
    char ReadWrite;
} Msg;

int main (int argc, char *argv[]) {
    signal(SIGINT, ProgramSignal_Close);
    srand ( time(NULL) );

    // SharedClock_Setup();
    MsgQueue_Setup();

    int i;
    for(i = 0; i < 300; i++){
        Msg.mtype = 1;
        Msg.ReadWrite = (rand() % 2) == 0 ? 'r' : 'w';
        Msg.Location = rand() % 86870;
        Msg.pid = getpid();
        int msgSent = msgsnd(MsgQueue_ID, &Msg, sizeof(Msg), 0);
        if (msgSent < 0){
            printf("Child %d: failed to send Msg.\n", getpid());
            printf("Error: %d\n", errno);
            ProgramClose();
        }

        msgrcv(MsgQueue_ID, &Msg, sizeof(Msg), getpid(), 0);
    }

    // printf("Child %d: an error has occured.", getpid());
    // printf("Child %d: Recived permission to close\n.", getpid());
    ProgramClose();
}

void ProgramSignal_Close(int sig){
    ProgramClose();
}

void ProgramClose(){
    shmdt(ClkSharedPointer);
    // printf("Child %d Exiting gracefully.\n", getpid());
    exit(0);
}

void SharedClock_Setup(){
    key_t ClockKey_Shared;
    if (-1 != open(SharedMemName, O_CREAT, 0777)) {
        ClockKey_Shared = ftok(SharedMemName, CLOCKVAR);
    } else {
        printf("ftok error in child: SharedClock_Setup\n");
        printf("Error: %d\n", errno);
        exit(1);
    }

    ClkSharedMemId = shmget(ClockKey_Shared, sizeof(int)*2, IPC_CREAT | 0777);
    if (ClkSharedMemId < 0) {
        printf("shmget error in child: SharedClock_Setup\n");
        printf("Error: %d\n", errno);
        exit(1);
    }

    ClkSharedPointer = (int *) shmat(ClkSharedMemId, NULL, 0);
    if ((long) ClkSharedPointer == -1) {
        printf("shmat error in child: SharedClock_Setup\n");
        printf("Error: %d\n", errno);
        shmctl(ClkSharedMemId, IPC_RMID, NULL);
        exit(1);
    }
}

void MsgQueue_Setup(){
    key_t MsgQueue_Key;
    if (-1 != open(MsgQueueName, O_CREAT, 0777)) {
        MsgQueue_Key = ftok(SharedMemName, QUEUEVAR);
    } else {
        printf("ftok error in child: MsgQueue_Setup\n");
        printf("Error: %d\n", errno);
        exit(1);
    }

    MsgQueue_ID = msgget(MsgQueue_Key, 0777 |IPC_CREAT);
    if (MsgQueue_ID < 0) {
        printf("msgget error in child: MsgQueue_Setup\n");
        printf("Error: %d\n", errno);
        exit(1);
    }
}