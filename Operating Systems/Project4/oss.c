/*
 Operting System Simulator (oss.c)

 Supported Arguments
 -s <max no. of childs>  : If provided, these many childs will be spawned at the maximum. Default is max of 5 childs to be spawned. 
                           'user' is the child process and that executable is expected to be present in the same directory where 'oss' is present. 
 -l <log file name>      : If provided, logs will be written to this file. Default is ./log.dat 
 -t <overall time limit> : If provided, the process will terminate after this much elapsed time. Default is 5 seconds. 

 Description:
 This simulator when started, parses the command line arguments, displays the help menu and skips processing if -h is passed in the arguments.
 If help flag is not set, it perform the following functions.
 1. Handler function to kill the parent and the ongoing child processes when the overall timelimit is reached. 
 2. Log file is opened in write mode.
 3. Assigns two shared memory one to maintain a clock and another to communicate with the user (forked processes) within this simulator.
 4. Sets up the semaphore for handling the critical section code withing the users.
 5. Necessary number of concurrrent user processes are spawned. (5 by default).
 6. Goes into a loop incrementing the clock, and if there are any messages received from a user saying that it is going to get terminated, this will wait until that user is getting terminated and start a new one.
 7. The loop will be broken if the clock reaches 2 seconds, or the total number of users spawned has reached 100. And then acquired shared memory and semaphore is released and oss terminates.
 8. If the overall time limit is reached (Default is 5 seconds) before the above loop is broken, 
    then alarm is triggered and the handler function releases the acquired shared memory and semaphore and kills oss and its child processes.
 
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/msg.h>

static const int  MAX_PRCS=5, TIME_QUANTUM=30, FILE_OFFSET=38;
static const unsigned long maxTimeBetweenNewProcsSecs=0, maxTimeBetweenNewProcsNS=1000000;

/* Structure for Process Control Block */
struct ProcessesPCB
{
   int pid, pidpriority, bitvecloc;
   long int totCPU[2], tottime[2], lastbursttime[2];
};

/* Structure for Message Queue */
struct mesg_buffer { 
	long mesg_type; 
	char mesg_text[100]; 
}; 

FILE *lptr;
long int *sharedmemclock;
long int *shmMsg;
struct ProcessesPCB *prcs;
struct mesg_buffer message;

int semid;
int msgid;
long int shmid;
long int shmid2;
long int shmid3;
short zero = 0;
char ch[200];

/* Handler function to kill the parent and the ongoing child processes when the overall timelimit is reached. 
   It also releases the acquired shared memory and semaphore */
static void myhandler(int s)
{
   printf("Overall timelimit reached... Now terminating...\n");

   strcpy(ch,"");
   snprintf(ch, sizeof(ch), "Master pid: %i exiting because overall timelimit reached.\n", getpid());
   fputs(ch,lptr);

   fclose(lptr);

   if (shmdt(sharedmemclock) == -1)
      perror("oss: Error: Failed to detach the sharedmemclock shmdt()");
   else if (shmctl(shmid, IPC_RMID, NULL) == -1)
      perror("oss: Error: Failed to remove the shmid shmctl()");

   if (shmdt(shmMsg) == -1)
      perror("oss: Error: Failed to detach the shmMsg shmdt()");
   else if (shmctl(shmid2, IPC_RMID, NULL) == -1)
      perror("oss: Error: Failed to remove the shmid2 shmctl()");

   if (semctl(semid, 0, IPC_RMID, &zero) == -1)
      perror("oss: Error: Failed to remove the semid semctl()");

   kill(-1*getpid(), SIGKILL);
}

int main(int argc, char **argv)
{
   int hflag = 0;
   int otimelimit = 5;
   int nchild = MAX_PRCS;
   char *lpath = "./log.dat";
   int index;
   int c;
   int i;
   int j;
   int n;
   int status;
   int breakflag = 0;
   int totalchild = 0;
   char cshmid[12];
   char cshmid2[12];
   char cshmid3[12];
   int pcbbitvec[20]; /* Change the number here to match MAX_PRCS */
   unsigned long x = 0;
   unsigned long y = 0;
   unsigned pprcsstartsec = 0;
   unsigned pprcsstartnsec = 0;
   unsigned nprcsstartsec = 0;
   unsigned nprcsstartnsec = 0;

   pid_t forkpid;
   pid_t w;
   opterr = 0;
   key_t key;

   /* Initializing the sigaction structure to set the timer */
   struct sigaction act;
   act.sa_handler = myhandler;
   act.sa_flags = 0;

   /* Using sigaction's SIGALRM argument to trigger the alarm based on the elapsed time */
   if ((sigemptyset(&act.sa_mask) || sigaction(SIGALRM, &act, NULL)) == -1)
   {
      perror("oss: Error: Failed to set up handler for SIGALRM");
      exit(EXIT_FAILURE);
   }
   /* Setting the alarm for handler to be triggered after otimelimit set by user or 5 seconds by default */
   alarm(otimelimit);

   printf("Log File: %s \n",lpath);
   printf("Max Children that will be spawned: %d \n",nchild);
   printf("Overall Time for oss to execute: %d \n",otimelimit);

   /* Open the log file in write mode */
   lptr = fopen(lpath, "w");
   if (lptr == NULL)
   {
      perror("oss: Error: Output file: fopen()");
      exit(EXIT_FAILURE);
   }

   /* Acquiring the shared memory for the clock and attaching it to shmid */
   shmid = shmget(IPC_PRIVATE, sizeof(long int),0666);
   sharedmemclock = (long int *)shmat(shmid,NULL,0);
   snprintf(cshmid, 12, "%ld", shmid);

   /* Acquiring the shared memory for the shmMsg communication area with users and attaching it to shmid2 */
   shmid2 = shmget(IPC_PRIVATE, sizeof(long int),0666);
   shmMsg = (long int *)shmat(shmid2,NULL,0);
   snprintf(cshmid2, 12, "%ld", shmid2);

   /* Acquiring the shared memory for the Process PCB table and attaching it to shmid3 */
   shmid3 = shmget(IPC_PRIVATE, sizeof(struct ProcessesPCB),0666);
   prcs = (struct ProcessesPCB *)shmat(shmid3,NULL,0);
   snprintf(cshmid3, 12, "%ld", shmid3);

   /* Initializing the bitvector - Setting all the positions to 0 indicating that no processes are running currently */
   for (i=0; i < MAX_PRCS; i++)
      pcbbitvec[i] = 0;

   /* Acquiring the key and the semid for the semaphore */ 
   key = ftok("./Makefile", 'E');
   semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
   semctl(semid,0,SETALL,&zero);

   /* Acquiring the key and the msqid for the message queue */ 
   key = ftok("./oss.c", 'E');
   msgid = msgget(key, 0666 | IPC_CREAT); 
   message.mesg_type = 1; 

   /* Setting the initial time in the shared memory clock */
   sharedmemclock[0] = 0;
   sharedmemclock[1] = 0;

   /* Displaying the shmid and semid so that it can be used to release them manually in case of abnormal termination */
   printf("Current Shared Memory Id (shmid) of Clock        : %ld \n", shmid);
   printf("Current Shared Memory Id (shmid) of shmMsg       : %ld \n", shmid2);
   printf("Current Shared Memory Id (shmid) of ProcessesPCB : %ld \n", shmid3);
   printf("Current Semaphore (semid) used by this process   : %d  \n", semid);
   printf("Current Message Queue Id (msqid) used            : %d  \n", msgid);

   strcpy(ch,"");
   snprintf(ch, sizeof(ch), "Master pid: %i\n", getpid());
   fputs(ch,lptr);

   /* All the initializations are done - Main processing is starting now */

   
   /* Generates a random value from 0 to maxTimeBetweenNewProcsSecs and maxTimeBetweenNewProcsNS */
   srand(getpid());
   x = rand();
   x <<= 15;
   x ^= rand();
   x %= maxTimeBetweenNewProcsSecs;

   y = rand();
   y <<= 15;
   y ^= rand();
   y %= maxTimeBetweenNewProcsNS;

   /* Calculates the new process start time from the prev process start time */
   nprcsstartsec = pprcsstartsec + x;
   nprcsstartnsec = pprcsstartnsec + y;

   if ((nprcsstartnsec) >= 1000000000)
   {
      nprcsstartsec = nprcsstartsec + 1;
      nprcsstartnsec = nprcsstartnsec - 1000000000;
   }

   while (breakflag == 0)
   {
      if ((nprcsstartsec <= sharedmemclock[0]) && (nprcsstartnsec <= sharedmemclock[1]))
      { /* New Process start time has not reached yet */

         
      }
      else /* New Process start time has reached */
      {
         
      }
   }

   /* Close the file and release the acquired shared memory and semaphore and terminate */
   fclose(lptr);

   if (shmdt(sharedmemclock) == -1)
   {
      perror("oss: Error: Failed to detach the sharedmemclock shmdt()");
      exit(EXIT_FAILURE);
   }
   else if (shmctl(shmid, IPC_RMID, NULL) == -1)
   {
      perror("oss: Error: Failed to remove the shmid shmctl()");
      exit(EXIT_FAILURE);
   }

   if (shmdt(shmMsg) == -1)
   {
      perror("oss: Error: Failed to detach the shmMsg shmdt()");
      exit(EXIT_FAILURE);
   }
   else if (shmctl(shmid2, IPC_RMID, NULL) == -1)
   {
      perror("oss: Error: Failed to remove the shmid2 shmctl()");
      exit(EXIT_FAILURE);
   }

   if (semctl(semid, 0, IPC_RMID, &zero) == -1)
   {
      perror("oss: Error: Failed to remove the semid semctl()");
      exit(EXIT_FAILURE);
   }

   return 0;
}

