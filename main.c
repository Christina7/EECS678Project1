#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>

#include "command.h"

extern const CMDSIZE = 1028;
extern char *cmdline; //[CMDSIZE];


/*
// A process is a single process
typedef struct process{
  //struct process *next;      // next process in pipeline 
  char **argv;                //for exec 
  pid_t pid;                   //process ID 
  char completed;             //true if process has completed 
  char stopped;               //true if process has stopped 
  int status;                  //reported status value 
} process;
// A job is a pipeline of processes.  


*/

/*
typedef struct job
{
  struct job *next;           //next active job 
  char *command;              //command line, used for messages 
  process *first_process;     //list of processes in this job 
  pid_t pgid;                 //process group ID 
  char notified;              //true if user told about stopped job 
  struct termios tmodes;      // saved terminal modes 
  int stdin, stdout, stderr;  // standard i/o channels 
} job;

//The active jobs are linked into a list.  This is its head.   
job *first_job = NULL;

*/


void parser(){
	 fgets(cmdline, CMDSIZE, stdin);
}




int main(int argc, char *argv[], char *envp[])
{
/*
 *	need parser
 *
 */
 


 
 
 /*	string word;
 	char c;
 	int i = 0;
	printf("\n[QUASH >> ] ");
	while((c != "\n") && (i < CMDSIZE)) {
		c = getchar();
		if (c == " "){
			i++;
		}
		else if(c == "|"){
			i++
		}
		else if(c == ">"){
			i++;
		}
		else if(c == "<"){
			i++
		}
		else{
			strcat(cmdline[i], c);
		}
		
		printf("[QUASH >> ] ");
	}
	printf("\n");
	return 0;
 */

/*
 *	interpreter for commands
 *
 */


/*
 *	
 *	other
 *
 *	file type for file input is .sh
 *
 */
}
