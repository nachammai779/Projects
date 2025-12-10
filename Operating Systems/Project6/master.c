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
#include "queue.c"

void ChildSignal_Closed(int sig);
int Child_Close();
void ProgramSignal_Close(int sig);
void ProgramClose();

void OutputFile_Setup();

void TimeGap();

void Process_Creations();

void SharedClock_Setup();
#define CLOCKVAR 0
#define SharedMemName "/classes/OS/kaur/kaur.6/navTemp.txt"
int ClkSharedMemId;
int* ClkSharedPointer;

void MsgQueue_Setup();
#define QUEUEVAR 0
#define MsgQueueName "/classes/OS/kaur/kaur.6/navTemp2.txt"
int MsgQueue_ID;

void Msg_Receive();

struct MsgBuffer { 
    long mtype;
    pid_t pid;
    int Location;
    char ReadWrite;
} Msg;

FILE* outputFile;

int Process_Current;
pid_t Process_Open[18] = {0};
pid_t blockedProcesses[18] = {0};
int Process_Max;

struct BlockMemory {
    int pid;
    int Bit_Refer;
    int Bit_Dirt;
    int Bit_Read;
    int Bit_Write;
};

#define Frames_total 10
int PAGE[18][32] = {0};
int Frames_Open[Frames_total] = {0};
struct BlockMemory frames[Frames_total];

int NextProcess_CreationAt = -1;

int FrameSpace_Free();

struct MsgQueue* Queue_Page;

int main (int argc, char *argv[]) {
    srand ( time(NULL) );
    //set signals
    signal(SIGCHLD, ChildSignal_Closed);
    signal(SIGINT, ProgramSignal_Close);

    //set default values and get command line inputs
    int c;
    int RunTime_max = 20;
    int Process_Max = 18;
    char* logFile = "logFile.txt";

    while ((c = getopt (argc, argv, "hs:l:t:")) != -1){
        switch (c){
            case 'h':
                printf("Help Options:\n-h: Help\n-l: The given argument(string) specifies the name of the logfile.\n-t: The given number(int) specifies the max amount of time the program will run for.\n");
                exit(0);
                break;
            case 's':
                if (atoi(optarg) <= 0 || atoi(optarg) > 18){
                    Process_Max = 18;
                } else {
                    Process_Max = atoi(optarg);
                }
                break;
            case 'l':
                logFile = optarg;
                break;
            case 't':
                RunTime_max = atoi(optarg);
                break;
            default:
                printf("there was an error with arguments");
                exit(1);
                break;
        }
    }

    //setup output file
    OutputFile_Setup();

    //display run parameters
    printf("Log file name: %s\n", logFile);
    fprintf(outputFile, "Log file name: %s\n", logFile);
    printf("Max run time: %d\n", RunTime_max);
    fprintf(outputFile, "Max run time: %d\n", RunTime_max);

    //Intilize various shared memory
    SharedClock_Setup();
    ClkSharedPointer[0] = 0;
    ClkSharedPointer[1] = 0;

    MsgQueue_Setup();

    Queue_Page = createQueue(Frames_total);

    while(ClkSharedPointer[0] < 1){
    // while(1==1){
        if ((Process_Current < Process_Max)){
            Process_Creations();
        }
        TimeGap();
        Msg_Receive();
    }

    ProgramClose();
}

void ChildSignal_Closed(int sig){
    Child_Close();
}

int Child_Close(){
    pid_t closedChild = wait(NULL);
    if (closedChild > 0){
        int i;
        for(i = 0; i < 18; i++){
            if (Process_Open[i] == closedChild){
                Process_Open[i] = 0;
            }
        }
        Process_Current--;
    }
    return closedChild;
}

void ProgramSignal_Close(int sig){
    ProgramClose();
}

void ProgramClose(){
    msgctl(MsgQueue_ID, IPC_RMID, NULL);
    shmctl(ClkSharedMemId, IPC_RMID, NULL);
    // shmdt(ClkSharedPointer);
    fclose(outputFile);
    int i;
    for(i = 0; i < 18; i++){
        if (Process_Open[i] != 0){
            kill(Process_Open[i], SIGINT);
        }
    }
    printf("Exiting.\n");
    while (Child_Close() > 0){}
    exit(0);
}

void OutputFile_Setup(){
    char* logFile = "logFile.txt";
    outputFile = fopen(logFile, "w");
    if (outputFile == NULL){
        printf("Failed to open output file.\n");
        fprintf(outputFile, "Failed to open output file.\n");
        ProgramClose();
    }
}

void SharedClock_Setup(){
    key_t ClockKey_Shared;
	
	
	//ClockKey_Shared= open(SharedMemName, O_CREAT, 0666);
	//if (ClkSharedMemId != -1) {
    if (-1 != open(SharedMemName, O_CREAT, 0666)) {
        ClockKey_Shared = ftok(SharedMemName, CLOCKVAR);
    } else {
        printf("ftok error in parrent: SharedClock_Setup\n");
        printf("Error: %d\n", errno);
        exit(1);
    }

    ClkSharedMemId = shmget(ClockKey_Shared, sizeof(int)*2, IPC_CREAT | 0666);
    if (ClkSharedMemId < 0) {
        printf("shmget error in parrent: SharedClock_Setup\n");
        printf("Error: %d\n", errno);
        exit(1);
    }

    ClkSharedPointer = (int *) shmat(ClkSharedMemId, NULL, 0);
    if ((long) ClkSharedPointer == -1) {
        printf("shmat error in parrent: SharedClock_Setup\n");
        printf("Error: %d\n", errno);
        shmctl(ClkSharedMemId, IPC_RMID, NULL);
        exit(1);
    }
}

void TimeGap(){
    // ClkSharedPointer[0] += 1;
    // ClkSharedPointer[1] += rand() % 1000;
    ClkSharedPointer[1] += 1000;
    while (ClkSharedPointer[1] >= 1000000000){
        ClkSharedPointer[1] -= 1000000000;
        ClkSharedPointer[0]++;
        printf("%d:%d\n", ClkSharedPointer[0], ClkSharedPointer[1]);
        fprintf(outputFile, "%d:%d\n", ClkSharedPointer[0], ClkSharedPointer[1]);
    }
}

void Process_Creations(){
    if (NextProcess_CreationAt < 0){
        int randNumber = 1000;
        NextProcess_CreationAt = randNumber + ClkSharedPointer[1];
        // printf("next process at %d seconds\n", NextProcess_CreationAt);
    }

    if ((ClkSharedPointer[1] > NextProcess_CreationAt) && (NextProcess_CreationAt > 0)){
        int i;
        int openSpace;
        for(i = 0; i < 18; i++){
            if (Process_Open[i] == 0){
                openSpace = i;
                break;
            }
        }
        pid_t newForkPid;
        newForkPid = fork();
        if (newForkPid == 0){
            execlp("./user","./user", NULL);
            fprintf(stderr, "Failed to exec user!\n");
            fprintf(outputFile, "Failed to exec user!\n");
            exit(1);
        }
        Process_Open[openSpace] = newForkPid;
        NextProcess_CreationAt = -1;
        //printf("Process Created P %d\n", Process_Open[openSpace]);
        //fprintf(outputFile, "Process Created P %d\n", Process_Open[openSpace]);
        Process_Current++;
    }
}

void MsgQueue_Setup(){
    key_t MsgQueue_Key;
    if (-1 != open(MsgQueueName, O_CREAT, 0777)) {
        MsgQueue_Key = ftok(SharedMemName, QUEUEVAR);
    } else {
        printf("ftok error in parrent: MsgQueue_Setup\n");
        printf("Error: %d\n", errno);
        exit(1);
    }

    MsgQueue_ID = msgget(MsgQueue_Key, (IPC_CREAT | 0777));
    if (MsgQueue_ID < 0) {
        printf("msgget error in parrent: SharedClock_Setup\n");
        printf("Error: %d\n", errno);
        exit(1);
    }
}

void Msg_Receive(){
    int msgRecived  = msgrcv(MsgQueue_ID, &Msg, sizeof(Msg), 1, IPC_NOWAIT);
    if (msgRecived == -1){
        return;
    }

    pid_t PID_Requesting = Msg.pid;
    int requestedAddress = Msg.Location;
    int requestedPage = requestedAddress / 1024;
    char ReadWrite = Msg.ReadWrite;

    printf("Master: P%d requesting read of address %d at time %d:%d\n", PID_Requesting, requestedAddress, ClkSharedPointer[0], ClkSharedPointer[1]);
    fprintf(outputFile, "Master: P%d requesting read of address %d at time %d:%d\n", PID_Requesting, requestedAddress, ClkSharedPointer[0], ClkSharedPointer[1]);

    int j;
    int Process_Location;
    for(j = 0; j < 18; j++){
        if (Process_Open[j] == PID_Requesting){
            Process_Location = j;
            break;
        }
    }

    if (PAGE[Process_Location][requestedPage] == 0){
        printf("Master: Address %d is not in a frame, pagefault.\n", requestedAddress);
        fprintf(outputFile, "Master: Address %d is not in a frame, pagefault.\n", requestedAddress);
        int j;
        int Frame_Open = -1;
        for(j = 0; j < Frames_total; j++){
            if (Frames_Open[j] == 0){
                Frame_Open = j;
                printf("Master: Frame %d is open, page %d will be loaded here.\n", Frame_Open, requestedPage);
                fprintf(outputFile, "Master: Frame %d is open, page %d will be loaded here.\n", Frame_Open, requestedPage);
                break;
            }
        }
        if (Frame_Open < 0){
            Frame_Open = FrameSpace_Free();
            printf("Master: Clearing frame %d and swapping in P%d page %d \n", Frame_Open, PID_Requesting, requestedPage);
            fprintf(outputFile, "Master: Clearing frame %d and swapping in P%d page %d .\n", Frame_Open, PID_Requesting, requestedPage);
			
			     printf("**************Frame %d is NOT OCCUPIED ************\n", Frame_Open);
            fprintf(outputFile, "**********Frame %d is NOT OCCUPIED *************\n", Frame_Open);
        }

        if (ReadWrite == 'w'){
            PAGE[Process_Location][requestedPage] = Frame_Open;
            struct BlockMemory Block_new;
            Block_new.Bit_Dirt = 1;
            Block_new.pid = PID_Requesting;
            Block_new.Bit_Read = 0;
            Block_new.Bit_Refer = 1;
            Block_new.Bit_Write = 1;
            frames[Frame_Open] = Block_new;
            Frames_Open[Frame_Open] = 1;
            enqueue(Queue_Page, Frame_Open);
			
			printf("************Dirty Bit is set to 1 for frame %d at time %d:%d ************\n", Frame_Open, ClkSharedPointer[0], ClkSharedPointer[1]);
            fprintf(outputFile, "**************Dirty Bit is set to 1 for frame %d at time %d:%d ***********\n",Frame_Open, ClkSharedPointer[0], ClkSharedPointer[1]);
			
			
			
			// ddress 12345 in frame 203, writing data to frame at time xxx:xxx
			
			printf("Master: Address %d in frame %d, writing data to frame at time %d:%d\n", requestedAddress, Frame_Open, ClkSharedPointer[0], ClkSharedPointer[1]);
            fprintf(outputFile, "Master: Address %d in frame %d, writing data to frame at time %d:%d\n", requestedAddress, Frame_Open, ClkSharedPointer[0], ClkSharedPointer[1]);
			
			
        } else {
            PAGE[Process_Location][requestedPage] = Frame_Open;
            struct BlockMemory Block_new;
            Block_new.Bit_Dirt = 1;
            Block_new.pid = PID_Requesting;
            Block_new.Bit_Read = 1;
            Block_new.Bit_Refer = 1;
            Block_new.Bit_Write = 0;
            frames[Frame_Open] = Block_new;
            Frames_Open[Frame_Open] = 1;
            enqueue(Queue_Page, Frame_Open);
			
			//Master: Address 25237 in frame 13, giving data to P2 at time xxx:xxx
			printf("Master: Address %d in frame %d, giving data to P%d at time %d:%d\n", requestedAddress, Frame_Open, PID_Requesting, ClkSharedPointer[0], ClkSharedPointer[1]);
			fprintf(outputFile, "Master: Address %d in frame %d, giving data to P%d at time %d:%d\n", requestedAddress, Frame_Open, PID_Requesting, ClkSharedPointer[0], ClkSharedPointer[1]);
          
	;
        }
    } else {
        if (ReadWrite == 'w'){
            int blockInFrame = PAGE[Process_Location][requestedPage];
            struct BlockMemory block = frames[blockInFrame];
            block.Bit_Dirt = 1;
            block.Bit_Write = 1;
            block.Bit_Refer = 1;
			// Master: Dirty bit of frame 107 set, adding additional time to the clock
			
            printf("Master: Dirty bit of frame %d set, adding additional time to the clock\n", blockInFrame);
            fprintf(outputFile, "Master: Dirty bit of frame %d set, adding additional time to the clock\n", blockInFrame);
        } else {
            int blockInFrame = PAGE[Process_Location][requestedPage];
            struct BlockMemory block = frames[blockInFrame];
            block.Bit_Dirt = 1;
            block.Bit_Read = 1;
            block.Bit_Refer = 1;
            printf("Master: Process %d read from page %d: already in frame %d.\n", PID_Requesting, requestedPage, blockInFrame);
            fprintf(outputFile, "Master: Process %d read from page %d: already in frame %d.\n", PID_Requesting, requestedPage, blockInFrame);
        }
    }

    Msg.mtype = PID_Requesting;

    int msgSent = msgsnd(MsgQueue_ID, &Msg, sizeof(Msg), 0);
    if (msgSent < 0){
        printf("Parrent: failed to send Msg.\n");
    }
	// Indicating to P2 that write has happened to address 03456
	
    printf("Master: Indicating to P%d that write has happened to address %d.\n", PID_Requesting, requestedAddress);
    fprintf(outputFile, "Master: Indicating to P%d that write has happened to address %d.\n", PID_Requesting, requestedAddress);
}

int FrameSpace_Free(){
    for (;;){
        int Deletion_Candidate = First(Queue_Page);

        if (frames[Deletion_Candidate].Bit_Refer == 0 && frames[Deletion_Candidate].Bit_Dirt == 0){
            Frames_Open[Deletion_Candidate] = 0;
            dequeue(Queue_Page);
           // printf("Parrent: Dealocated frame %d.\n", Deletion_Candidate);
            //fprintf(outputFile, "Parrent: Dealocated frame %d.\n", Deletion_Candidate);
			
			printf("************Dirty Bit is set to 0 for frame %d at time %d:%d ******************\n", Deletion_Candidate, ClkSharedPointer[0], ClkSharedPointer[1]);
            fprintf(outputFile, "***************Dirty Bit is set to 0 for frame %d at time %d:%d*******************\n",Deletion_Candidate, ClkSharedPointer[0], ClkSharedPointer[1]); 
			
            return Deletion_Candidate;
        } else {
            frames[Deletion_Candidate].Bit_Refer = 0;
            frames[Deletion_Candidate].Bit_Dirt = 0;
            dequeue(Queue_Page);
            enqueue(Queue_Page, Deletion_Candidate);
            printf("**********Frame %d is OCCUPIED at time %d:%d*************\n", Deletion_Candidate, ClkSharedPointer[0], ClkSharedPointer[1]);
            fprintf(outputFile, "**********Frame %d is OCCUPIED at time %d:%d************** \n", Deletion_Candidate, ClkSharedPointer[0], ClkSharedPointer[1]);
        }
    }
}