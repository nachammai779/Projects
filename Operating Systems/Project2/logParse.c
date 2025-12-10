// This utility parses the input file and forks a child for each subset problem that is requested to be solved
// Option Arguments -h for help menu, -i for input file name, -o for output file name, -t for the overall time this utility has to run
// Default input file is ./input.dat
// Default output file is ./output.dat
// Default overall timelimit is 10 seconds
// Each child process if unable to find a subset or unable to decide subset is not possible within a second, child process will terminate with appropriate results
// Parent process will wait until a forked child process is complete, before forking off the next child
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

// Handler function to kill the parent and the ongoing child processes when the overall timelimit is reached.
static void myhandler(int s)
{
   printf("Overall Waitime over... Now terminating...\n");
   kill(-1*getpid(), SIGKILL);
}

// Returns the subset in result[] if there is a subset of set[] with sum equal to given sum.
// If there is no subset found within a second then result[0] is populated with -1.
// If there is no subset possible then result[0] is populated with 0.
bool isSubsetSum(int set[], int n, int sum, int result[])
{
   bool subset[n+1][sum+1];
   int i;
   int j;
   int k;
   time_t cstart, cstop;

   // Noting down the start time of the current child process.
   cstart = time(NULL);

   // The value of subset[i][j] will be true if there is a
   // subset of set[0..j-1] with sum equal to i

   // If sum is 0, then answer is true
   for (i = 0; i <= n; i++)
      subset[i][0] = true;

   // If sum is not 0 and set is empty, then answer is false
   for (i = 1; i <= sum; i++)
      subset[0][i] = false;

   // Fill the subset table in botton up manner
   for (i = 1; i <= n; i++)
   {
      for (j = 1; j <= sum; j++)
      {
         if (j<set[i-1])
            subset[i][j] = subset[i-1][j];
         if (j >= set[i-1])
            subset[i][j] = subset[i-1][j] || subset[i - 1][j-set[i-1]];
      }

      // For testing purpose - Manually sleeping for 1 second and 1 microsecond when 9248 (wait) is in sum
      // Remove the below if condition, if subset of a set needs to be found for a sum of value 9248.
      if (sum == 9248)
         usleep(1000001);

      // Getting the current time of the current child process to calcualte the difference from the start time noted.
      cstop = time(NULL);

      // Return -1 in result[0] if difference between start time and the current time is greater than 1 second.
      if ((cstop - cstart) >= 1)
      {
         result[0] = -1;
         i=n;
      }
   }

   // If current child process is not  timed out.
   if (result[0] != -1)
   {
      // If subset[n][sum] is 0, then it means that there is no subset possible from the given set for the given sum. Return 0 in result[0].
      if (subset[n][sum] == false)
         result[0] = 0;
      // If subset[n][sum] is 1, then there is a subset possible.
      else
      {
         i=n;
         j=sum;
         k=0;
         // Traverse the subset array to find the list of possible integers to form the subset and move them to result[].
         while (i>0 && j>0)
         {
            i--;
            if (subset[i][j] == false)
            {
               result[k] = set[i];
               k++;
               j=j-set[i];
            }
         }
         // Moving 0 after the last integer in result[] array to mark the completion of the subset in it.
         result[k] = 0;
      }
   }
}

// This function is called from main() to find the subset of cset which has the sum of csum and return the result in result[] array.
// The inputs csum and cset are in char type and result[] which is passed to main will be in integer format.
int isSubsetSummain(char *csum, char *cset, int result[])
{
   int sum = atoi(csum);
   int set[100];
   int i=0;
   char csetc[200];
   char *csettok;

   // Using strtok function to parse the cset and split the integers
   csetc == "";
   strcpy(csetc,cset);
   csettok = strtok(csetc," ");

   while (csettok != NULL)
   {
       set[i] = atoi(csettok);
       i++;
       csettok = strtok(NULL," ");
   }

   // Caling isSubsetSum function to find the subset
   isSubsetSum(set, i, sum, result);
}

int main(int argc, char **argv)
{
   int hflag = 0;
   int c;
   int i;
   int n;
   int otimelimit = 10;
   char *ipath = "./input.dat";
   char *opath = "./output.dat";
   char ch[200];
   char fch[200];
   opterr = 0;
   FILE *ifptr;
   FILE *ofptr;

   pid_t forkpids[100];
   pid_t w;
   int status;
   int j;
   char chtokch[200];
   char *chtok;
   char sum[10];
   char set[200];

   int result[200];
   char tempch[200];

   /* Parsing the command line arguments */
   while ((c = getopt (argc, argv, ":hi:o:t:")) != -1)
   {
      switch (c)
      {
      case 'h':
         hflag = 1;
         break;
      case 'i':
         ipath = optarg;
         break;
      case 'o':
         opath = optarg;
         break;
      case 't':
         otimelimit = atoi(optarg);
         break;
      case ':':
         fprintf (stderr, "Option -%c requires an argument.\n",optopt);
         return 1;
      case '?':
         if (isprint (optopt))
            fprintf (stderr, "Unknown option `-%c'.\n", optopt);
         else
            fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
         return 1;
      default:
         abort ();
      }
   }

   // Displaying the Help menu if help flag is set
   if (hflag == 1)
   {
      printf("logParse: Supported Arguments. \n");
      printf(" -i <input file name>    : If provided, input file name should follow the below format. Default is ./input.dat \n");
      printf("                           First line - Total number (N) of subset sum problems. \n");
      printf("                           Second line and so forth till N+1th line, set of integers separated by a SPACE/BLANK \n");
      printf(" -o <output file name>   : If provided, output of the subset sum problems will be written to this file. Default is ./output.dat \n");
      printf(" -t <overall time limit> : If provided, the process will terminate after this much elapsed time. Default is 10 seconds. \n");
   }
   else
   {
      // Initializing the sigaction structure to set the timer
      struct sigaction act;
      act.sa_handler = myhandler;
      act.sa_flags = 0;

      // Using sigaction's SIGALRM argument to trigger the alarm based on the elapsed time
      if ((sigemptyset(&act.sa_mask) || sigaction(SIGALRM, &act, NULL)) == -1)
      {
         perror("logParse: Error: Failed to set up handler for SIGALRM");
         exit(0);
      }
      // Setting the alarm for handler to be triggered after otimelimit set by user or 10 seconds by default
      alarm(otimelimit);

      // Displaying the input file, output file and the overall timelimit being used
      printf("Input File: %s \n",ipath);
      printf("Output File: %s \n",opath);
      printf("Overall Time Limit Used: %d \n",otimelimit);

      // Opening the input file in the read mode
      ifptr = fopen(ipath, "r");
      if (ifptr == NULL)
      {
         perror("logParse: Error: Input file: fopen()");
         exit(0);
      }
      // Opening the output file in the write mode
      ofptr = fopen(opath, "w");
      if (ofptr == NULL)
      {
         perror("logParse: Error: Output file: fopen()");
         exit(0);
      }

      // Reading the first line of the input file to get the number of the child processed to start
      fgets(ch, sizeof ch, ifptr);
      n = atoi(ch);

      // Looping based on the number mentioned in the first line of the file.
      for (i=0; i < n; i++)
      {
         // Reading the next line to get the sum and set to work on
         strcpy(ch,"");
         fgets(ch, sizeof ch, ifptr);

         // Forking off the child process and storing the pid in forkpids array for reporting down the line
         forkpids[i] = fork();

         // If there is an error in forking of a child process err it
         if(forkpids[i] < 0)
         {
            perror("logParse: Error: Starting the child process: fork()");
            exit(0);
         }
         // If pid is 0, then this is the child process - controlling it within this else if statement.
         else if(forkpids[i] ==0)
         {
            // Child Process Starts Here
            strcpy(sum,"");
            strcpy(set,"");
            chtokch == "";
            strcpy(chtokch, ch);

            // Using the strtok function and a temp variable chtokch to parse the sum out of the main ch variable which has the sum and set clubbed
            // Using a temporary variable as strtok function overwrites the parsing variable
            chtok = strtok(chtokch," ");
            strcpy(sum,chtok);

            chtok = strtok(NULL,"\n");
            strcpy(set,chtok);

            // Intializing the result set to all 0
            for (i = 0; i < 200; i++)
               result[i] = 0;

            // Calling the function to find the subset
            isSubsetSummain(sum,set,result);

            // Above function returns the subset in result[] if there is a subset of set[] with sum equal to given sum.
            // If there is no subset found within a second then result[0] is populated with -1.
            // If there is no subset possible then result[0] is populated with 0.

            // If there is no subset possible or a subset not found within a second, write appropriate messages in the output file
            if  (result[0] <= 0)
            {
               if (result[0] == -1)
               {
                  strcpy(fch,"");
                  snprintf(fch, sizeof(fch), "%i: No valid subset found within 1 second for the sum %s\n", getpid(), sum);
                  fputs(fch,ofptr);
               }
               else
               {
                  strcpy(fch,"");
                  snprintf(fch, sizeof(fch), "%i: No subset of numbers summed to %s \n", getpid(), sum);
                  fputs(fch,ofptr);
               }
            }
            // If there is a subset found, write the subset in expected format in the output file
            else
            {
               i=0;
               strcpy(fch,"");
               snprintf(fch, sizeof(fch), "%i:", getpid());
               while (result[i] != 0)
               {
                  strcpy(tempch,"");
                  if (i == 0)
                     snprintf(tempch, sizeof(tempch), "%s %d", fch, result[i]);
                  else
                     snprintf(tempch, sizeof(tempch), "%s + %d", fch, result[i]);
                  strcpy(fch, tempch);
                  i++;
               }
               strcpy(tempch,"");
               snprintf(tempch, sizeof(tempch), "%s = %s\n", fch,sum);
               strcpy(fch, tempch);
               fputs(fch,ofptr);
            }

            // Child Process Ends Here
            exit(0);
         }

         // if pid is > 0, then this is the parent process - making the parent process to wait until the current child process is complete
         else
         {
            w = waitpid(forkpids[i],&status,0);
         }
      }

      // Write the parent's process id and the process ids of all the child processes forked in the output file
      strcpy(ch,"");
      snprintf(ch, sizeof(ch), "Parent pid: %i\n", getpid());
      fputs(ch,ofptr);

      for (i=0; i < n; i++)
      {
         strcpy(ch,"");
         snprintf(ch, sizeof(ch), "Child %d pid: %i\n", i+1, forkpids[i]);
         fputs(ch,ofptr);
      }

      // Close the input and the output file
      fclose(ifptr);
      fclose(ofptr);
   }
   return 0;
}
