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
//const int generates error variably mod. at file scope
#define LISTSIZE 100
#define MAXARG 10
#define CMDSIZE 1028
char *cmdline; //[CMDSIZE];
int generID, jobcount;


/* A process is a single process.  */
typedef struct Job
{
    int id;
    int argcount;
 	char *args[MAXARG];
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
		for(i = 0; i < MAXARG; i++){
			j->args[i] = (char*) malloc(256);
		}

}

void deallocJob(struct Job * j)
{

		int i; for(i = 0; i < MAXARG; i++){
			free(j->args[i]);
		}

}

//arrays of foreground and background jobs
Job jobfore[LISTSIZE];
Job jobback[LISTSIZE];

void getcmd()
{
	//fgets(line, MAX_LENGTH, stdin);
};

int main(int argc, char **argv, char **envp){
	
	int i; 
	for( i = 0; i < LISTSIZE; i++)
	{
		initJob(&jobfore[i]);
	}

	for( i = 0; i < LISTSIZE; i++)
	{
		initJob(&jobback[i]);
	}
    printf(" It has been initialized.\n");
	/*
	getcmd(); //needs to read in input and increment job count
	
	while(jobcount > 0){
		printf("[ --QUASH-- ]");
		//execute job i
		jobcount -= 1;
	}
	printf("Ending Quash \n");
	exit(0);
	*/
}




