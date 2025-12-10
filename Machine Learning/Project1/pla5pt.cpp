
#include <stdio.h>

static int arraymax=5;

/* blue plus array */
static int bparray[5][2] = {{1,4}, {8,14}, {1,7}, {2,17},{8,17}};

/* red minus array */
static int rmarray[5][2] = {{16,18}, {20,9}, {19,2}, {16,5},{14,5}};

static double c;
static int d, k;
static double w0;
static double w1;
static double w2;

int scanbparray(void)
{
    int i=0;
    double sol=0;
    do
    {
       sol = w0 + (w1*bparray[i][0]) + (w2*bparray[i][1]);
       //printf("scanbparray sol: %lf\n", sol);
       i = i + 1;
    } while (sol >= 0 && i < arraymax);
    return i;
}

int scanrmarray(void)
{
    int i=0;
    double sol=0;
    do
    {
       sol = w0 + (w1*rmarray[i][0]) + (w2*rmarray[i][1]);
       //printf("scanrmarray sol: %lf\n", sol);
       i = i + 1;
    } while (sol <= 0 && i < arraymax);
   return i;
}


int main() {
   int i, j;
 
   /* output bparray element's value */
   for ( i = 0; i < arraymax; i++ ) 
   {
      printf("bparray[%d] = %d,%d\n", i, bparray[i][0],bparray[i][1]);
   }

   /* output rmarray element's value */
   for ( i = 0; i < arraymax; i++ ) 
   {
      printf("rmarray[%d] = %d,%d\n", i, rmarray[i][0],rmarray[i][1]);
   }

   c=0.1;
   d=1;
   k=1;
   w0=0;
   w1=1;
   w2=0.5;

   printf("Initial values of the equation is as below...\n");
   printf("c=%lf\n",c);
   printf("d=%d\n",d);
   printf("k=%d\n",k);
   printf("w0=%lf\n",w0);
   printf("w1=%lf\n",w1);
   printf("w2=%lf\n",w2);
   printf("Hence initial equation to start with is: 0=%lf+%lfx1+%lfx2\n", w0, w1, w2);

   char corr_req = 'Y';
   int x1, x2, bprc, rmrc;

   do
   {
      /* scan bparray and make the equation to learn */
      bprc = scanbparray();
      printf("bprc=%d; w0 = %lf; w1 = %lf; w2 = %lf\n",bprc,w0,w1,w2);
      if (bprc >= arraymax) 
      { 
         corr_req='N';
         /* scan rmarray and make the equation to learn */
         rmrc = scanrmarray();
	 printf("rmrc=%d; w0 = %lf; w1 = %lf; w2 = %lf\n",rmrc,w0,w1,w2);
         if (rmrc >= arraymax) {corr_req = 'N';}
         else
         {
            d = -1;
            w0 = w0 + (c*d*k);
	    w1 = w1 + (c*d*rmarray[rmrc][0]);
	    w2 = w2 + (c*d*rmarray[rmrc][1]);
            corr_req = 'Y';
         }
      }
      else
      {
         d = 1;
         w0 = w0 + (c*d*k);
         w1 = w1 + (c*d*bparray[bprc][0]);
         w2 = w2 + (c*d*bparray[bprc][1]);
         corr_req = 'Y';
      }
     // printf("corr_req flag = %c\n",corr_req);
   } while (corr_req == 'Y');

   printf("Final values of the equation is as below...\n");
   printf("c=%lf\n",c);
   printf("d=%d\n",d);
   printf("k=%d\n",k);
   printf("w0=%lf\n",w0);
   printf("w1=%lf\n",w1);
   printf("w2=%lf\n",w2);
   printf("Hence final equation to end with is: 0=%lf+%lfx1+%lfx2\n", w0, w1, w2);
   
   return 0;
}


