/*
 User Process (user.c)

 Description:
 This user process is forked off from the simulator. It perform the following functions.
 1. The shared memory address ids of the clock and the communication area are got from the oss as command line arguments.
 2. Attaches necessary ids to those addressed and opens the semaphore set based on the key set on the oss.
 3. Generates a random value from 0 to 1000000 and uses it to calculate the end time of this user from the oss clock time by adding this random value to it.
 4. This goes into a loop to wait until the critical section can be reached. The criteria to reach the critical section is as below.
    a) semahore value should be 0.
    b) There should not be any messages in the shmMsg area.
    c) oss clock should have gone past the calculated termination time.
 5. Once all the above criteria is met, critical section is reached, writes the termination time in the shmMsg area, releases the semaphore lock and releases attached shared memory and terminates.

*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/sem.h>

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

int main(int argc, char **argv)
{
   long int shmid;
   long int shmid2;
   long int shmid3;
   long int *sharedmemclock;
   long int *shmMsg;
   int x, y, z;
   struct timespec tim, tim2;
   key_t key;
   int semid;
   int msgid;
   short zero = 0;
   struct sembuf sops[2];
   int gocritical = 0;
   long int terms = 0;
   long int termns = 0;
   struct ProcessesPCB *prcs;
   struct mesg_buffer message;


   key = ftok("./Makefile", 'E');
   semid = semget(key, 1, 0);

   /* The shared memory address ids of the clock and the communication area are got from the oss as command line arguments */
   shmid = atoi(argv[1]);
   shmid2 = atoi(argv[2]);
   shmid3 = atoi(argv[3]);

   sharedmemclock = (long int *)shmat(shmid,NULL,0);
   shmMsg = (long int *)shmat(shmid2,NULL,0);
   prcs = (long int *)shmat(shmid3,NULL,0);

   /* Generates a random value from 0 to 1, to decide on whether to consume the full quantum */
   srand(getpid());
   x = rand() % 2;

   printf("x is: %d", x);

   /* Release the attachments to the shared memory and terminate */
   if (shmdt(sharedmemclock) == -1)
   {
      perror("user: Error: Failed to detach the sharedmemclock shmdt()");
      exit(EXIT_FAILURE);
   }

   if (shmdt(shmMsg) == -1)
   {
      perror("user: Error: Failed to detach the shmMsg shmdt()");
      exit(EXIT_FAILURE);
   }

   if (shmdt(prcs) == -1)
   {
      perror("user: Error: Failed to detach the prcs shmdt()");
      exit(EXIT_FAILURE);
   }
   return 0;
}

