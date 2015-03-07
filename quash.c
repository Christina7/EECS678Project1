#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define CMD_LENGTH 100
#define ARG_LIMIT 100
#define BG_PROCESS_LIMIT 100

int	bgProcesses[BG_PROCESS_LIMIT];
int	bgProcessIndex = 0;

struct shellfeatures
{
	int inBackground;
};

char * readCommand()
{
	char * command = (char * ) malloc(CMD_LENGTH + 1);
	printf("quash> ");
	fgets( command, CMD_LENGTH, stdin );
	return command;				
}

char ** getArgv( char * command, struct shellfeatures * shellFeatures)
{
	char * curArg = strtok(command, "= \n");
	char ** argv = (char**) malloc(ARG_LIMIT + 1);
	int argv_index = 0;
	while( curArg != NULL )
	{

		if (strcmp(curArg, "&") == 0) 
			shellFeatures->inBackground = 1;
		else			
		{
			argv[ argv_index ] = ( char * ) malloc( strlen( curArg ) );
			strcpy (argv[ argv_index++ ], curArg);
		}
		curArg = strtok(NULL, "= \n");
	} 
	argv[argv_index] = NULL;
	return argv;
}

void storeBackgroundChild( int pid )
{
	bgProcesses[ bgProcessIndex++ ] = pid;
}

void runExec( char ** argv, char ** envp, struct shellfeatures * shellFeatures )
{
		int child_pid = fork();
		if( child_pid == 0 )
		{
			if ( shellFeatures->inBackground )
				storeBackgroundChild( child_pid );
			if( execve(argv[0], argv, envp) < 0 )
			{
				if ( shellFeatures->inBackground )
					printf("[JOBID] PID finished COMMAND abnormally\n");
				exit(1);		
			}
			else
		//		if ( shellFeatures->inBackground )
		//			printf("[JOBID] PID finished COMMAND\n");
				exit(0);

		}
		else if ( ! shellFeatures->inBackground ) 
			wait(NULL);
		else
			printf("[JOBID] %i running in background\n", child_pid);

}

char getAbsolute(char ** argv, char ** envp)
{
	char * fullPath = malloc(10000);
        strcpy(fullPath, getenv("PATH"));
	char * pathIndex = strtok(fullPath,":\n");
	while(pathIndex != NULL){
		char absPath[100];
		strcpy(absPath, pathIndex);
		strcat(absPath, "/");
		strcat(absPath, argv[0]);
		if( access(absPath, F_OK) != -1 )
		{
			argv[0] = malloc( CMD_LENGTH );
			strcpy( argv[0] , absPath);
			return 1;
		}
		pathIndex = strtok(NULL,":\n");
	}
	return 0;
}



void runCommand(char * command, char **envp)
{
	struct shellfeatures * shellFeatures = ( struct shellfeatures * ) malloc( sizeof( struct shellfeatures ) );
	char ** argv = getArgv( command, shellFeatures );
	if( strcmp( command, "\n" ) == 0 )
		return;
	else if (strcmp( argv[0] , "exit") == 0 || strcmp( argv[0] , "quit") == 0)
		exit(0);
	else if (strcmp( argv[0] , "set") == 0) 
	{
		setenv( argv[1], argv[2], 1 );
		printf("set %s to %s\n", argv[1], argv[2]);


		/*strcpy( argv[0], "export" );
		getAbsolute( argv, envp );
		runExec( argv, envp );*/
	}
	else if (strcmp( argv[0] , "cd") == 0) 
	{
		chdir(
				argv[1] == NULL?
					getenv("HOME")
			:
				argv[1]
		     );
	}
	else if( access( argv[0], F_OK) != -1 && argv[0][0] == '/')	
		runExec( argv, envp, shellFeatures );
	else if( getAbsolute( argv, envp ) )
		runExec( argv, envp, shellFeatures );
	else
		printf("Could not find %s in PATH.\n", argv[0], strlen(argv[0]), argv[0][0]);
		//printf("Could not find %s.  Length is %i, first char is %i\n", argv[0], strlen(argv[0]), argv[0][0]);
	
}

void jobComplete()
{
	int status;
	int job_pid = waitpid(WAIT_ANY, &status, WNOHANG | WUNTRACED);
	printf("Job is done.\n");
	printf("[%i] PID finished COMMAND\n", job_pid);
}

int main(int argc, char * argv[], char **envp) 
{
	char * currentCommand;
	signal(SIGCHLD, jobComplete);	
	int i = 0;
	while (i++ < 100)
	{
		currentCommand = readCommand();
		runCommand(currentCommand, envp);
	}
}
