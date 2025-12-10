#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/msg.h>
#include <getopt.h>
#include <string.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define SHMCLOCKKEY	86868            /* Sem clock.*/
#define MSGQUEUEKEY	68686            /*  Msgqueue Key */
#define MAXRESOURCEKEY	71657            /* Key to check on resources.*/

#define PERMS (mode_t)(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define FLAGS (O_CREAT | O_EXCL)

#define TERMCONSTANT 2 				// Percent chance that a child process will terminate instead of requesting/releasing a resource
#define REQUESTCONSTANT 50			// Percent chance that a child process will request a new resource

typedef struct node {
    struct node *next;
    struct pcb *H1; 
} Queue;

typedef struct {
	int seconds;
	int nanosecs;
} clockStruct;

typedef struct {
	int resourcesUtil[20];
} resourceStruct;

typedef struct pcb {
	int pid;
	int requestedResource;
	int totalBlockedTime;
	int blockedBurstSecond;
	int blockedBurstNano;
	resourceStruct *resUsed;
} PCB;

static int ID_QUEUE;
static clockStruct *sharedClock;
static clockStruct *forkTime;
static resourceStruct *ResourceMax;
static resourceStruct *ResourceAlloc;
static resourceStruct *ResourceAvailable;

int randForkTime;
int ResourceSeg_Max;
int Cnt_Child;
int shmclock;
int MsgLen;

int SignalHandling();
static void endAllProcesses(int signo);
static void childFinished(int signo);

int ProcessCntlBlck();
void Sch_Detach();

Queue *NewProcess(int pid);
Queue *NewBlockedMem(PCB *pcb);
void ProcessListDeletion(int pidToDelete, Queue *ptr);
void printQueue(Queue * ptr);
PCB *PCB_New(int pid);
PCB *SearchPCB(int pid, Queue * ptrHead);

int ForkTime_Chk();
void ForkTime();
int deadlockAvoidance(int Resource);
int bankersAlgorithm(int Resource, PCB * Process);
void ResourceRelease(resourceStruct * Resource);