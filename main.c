#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>

typedef int bool;
#define true 1
#define false 0
//may not need extern

const LISTSIZE = 100;
const MAXARG = 10; 
extern const CMDSIZE = 1028;
extern char *cmdline; //[CMDSIZE];
int generID, jobcount;


/* A process is a single process.  */
typedef struct Job
{
    int id;
    int argcount;
 	char *args[MAX_ARGS];
    pid_t pid;                  /* process ID */
    int running;	//for background processes only
  	char * input;
	char * output;
} Job;

void initJob(struct  Job* j)
{
		j->input = (char*) malloc(10000);	
		j->output = (char*) malloc(10000);	

		j->id = generID;
		generID++;
		j->argcount = 0;
		//j->background = false;
		j->running = false;
		int i;
		for(i = 0; i < MAX_ARGS; i++){
			j->args[i] = (char*) malloc(256);
		}

}


/* The active jobs are linked into a list.  This is its head.   */
Job jobfore[LISTSIZE];
Job jobback[LISTSIZE];



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
