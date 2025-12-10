#include "oss.h"

typedef struct {
	long mtype;
	pid_t pid;
	int Resource;
} UserMsg;

Queue *BlockedQueue1;
Queue *BlockedQueue2; 
Queue *ProcessList1;
Queue *ProcessList2;


static volatile sig_atomic_t CmpltFlag = 0;
static UserMsg *SentMsg;
int GrantedRequest;
int DeadLockCnt;

int main (int argc, char *argv[]){

	FILE *logfile;
	logfile = fopen("logfile", "w");

	srand(time(NULL) + getpid());

	Queue * TempPointer;
	Queue * prevTempPointer;

	TempPointer = NULL;
	prevTempPointer = NULL;

	BlockedQueue1 = NULL;
	BlockedQueue2 = NULL; 

	ProcessList1 = NULL;
	ProcessList2 = NULL;

	int childPid;
	int ElaspsedTime = 2;
	int ResoucesShared;
	int verbose = 1; //1: verbose on
	int ProcessLimit = 18;
    int TotalProcess = 0;
    int LogLength = 0;

	Cnt_Child = 0;
    GrantedRequest = 0;
	DeadLockCnt = 0;


	SignalHandling();
	ProcessCntlBlck();

	alarm(ElaspsedTime);

	// init message struct 
	SentMsg = malloc(sizeof(UserMsg));
	MsgLen = sizeof(UserMsg) - sizeof(long);

	//init allocated resources
	ResourceAlloc = malloc(sizeof(resourceStruct));
	ResourceAvailable = malloc(sizeof(resourceStruct));

	// init both resource structs' resource elements
	int i;
	for (i = 0; i < 20; i++){
		ResourceMax->resourcesUtil[i] = (rand() % 9) + 1;
		ResourceAvailable->resourcesUtil[i] = ResourceMax->resourcesUtil[i];
		ResourceAlloc->resourcesUtil[i] = 0;
		
	}	
	
	ForkTime();
	
	// Creation of Process
	
	while(!CmpltFlag){
		if(Cnt_Child < ProcessLimit && ForkTime_Chk() == 1){

			if ((childPid = fork()) == 0){
				execlp("./user", "./user", (char*)NULL);

				fprintf(stderr, "%sFailed to fork child process!\n", argv[0]);
				_exit(1);
			}	

			if (ProcessList1 == NULL){
				ProcessList1 = NewProcess(childPid);
				ProcessList2 = ProcessList1;
			} else {
				ProcessList2 = ProcessList2->next = NewProcess(childPid);
			}

			TotalProcess  += 1;
			Cnt_Child += 1;
			printf("OSS: Process with PID %d is created at time %d:%d\n", childPid, sharedClock->seconds, sharedClock->nanosecs);
			ForkTime();
		}	

// When process terminates release the resourcse

		if(msgrcv(ID_QUEUE, SentMsg, MsgLen, 1, IPC_NOWAIT) != -1){
			ProcessListDeletion(SentMsg->pid, ProcessList1);

			SentMsg->mtype = SentMsg->pid;
			msgsnd(ID_QUEUE, SentMsg, MsgLen, 0);
		}

//Process is requesting resource

		if(msgrcv(ID_QUEUE, SentMsg, MsgLen, 3, IPC_NOWAIT) != -1){
			if(verbose == 1 && LogLength <= 100000){
				fprintf(logfile, "Master has detected Process P%d requesting R%d at time %d:%d\n", SentMsg->pid, SentMsg->Resource, sharedClock->seconds, sharedClock->nanosecs);
				LogLength += 1;
			}

// Banker's Algorithm to check the possible  deadlock condition.

			DeadLockCnt += 1;
			if (bankersAlgorithm(SentMsg->Resource, SearchPCB(SentMsg->pid, ProcessList1)) == 1){
				if(verbose == 1 && LogLength <= 100000){
					fprintf(logfile, "Master granting P%d request R%d at time %d:%d\n", SentMsg->pid, SentMsg->Resource, sharedClock->seconds, sharedClock->nanosecs);
					if(DeadLockCnt % 20 == 0){
						int o; 
						int n = 0;
						TempPointer = ProcessList1;
						fprintf(logfile, "     ");
						for(o = 0; o < 20; o++){
							
							fprintf(logfile, "Master running deadlock detection at time %d:%d\n",  sharedClock->seconds, sharedClock->nanosecs);
						}
						//printf("Number of times deadlocked : P%d", o);
						LogLength += 1;
						fprintf(logfile, "\n");
						LogLength += 1;
						fprintf(logfile, "--------------Released Resources:------------------------\n");
                        fprintf(logfile, "\n");
						while(TempPointer != NULL){
							fprintf(logfile, "P%d", n);
							for(o = 0; o < 20; o++){

								fprintf(logfile, " %d", TempPointer->H1->resUsed->resourcesUtil[o]);
								
							}
							fprintf(logfile, "\n\n\n\n");
							TempPointer = TempPointer->next;
							n += 1;
							LogLength += 1;
						}
					}
					LogLength += 1;
				}
// Whether the resource is granted to the process or it is deadlocked 

				GrantedRequest += 1;
				SearchPCB(SentMsg->pid, ProcessList1)->resUsed->resourcesUtil[SentMsg->Resource] += 1;
				SentMsg->mtype = SentMsg->pid;
				msgsnd(ID_QUEUE, SentMsg, MsgLen, 0);
			} else {
				
				LogLength += 1;
				
				
// If the resource is not available the process is put to sleep in queue

				if (BlockedQueue1 == NULL){
					BlockedQueue1 = NewBlockedMem(SearchPCB(SentMsg->pid, ProcessList1));
					BlockedQueue2 = BlockedQueue1;
				} else {
					BlockedQueue2 = BlockedQueue2->next = NewBlockedMem(SearchPCB(SentMsg->pid, ProcessList1));
				}
				BlockedQueue2->H1->blockedBurstSecond = sharedClock->seconds;
				BlockedQueue2->H1->blockedBurstNano = sharedClock->nanosecs;
				BlockedQueue2->H1->requestedResource = SentMsg->Resource;
			}

		}
		
// Process is releasing resource

		if(msgrcv(ID_QUEUE, SentMsg, MsgLen, 2, IPC_NOWAIT) != -1){
				if(verbose == 1 && LogLength <= 100000){
					fprintf(logfile, "Master has acknowledged Process P%d releasing R%d at time %d:%d", SentMsg->pid, SentMsg->Resource, sharedClock->seconds, sharedClock->nanosecs);
					LogLength += 1;
				}			
			ResourceAlloc->resourcesUtil[SentMsg->Resource] -= 1;
			ResourceAvailable->resourcesUtil[SentMsg->Resource] += 1;
			SearchPCB(SentMsg->pid, ProcessList1)->resUsed->resourcesUtil[SentMsg->Resource] -= 1;

			SentMsg->mtype = SentMsg->pid;
			msgsnd(ID_QUEUE, SentMsg, MsgLen, 0);

		}

// If the resources get available, wake up the process (remove from blocked queue)

		TempPointer = BlockedQueue1;
		prevTempPointer = BlockedQueue1;

		while(TempPointer != NULL){
			ResoucesShared = TempPointer->H1->requestedResource;
			
			if ((ResourceAlloc->resourcesUtil[ResoucesShared]) < (ResourceMax->resourcesUtil[ResoucesShared])){
				
				if(TempPointer == prevTempPointer){
					BlockedQueue1 = TempPointer->next;
				} else {
					if (TempPointer->next == NULL){
						BlockedQueue2 = prevTempPointer;
					}

						prevTempPointer->next = TempPointer->next;	
				}
				
				LogLength += 1;
				ResourceAlloc->resourcesUtil[ResoucesShared] += 1;
				ResourceAvailable->resourcesUtil[ResoucesShared] -= 1;
				SearchPCB(TempPointer->H1->pid, ProcessList1)->resUsed->resourcesUtil[ResoucesShared] += 1;
				SentMsg->mtype = TempPointer->H1->pid;
				msgsnd(ID_QUEUE, SentMsg, MsgLen, 0);
				TempPointer = NULL;
				prevTempPointer = NULL;
			} else {
				if (prevTempPointer != TempPointer){
					prevTempPointer = TempPointer;
				}

				TempPointer = TempPointer->next;
				if (TempPointer != NULL){
					
				}
			}
		}


     
         sharedClock->nanosecs += 1000000;
            if (sharedClock->nanosecs >= 1000000000){
                sharedClock->seconds += 1;
                sharedClock->nanosecs = sharedClock->nanosecs % 1000000000;
            }
            if (sharedClock->seconds >= 1000){
                sharedClock->nanosecs = 0;
                CmpltFlag = 1;
            }

            if(TotalProcess >= 99){
            	CmpltFlag = 1;
            }

    }

   



   printf("\n");

	printf("********************Total number of process in the system: %d****************\n", TotalProcess);

	 printf("\nAvailable Resources to grant to the processes: \n[");
	 int n;
	 for(n = 0; n < 20; n++){
	 	printf("%d,", ResourceAvailable->resourcesUtil[n]);
	 }
	 printf("]\nNumber of Allocated Resources to the Processes: \n[");
	 for(n = 0; n < 20; n++){
	 	printf("%d,", ResourceAlloc->resourcesUtil[n]);
	 }
	printf("]\n Total number of Resources in the system: \n[");
	 for(n = 0; n < 20; n++){
	 	printf("%d,", ResourceMax->resourcesUtil[n]);
	 }
	 printf("]\n\n\n\n");
printf("*********Requests that have been granted: %d*************\n", GrantedRequest);
	printf("*********Number of times deadlocked detection is run : %d ***************\n", DeadLockCnt);
	fprintf(logfile, "\n\n");
	fprintf(logfile, "*********Number of times deadlocked detection is run : %d ***************\n", DeadLockCnt);
	
printf("\n");


	printf("***************Percentage of Deadlock detection and the processes that are terminated sucessfully: %f*******************\n", (float)GrantedRequest/(float)DeadLockCnt*100);
printf("\n");

fprintf(logfile, "***************Percentage of Deadlock detection and the processes that are terminated sucessfully: %f*******************\n", (float)GrantedRequest/(float)DeadLockCnt*100);
fprintf(logfile, "*********Requests that have been granted: %d*************\n", GrantedRequest);

	Sch_Detach();


	return 0;

	fclose(logfile);


}



// Signal Handling 

int SignalHandling(){

//Setting up Signal Handler to set up the alarm after some time limit

	struct sigaction timerAlarm;

	timerAlarm.sa_handler = endAllProcesses;
	timerAlarm.sa_flags = 0;

	if ((sigemptyset(&timerAlarm.sa_mask) == -1) || (sigaction(SIGALRM, &timerAlarm, NULL) == -1)) {
		perror("Failed to set SIGALRM to handle timer alarm");
		return -1;
	}

//Setting up a handler for SIGINT

	struct sigaction controlc;

	controlc.sa_handler = endAllProcesses;
	controlc.sa_flags = 0;

	if ((sigemptyset(&controlc.sa_mask) == -1) || (sigaction(SIGINT, &controlc, NULL) == -1)) {
		perror("Failed to set SIGINT to handle control-c");
		return -1;
	}

// Signal Handler when the Process terminates 

	struct sigaction workerFinished;

	workerFinished.sa_handler = childFinished;
	workerFinished.sa_flags = 0;

	if ((sigemptyset(&workerFinished.sa_mask) == -1) || (sigaction(SIGCHLD, &workerFinished, NULL) == -1)) {
		perror("Failed to set SIGCHLD to handle signal from child process");
		return -1;
	}


	return 1;
}

// Killing All the Processes if the system is in DEADLOCK

static void endAllProcesses(int signo){
	CmpltFlag = 1;
	if(signo == SIGALRM){
				 printf("]\n\n\nKILLING ALL PROCESSES!!!!!\n\n\n");
		killpg(getpgid(getpid()), SIGINT);
	}
}

// Functionnn to check when the child is finished

static void childFinished(int signo){
	pid_t finishedpid;
	while((finishedpid = waitpid(-1, NULL, WNOHANG))){
		if((finishedpid == -1) && (errno != EINTR)){
			break;
		} else {
			printf("Process with PID %d finished!\n", finishedpid);
			Cnt_Child -= 1;
		}
	}
}

// Shared Memory Clock

int ProcessCntlBlck(){
	
	shmclock = shmget(SHMCLOCKKEY, sizeof(clockStruct), 0666 | IPC_CREAT);
	sharedClock = (clockStruct *)shmat(shmclock, NULL, 0);
	if (shmclock == -1){
		return -1;
	}

	sharedClock -> seconds = 0;
	sharedClock -> nanosecs = 0;

// Initiating the time when to fork  the processes
	
	forkTime = malloc(sizeof(clockStruct));

//Resource Initiation 

	ResourceSeg_Max = shmget(MAXRESOURCEKEY, (sizeof(resourceStruct) + 1), 0666 | IPC_CREAT);
	ResourceMax = (resourceStruct *)shmat(ResourceSeg_Max, NULL, 0);
	if (ResourceSeg_Max == -1){
		return -1;
	}

//Message queues id

	ID_QUEUE = msgget(MSGQUEUEKEY, PERMS | IPC_CREAT);
	if (ID_QUEUE == -1){
		return -1;
	} 

	return 0;
}

// Deallocating Shared Mem
void Sch_Detach(){
	shmdt(sharedClock);
	shmctl(shmclock, IPC_RMID, NULL);
	shmdt(ResourceMax);
	shmctl(ResourceSeg_Max, IPC_RMID, NULL);
	msgctl(ID_QUEUE, IPC_RMID, NULL);
}

// Queues for the Processes

Queue *NewProcess(int pid)
{
    Queue *Qnew;
    Qnew = malloc(sizeof(Queue));
    Qnew->next = NULL;
    Qnew->H1 = malloc(sizeof(PCB));
    Qnew->H1 = PCB_New(pid);
    
    return Qnew;
}
Queue *NewBlockedMem(PCB *pcb)
{
    Queue *Qnew;
    Qnew = malloc(sizeof(Queue));
    Qnew->next = NULL;
    Qnew->H1 = malloc(sizeof(PCB));
    Qnew->H1 = pcb;
    
    return Qnew;
}

// When the proceses are terminated release them from the queues or PRocess list

void ProcessListDeletion(int pidToDelete, Queue *ptr){
	//case of first element in queue
	if (ptr->H1->pid == pidToDelete){
		// printf("RESOURCES RELEASED! from %d\n", ptr->H1->pid);
		ResourceRelease(ptr->H1->resUsed);
		ProcessList1 = ptr->next;
		return;
	} else {
		while(ptr != NULL){
			if (ptr->next->H1->pid == pidToDelete){
				// printf("RESOURCES RELEASED! from %d\n", ptr->next->H1->pid);
				ResourceRelease(ptr->next->H1->resUsed);
				ptr->next = ptr->next->next;
				if(ptr->next == NULL){
					ProcessList2 = ptr;
				}
				return;
			} else {
				ptr = ptr->next;
			}
		}
	}
}

// Process Control block to keep a track of the information for all the blocked and newly created process

PCB *PCB_New(int pid){
	PCB *newP;
	newP = malloc(sizeof(PCB));
	newP->pid = pid;
	newP->requestedResource = 0;
	newP->resUsed = malloc(sizeof(resourceStruct));
	int n;
	for(n = 0; n < 20; n++){
		newP->resUsed->resourcesUtil[n] = 0;
	}

	return newP;
}

PCB *SearchPCB(int pid, Queue * ptrHead){
	while(ptrHead != NULL){
		if(ptrHead->H1->pid == pid){
			return ptrHead->H1;
		} else {
			ptrHead = ptrHead->next;
		}
	}
	return NULL;
}

// Check to see if it is a time to fork off new process in the system

int ForkTime_Chk(){
	
	if ((sharedClock->nanosecs >= forkTime->nanosecs) && (sharedClock->seconds >= forkTime->seconds)){
		return 1;
	} else {
		if(sharedClock->seconds < 2 && sharedClock->nanosecs%100000000 == 0){

	}
		return 0;
	}
}

// Clock timing to fork new process in system

void ForkTime(){
	randForkTime = (rand() % 500) * 1000000;

	forkTime->nanosecs = sharedClock->nanosecs + randForkTime;
	forkTime->seconds = sharedClock->seconds;
	if(forkTime->nanosecs >= 1000000000){
		forkTime->seconds += 1;
		forkTime->nanosecs = forkTime->nanosecs%1000000000;
	}
}

// Deadlock Avaoidance 

int deadlockAvoidance(int Resource){
	if((ResourceAlloc->resourcesUtil[Resource]) < (ResourceMax->resourcesUtil[Resource])){
		ResourceAlloc->resourcesUtil[Resource] += 1;
		ResourceAvailable->resourcesUtil[Resource] -= 1;
		return 1;
	} else {
		return 0;
	}
}

// To check if there are sufficifient available resources for the process and they are not getting DEADLOCKED.

int bankersAlgorithm(int Resource, PCB * Process){
	int r;
	int s;

	if(ResourceAvailable->resourcesUtil[Resource] > 1){
		ResourceAlloc->resourcesUtil[Resource] += 1;
		ResourceAvailable->resourcesUtil[Resource] -= 1;
		return 1;
	} else if (ResourceAvailable->resourcesUtil[Resource] == 0){
		return 0;
	} else {
		for(r = 0; r < 20; r++){
			s = r;
			if(r == Resource){
				s = Resource + 1;
			}
			if(ResourceAvailable->resourcesUtil[s] + Process->resUsed->resourcesUtil[s] < 1){
				return 0;
			}
		}
		ResourceAlloc->resourcesUtil[Resource] += 1;
		ResourceAvailable->resourcesUtil[Resource] -= 1;
		
		return 1;
	}

}

// Resources Released 

void ResourceRelease(resourceStruct * Resource)
{
	int r;
	for (r = 0; r < 20; r++)
	{
		if(Resource->resourcesUtil[r] > 0)
		{
			ResourceAvailable->resourcesUtil[r] += Resource->resourcesUtil[r];
			ResourceAlloc->resourcesUtil[r] -= Resource->resourcesUtil[r];
		}
	}
}