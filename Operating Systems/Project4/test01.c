#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/sem.h>

int main()
{
   int x;

   /* Generates a random value from 0 to 1000000 */
   srand(getpid());
   x = rand() % 2;

   printf("x is: %d", x);
   exit(0);
}
