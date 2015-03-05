#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>


//may not need extern

const LISTSIZE = 100; 
extern const CMDSIZE = 1028;
extern char *cmdline; //[CMDSIZE];
int gener+ID, jobcount;


/* A process is a single process.  */
typedef struct job
{
  int id, argcount;
  //char **argv;                /* for exec */
  pid_t pid;                  /* process ID */
  //char completed;             /* true if process has completed */
  int running;
} job;



/* The active jobs are linked into a list.  This is its head.   */
int joblist[]



int main(int argc, char **argv, char **envp){
	
	
	getcmd(); //needs to read in input and increment job count
	while(jobcount > 0){
		printf("[ --QUASH-- ]
		//execute job i
		jobcount -= 1;
	}
	printf("Ending Quash \n");
	exit(0);
}
