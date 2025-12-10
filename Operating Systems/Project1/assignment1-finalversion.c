/* Utility to traverse a specified directory in a depth first order and print the file contents */
/* This utility uses getopt() to get file options like h - help message, I n - Changing the indent by n spaces, t - file type info
 * L - symlink, p - permission bits, i - links to file in inode table, u - file uid, g - group id, s - filesize, d - last modified time
 * l - printing information about file as though options tpiugs are all specified
 * This utility uses perror() and stderr to print error messages */

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdint.h>
#include <sys/sysmacros.h>

void listdir(const char *name, int indent, int tflag, int pflag, int iflag, int uflag, int gflag, int sflag, int lflag, int dflag, int Lflag)
{
   DIR *dir;
   struct dirent *entry;
   struct stat filestat;
   struct stat filelstat;
   struct passwd  *pwd;
   struct group   *grp;
   struct tm      *tm;
   char path[1024];
   char display[200];
   char tempdisplay[200];
   char datestring[256];
   long long sizeval;
   long long sizesplit;
   char sizeunit[1];

   /* Opening the directory */
   dir = opendir(name);
   if (dir == NULL)
      return;

   /* Reading the directory contents */
   while ((entry = readdir(dir)) != NULL)
   {
      path == "";
      /* Appending the filename to the directory path */
      snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);

      /* Reading the file statistics of the file using stat() */
      if (stat(path,&filestat) == -1)
      {
         perror("dt: Error: stat()");
         printf("%s \n", path);
         exit(EXIT_FAILURE);
      }

      /* Reading the file statistics of the file using lstat() to obtain the symlink information */
      if (lstat(path,&filelstat) == -1)
      {
         perror("dt: Error: lstat()");
         printf("%s \n", path);
         exit(EXIT_FAILURE);
      }

      /* Getting the userid information using the getpwuid() */
      if ((pwd = getpwuid(filestat.st_uid)) == NULL)
      {
         perror("dt: Error: getpwuid()");
         printf("%s \n", filestat.st_uid);
         exit(EXIT_FAILURE);
      }

      /* Getting the groupid information using the getgrgid() */
      if ((grp = getgrgid(filestat.st_gid)) == NULL)
      {
         perror("dt: Error: getgrgid()");
         printf("%s \n", filestat.st_gid);
         exit(EXIT_FAILURE);
      }

      /* Omitting the current and previous directories to bypass entering into an infinite loop */
      if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
         continue;

      /* Populating the main string display with the appropriate indentation and the filename */
      display == "";
      snprintf(display, sizeof(display), "%*s", indent, "");

      tempdisplay == "";
      snprintf(tempdisplay, sizeof(tempdisplay), "%s  %s ", display, entry->d_name);
      strcpy(display, tempdisplay);

      /* If length of the indent and filename exceeds 50 characters then display the other file details in a newline */
      if (strlen(display)<50)
         sprintf(display, "%s  %*s ", display, (50-strlen(display)), "");
      else
      {
         printf("%s \n", display);
         sprintf(display, "   %*s", 50, "");
      }

      /* Appending the file type info got from st_mode of filelstat() to the main display string */
      if (tflag == 1)
      {
         tempdisplay == "";
         switch (filelstat.st_mode & S_IFMT)
         {
            case S_IFBLK:   snprintf(tempdisplay, sizeof(tempdisplay), "%s  Block Device      ", display); break;
            case S_IFCHR:   snprintf(tempdisplay, sizeof(tempdisplay), "%s  Character  Device ", display); break;
            case S_IFDIR:   snprintf(tempdisplay, sizeof(tempdisplay), "%s  Directory         ", display); break;
            case S_IFIFO:   snprintf(tempdisplay, sizeof(tempdisplay), "%s  FIFO/Pipe         ", display); break;
            case S_IFLNK:   snprintf(tempdisplay, sizeof(tempdisplay), "%s  Symbolic Link     ", display); break;
            case S_IFREG:   snprintf(tempdisplay, sizeof(tempdisplay), "%s  Regular File      ", display); break;
            case S_IFSOCK:  snprintf(tempdisplay, sizeof(tempdisplay), "%s  Socket            ", display); break;
            default:        snprintf(tempdisplay, sizeof(tempdisplay), "%s  Unknown           ", display); break;
         }
         strcpy(display, tempdisplay);
      }

      /* Appending the permission bit information using the st_mode of filestat() to the main display string*/
      if (pflag == 1)
      {
         tempdisplay == "";
         snprintf(tempdisplay, sizeof(tempdisplay), "%s  %s%s%s%s%s%s%s%s%s%s", display,
       ((S_ISDIR(filelstat.st_mode) ? "d" : ((S_ISLNK(filelstat.st_mode) ? "l" : "-")))),
                                            (((filestat.st_mode & S_IRUSR)) ? "r" : "-"),
                                            (((filestat.st_mode & S_IWUSR)) ? "w" : "-"),
                                            (((filestat.st_mode & S_IXUSR)) ? "x" : "-"),
                                            (((filestat.st_mode & S_IRGRP)) ? "r" : "-"),
                                            (((filestat.st_mode & S_IWGRP)) ? "w" : "-"),
                                            (((filestat.st_mode & S_IXGRP)) ? "x" : "-"),
                                            (((filestat.st_mode & S_IROTH)) ? "r" : "-"),
                                            (((filestat.st_mode & S_IWOTH)) ? "w" : "-"),
                                            (((filestat.st_mode & S_IXOTH)) ? "x" : "-"));
         strcpy(display, tempdisplay);
      }

      /* Appending the number of links to file in the inode table to the main display string */
      if (iflag == 1)
      {
         tempdisplay == "";
         snprintf(tempdisplay, sizeof(tempdisplay), "%s  %d", display, filestat.st_nlink);
         strcpy(display, tempdisplay);
      }

      /* Appending the userid information to the main display string */
      if (uflag == 1)
      {
         tempdisplay == "";
         if ((pwd = getpwuid(filestat.st_uid)) != NULL)
            snprintf(tempdisplay, sizeof(tempdisplay), "%s  %-8.8s", display, pwd->pw_name);
         else
            snprintf(tempdisplay, sizeof(tempdisplay), "%s  %-8d", display, filestat.st_uid);
         strcpy(display, tempdisplay);
      }

      /* Appending the userid information to the main display string */
      if (gflag == 1)
      {
         tempdisplay == "";
         if ((grp = getgrgid(filestat.st_gid)) != NULL)
            snprintf(tempdisplay, sizeof(tempdisplay), "%s  %-8.8s", display, grp->gr_name);
         else
            snprintf(tempdisplay, sizeof(tempdisplay), "%s  %-8d", display, filestat.st_gid);
         strcpy(display, tempdisplay);
      }

      /* Appending the filesize info to the main display string */
      /* Filesize which is in bytes is converted to TB,GB,MB,KB based on which order of 1024 the filesize is */
      if (sflag ==1)
      {
         tempdisplay == "";
         sizeval = 0.0;
         strcpy(sizeunit, "B");
         sizeval = (intmax_t)filestat.st_size;
         sizesplit = 1099511627776;

         if (sizeval >= sizesplit)
         {
            sizeval = sizeval / sizesplit;
            strcpy(sizeunit, "T");
         }
         else
         {
            sizesplit = 1073741824;
            if (sizeval >= sizesplit)
            {
               sizeval = sizeval / sizesplit;
               strcpy(sizeunit, "G");
            }
            else
            {
               sizesplit = 1048576;
               if (sizeval >= sizesplit)
               {
                  sizeval = sizeval / sizesplit;
                  strcpy(sizeunit, "M");
               }
               else
               {
                  if (sizeval >= 1024)
                  {
                     sizeval = sizeval / (1024);
                     strcpy(sizeunit, "K");
                  }
                  else
                     strcpy(sizeunit, "B");
               }
            }
         }

         snprintf(tempdisplay, sizeof(tempdisplay), "%s  %4lld %s", display, sizeval, sizeunit);
         strcpy(display, tempdisplay);
      }

      /* Appending the last modified date and time information of the file to the maind isplay string */
      if (dflag == 1)
      {
         tempdisplay == "";
         tm = localtime(&filestat.st_mtime);
         strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);
         snprintf(tempdisplay, sizeof(tempdisplay), "%s  %.*s", display, 16, datestring);
         strcpy(display, tempdisplay);
      }

      /* Printing the main display string */
      printf("%s \n", display);

      /* Performing the recursive call during two conditions */
      /* 1. When the filetype is a directory */
      /* 2. When the filetype is a symlink and the -L flag is set */

      if( (S_ISDIR(filelstat.st_mode)) || (S_ISLNK(filelstat.st_mode) && Lflag == 1) )
         listdir(path, (indent + indent), tflag, pflag, iflag, uflag, gflag, sflag, lflag, dflag, Lflag);
   }
   /* Closing the directory that was opened using opendir() and read using readdir() */
   while ((closedir(dir) == -1) && (errno == EINTR)) ;
}

int main(int argc, char **argv)
{
   int hflag = 0;
   int tflag = 0;
   int pflag = 0;
   int iflag = 0;
   int uflag = 0;
   int gflag = 0;
   int sflag = 0;
   int lflag = 0;
   int dflag = 0;
   int Lflag = 0;
   char *ivalue = NULL;
   int index;
   int c;
   int indentvalue = 4;
   char path[100] = "";
   opterr = 0;

   /* Parsing the command line arguments */
   while ((c = getopt (argc, argv, ":htpiugsldLI:")) != -1)
   {
      switch (c)
      {
      case 'h':
         hflag = 1;
         break;
      case 'I':
         ivalue = optarg;
         break;
      case 't':
         tflag = 1;
         break;
      case 'p':
         pflag = 1;
         break;
      case 'i':
         iflag = 1;
         break;
      case 'u':
         uflag = 1;
         break;
      case 'g':
         gflag = 1;
         break;
      case 's':
         sflag = 1;
         break;
      case 'l':
         lflag = 1;
         tflag = 1;
         pflag = 1;
         iflag = 1;
         uflag = 1;
         gflag = 1;
         sflag = 1;
         break;
      case 'd':
         dflag = 1;
         break;
      case 'L':
         Lflag = 1;
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

   /* Parsing the dirname entered by the user. If user does not enter any dirname default is current directory */
   if (optind<argc)
      strcpy(path,argv[optind]);
   else
      strcpy(path,".");

   /* Converting the indentvalue from ASCII to integer */
   if (ivalue != NULL)
      indentvalue = ((int) *ivalue) - 48;
   if (indentvalue < 0)
      indentvalue = 0;

   /* Displaying the Help menu if help flag is set else invoke the listdir() to display the files in the specified directory */
   if (hflag == 1)
   {
      printf("dt: Supported Arguments. \n");
      printf(" -I <indentation value> : Indent as per the indentation value provided, if not indents with default of 4 spaces. \n");
      printf(" -t : Print information on file type. \n");
      printf(" -p : Print permission bits of the file. \n");
      printf(" -i : Print number of links to file in inode table. \n");
      printf(" -u : Print UID associated with the file. \n");
      printf(" -g : Print GID associated with the file. \n");
      printf(" -s : Print size of the file. \n");
      printf(" -l : Print all details of the file related to -t -p -i -u -g -s arguments. \n");
      printf(" -d : Print last updated time of the file. \n");
      printf(" -L : Print Symbolics. Symbolics are not included by default. \n");
   }
   else
      listdir(path , indentvalue, tflag, pflag, iflag, uflag, gflag, sflag, lflag, dflag, Lflag);

   return 0;
}
