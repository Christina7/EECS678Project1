#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

int generID, jobcount;


struct Job
{
    int id;		//job id
    int argcount;	//number of arguments a job has
 	char *args[MAXARG];		//arguments of a job i.e. cd ~/doc/place
    pid_t pid;             //process ID
    bool running;	//for background processes only
  	bool bkgrd;		//if background process
  	char * input;	//job's input comes from 
	char * output;	//job's output goes to
};


//arrays of foreground and background jobs
Job jobs[LISTSIZE];
//Job jobback[LISTSIZE];


//-----------------------------------------------------------------------------------------------


void initJob(struct  Job* j)
{
		j->input = (char*) malloc(10000);	
		j->output = (char*) malloc(10000);	

		j->id = generID;
		generID++;
		j->argindex = 0;
		j->bkgrd = false;
		j->running = false;
		int i;
		for(i = 0; i < MAXARG; i++){
			j->args[i] = (char*) malloc(256);
		}

}



//-----------------------------------------------------------------------------------------------


void deallocJob(struct Job * j)
{

		int i; for(i = 0; i < MAXARG; i++){
			free(j->args[i]);
		}

}



//-----------------------------------------------------------------------------------------------

/*
 *	gets input from user, parses input into commands 
 */
void getcmd()
{
	char *cmdline; //[CMDSIZE];
	fgets(cmdline, CMDSIZE+1, stdin);
	
	char currchar
	int curri = 0;	//current index in line
	int size = 0;  //index of the start of a new word 
	int jobi = 0;
	int argi = 0;
	char *cmd;
	
	// while not end of line create words from 
	while(currchar != \0){
		strncpy(currchar, cmdline + curri, 1);	//make first char in cmdline currchar
		if ((isAlpha(currchar)) || ((currchar != "&") && (currchar != "<") && (currchar != ">") && (currchar != "|") && (currchar != " ") && (currchar != "\n"))){	//if not important char 
			strcat(cmd, currchar, 1);	//concatenate onto cmd
			curri += 1;	//move to next char in cmdline
		}
		else{ //is not letter, puts it into job arg , need to check if | & > < 
			strcpy(jobs[jobi].args[argi], cmd, sizeof(cmd)); //puts cmd into arg of current job 
			jobs[jobi].argcount += 1;
			argi += 1; //
			cmd = NULL;	//reset cmd
			
			switch(currchar){
				case "&":
					jobs[jobi].bkgrd = true;	//current job is background job
					break;
				case "<":
					curri += 1; //move to next char in cmdline and concat onto cmd
					strncpy(currchar, cmdline + curri, 1);	//copy char into currchar
					
					while(currchar == " "){
						curri += 1;
						strncpy(currchar, cmdline + curri, 1);
					}
					
					while((isAlpha(currchar)) || ((currchar != "&") && (currchar != "<") && (currchar != ">") && (currchar != "|") && (currchar != " ") && (currchar != "\n"))){
						strcat(cmd, currchar, 1); //
						curri += 1;
						strncpy(currchar, cmdline + curri, 1);
					}
					strcpy(jobs[jobi].output, cmd, sizeof(cmd));
					
					//need to check if fails
					if (strlen(jobs[currJob].output) == 0) {
						perror ("Error opening input file\n");
					}
										
					break;
				case ">":
					curri += 1; //move to next char in cmdline and concat onto cmd
					strncpy(currchar, cmdline + curri, 1);	//copy char into currchar
					
					while(currchar == " "){
						curri += 1;
						strncpy(currchar, cmdline + curri, 1);
					}
					
					while((isAlpha(currchar)) || ((currchar != "&") && (currchar != "<") && (currchar != ">") && (currchar != "|") && (currchar != " ") && (currchar != "\n"))){
						strcat(cmd, currchar, 1); //
						curri += 1;
						strncpy(currchar, cmdline + curri, 1);
					}
					strcpy(jobs[jobi].input, cmd, sizeof(cmd));
					
					//need to check if fails
					if (strlen(jobs[currJob].input) == 0) {
						perror ("Error opening input file\n");
					}
					
					break;
				case "|":
					jobs[jobi].bkgrd = true;	//current job is a background job
					jobs[currJob].argcount = argi; 	//current job has this many arguments
					jobi += 1;	// move to next job
					argi = 0;	// reset argi index
					break;
				case "\n":
					jobi += 1;	//new job
					break;
				case " ":
					curri += 1;	//skip space
					break;
				default :
					printf("Switch default at line 154 in getcmd \n check input \n");
					break;
			}
		

	}
	
	jobcount += 1;
};



//-----------------------------------------------------------------------------------------------
/* will execute all new jobs from getcmd  need to keep track of what index we left off on 
void execute(){
	int jobi = 0;
	int argi = 0;
	
	while(jobcount > 0){
		switch(jobs[jobi].args[argi]){
			case "exit":
				printf("Exiting quash");
				exit(0);
				break;
				
			case "quit":
				printf("Exiting quash");
				exit(0);
				break;
				
			case "set":
			
				break;
			
			case "cd":
				if (jobs[i].argcount < 2){
					if(chdir(getenv("HOME")) < 0) {
						printf("ERROR: changing director to home\n");
						exit(1);
					}
				} 
				else {
					if(chdir(jobs[i].args[1]) < 0){
						printf("ERROR: changing directory to %s\n", jobs[i].args[1]);
						exit(1);
					}
				}			
				break;
			
			case "jobs":
			
				break;
			
			default:
				if ((jobs[jobi].input != NULL) || (jobs[jobi].input != NULL))
			
			
			
			
			
				break;
		
		}
		
		jobcount -= 1;	
	}
}
*/

//-----------------------------------------------------------------------------------------------

// to test getcmd

void printJobs(){
    for (i = 0; i < jobcount; i++){
    	printf("\n %s ", i);
    	for (int k; k < job[i].argcount; k++){
    		printf("%s ",job[i].args[k])
    	}
    }
}



//-----------------------------------------------------------------------------------------------

int main(int argc, char **argv, char **envp){
	
	int i; 
	for( i = 0; i < LISTSIZE; i++)
	{
		initJob(&jobs[i]);
	}

    printf(" It has been initialized.\n");
    

	
	printf("[ --QUASH-- ]");
	getcmd(); // gets first job needs to read in input and increment job count
	
	printJobs();
	
	
	/*
	while(1){
		//execute job i
		
		printf("[ --QUASH-- ]");
		getcmd(); //gets next job
		
		//jobcount -= 1;
		printf("\n");
	}
	printf("Ending Quash \n");
	exit(0);
	*/
}




