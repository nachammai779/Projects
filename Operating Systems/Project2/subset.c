#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
  
// Returns true if there is a subset of set[] with sun equal to given sum
bool isSubsetSum(int set[], int n, int sum)
{
   // Base Cases
   if (sum == 0)
     return true;
   if (n == 0 && sum != 0)
     return false;
  
   // If last element is greater than sum, then ignore it
   if (set[n-1] > sum)
     return isSubsetSum(set, n-1, sum);
  
   /* else, check if sum can be obtained by any of the following
      (a) including the last element
      (b) excluding the last element   */
   return isSubsetSum(set, n-1, sum) ||
                        isSubsetSum(set, n-1, sum-set[n-1]);
}
  
// Driver program to test above function
int isSubetmain()
{
  int set[] = {3, 6, 107, 25, 1, 3, 7};
  int sum = 10;
  int n = sizeof(set)/sizeof(set[0]);
  if (isSubsetSum(set, n, sum) == true)
     printf("Found a subset with given sum \n");
  else
     printf("No subset with given sum \n");
  return 0;
}

int main()
{
    FILE *ifptr;
    FILE *ofptr;
    char ifile[15] = "./input.dat";
    char ofile[15] = "./output.dat";
    char ch[200];
    int n;
    int i;

    ifptr = fopen(ifile, "r");
    if (ifptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }

    ofptr = fopen(ofile, "w");
    if (ofptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }

    fgets(ch, sizeof ch, ifptr);
    n = atoi(ch);

    for (i=1; i <= n; i++)
    {
        strcpy(ch,"");
        fgets(ch, sizeof ch, ifptr);
        printf ("ch inside %s\n", ch);
        fputs(ch,ofptr);
    }
    
    fclose(ifptr);
    fclose(ofptr);
}
