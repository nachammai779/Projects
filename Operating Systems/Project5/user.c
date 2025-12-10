#include "oss.h"

static volatile sig_atomic_t ProcessCmpltFlg = 0;


typedef struct {
	long mtype;
	pid_t pid;
	int msg;
} UserMsg;

static UserMsg *SentMsg;



int SignalHandling();
int ProcessCntlBlck();
static void ProcessEnd(int signo);
void Sch_Detach();

int main (int argc, char *argv[]){

	srand(time(NULL) + getpid());

	pid_t pid = getpid();
	int i;
	int ResourceIndex;
	int ResourceSelection;  // Random Selection of Resources By the Process
	int ResourceActivity;     
	int ResourceCount = 0;


	SignalHandling();
	ProcessCntlBlck();

 

	while(!ProcessCmpltFlg){
			ResourceActivity = rand() % 100;
			

// The Process terminates without requesting resource

			if(ResourceActivity < TERMCONSTANT){
				ProcessCmpltFlg = 1;
				
//Notify the OSS about the process termination 

				SentMsg->mtype = 1;
				SentMsg->pid = pid;
				SentMsg->msg = 0;
				msgsnd(ID_QUEUE, SentMsg, MsgLen, 1);
				if (msgrcv(ID_QUEUE, SentMsg, MsgLen, pid, 0) != -1){
					ResourceAlloc->resourcesUtil[ResourceSelection] += 1;
				}

//The process is requesting resources

			} else if (ResourceActivity >= TERMCONSTANT && ResourceActivity < REQUESTCONSTANT){
				ResourceSelection = rand() % 20;
				if ((ResourceAlloc->resourcesUtil[ResourceSelection]) < (ResourceMax->resourcesUtil[ResourceSelection]) && ResourceAlloc->resourcesUtil[ResourceSelection] < 1){
					SentMsg->mtype = 3;
					SentMsg->pid = pid;
					SentMsg->msg = ResourceSelection;
					
					msgsnd(ID_QUEUE, SentMsg, MsgLen, 3);
					if (msgrcv(ID_QUEUE, SentMsg, MsgLen, pid, 0) != -1){
						ResourceAlloc->resourcesUtil[ResourceSelection] += 1;
						ResourceCount += 1;
					}

				}
// Release the resources

			} else {
				if(ResourceCount > 0){
					ResourceIndex = 0;
					ResourceSelection = rand() % ResourceCount;
					for(i = 0; i < 20; i++){
						if(ResourceAlloc->resourcesUtil[i] > 0){
							if (ResourceIndex == ResourceSelection){
							

								SentMsg->mtype = 2;
								SentMsg->pid = pid;
								SentMsg->msg = i;
								
								msgsnd(ID_QUEUE, SentMsg, MsgLen, 2);
								if (msgrcv(ID_QUEUE, SentMsg, MsgLen, pid, 0) != -1){
									ResourceAlloc->resourcesUtil[i] -= 1;
									ResourceCount -= 1;
									i = 20;

								}
								
							} else {
								ResourceIndex += 1;
							}
						}
					}
				}	
			}



		
			
			if(sharedClock->seconds >= 1000){
				ProcessCmpltFlg = 1;
			}
		
	}

	
	

	printf("Process with PID %d is going to exit, the resources which were available to it were: [", pid);
	for (i = 0; i < 20; i++){
		printf("%d,", ResourceAlloc->resourcesUtil[i]);
	}
	printf("]\n");
	exit(1);
	return 1;


}
int ProcessCntlBlck(){

// initiating shared mem clock
	shmclock = shmget(SHMCLOCKKEY, sizeof(clockStruct), 0666 | IPC_CREAT);
	sharedClock = (clockStruct *)shmat(shmclock, NULL, 0);

	//initiating resources
	
	ResourceSeg_Max = shmget(MAXRESOURCEKEY, (sizeof(resourceStruct) + 1), 0666 | IPC_CREAT);
	ResourceMax = (resourceStruct *)shmat(ResourceSeg_Max, NULL, 0);
	if (ResourceSeg_Max == -1){
		return -1;
	}

// init allocated resources

	ResourceAlloc = malloc(sizeof(resourceStruct));
	int r;
	for (r = 0; r < 20; r++){
		ResourceAlloc->resourcesUtil[r] = 0;
	}

//queues

	ID_QUEUE = msgget(MSGQUEUEKEY, PERMS | IPC_CREAT);
	if (ID_QUEUE == -1){
		return -1;
	} 

	SentMsg = malloc(sizeof(UserMsg));
	MsgLen = sizeof(UserMsg) - sizeof(long);

	


	return 0;
}

//Shared Memory Deletion

void Sch_Detach(){
	shmdt(clock);
	shmdt(ResourceMax);

 	msgctl(ID_QUEUE, IPC_RMID, NULL);
}

// Interrupt Handling

int SignalHandling(){

//Interrupt handling for ctrl-C

	struct sigaction controlc;

	controlc.sa_handler = ProcessEnd;
	controlc.sa_flags = 0;

	if ((sigemptyset(&controlc.sa_mask) == -1) || (sigaction(SIGINT, &controlc, NULL) == -1)) {
		perror("Failed to set SIGINT to handle control-c");
		return -1;
	}

//Interrupt when the process terminates

	struct sigaction sigParent;

	sigParent.sa_handler = ProcessEnd;
	sigParent.sa_flags = 0;

	if ((sigemptyset(&sigParent.sa_mask) == -1) || (sigaction(SIGCHLD, &sigParent, NULL) == -1)) {
		perror("Failed to set SIGCHLD to handle signal from child process");
		return -1;
	}


	return 1;
}
static void ProcessEnd(int signo){
		ProcessCmpltFlg = 1;
}
