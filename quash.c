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
int	bgProcessIndex = 1;
char *  bgCommands[BG_PROCESS_LIMIT];

struct shellfeatures
{
	int inBackground;
	char * outFile;
	char * inFile;
};

void addJob( int pid, char * command )
{
	bgProcesses[ bgProcessIndex ] = pid;
	bgCommands[ bgProcessIndex++ ] = strdup( command );
}

void jobComplete()
{
	int status;
	int job_pid = waitpid(WAIT_ANY, &status, WNOHANG | WUNTRACED);
	int i;
	for( i = 0; i < bgProcessIndex; i++ )
		if( bgProcesses[i] == job_pid )
		{
			printf("[%i] %i finished %s\n", i, job_pid, bgCommands[i] );
			bgProcesses[i] = 0;
			if( i + 1 == bgProcessIndex )
				bgProcessIndex--;
			return;
		}
}


char * readCommand()
{
	char * command = (char * ) malloc(CMD_LENGTH + 1);
	printf("quash> ");
	fgets( command, CMD_LENGTH, stdin );
	return command;				
}

char ** getArgv( char * command, struct shellfeatures * shellFeatures)
{
	char * command_tmp = strdup( command );
	char * curArg = strtok(command_tmp, "= \n");
	char ** argv = (char**) malloc(ARG_LIMIT + 1);
	int argv_index = 0;
	while( curArg != NULL )
	{

		if (strcmp(curArg, "&") == 0) 
			shellFeatures->inBackground = 1;
		else if (strcmp(curArg, "<") == 0) 
			shellFeatures->inFile = strtok(NULL, " =\n");
		else if (strcmp(curArg, ">") == 0) 
			shellFeatures->outFile = strtok(NULL, " =\n");
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


void runExec( char ** argv, char ** envp, struct shellfeatures * shellFeatures, char * command )
{
		int child_pid = fork();
		if( child_pid == 0 )
		{
			if( shellFeatures->inFile )
			{
				FILE* inFile = fopen( shellFeatures->inFile, "r");
				dup2(fileno(inFile), fileno(stdin));
				fclose(inFile);
			}

			if( shellFeatures->outFile )
			{
				FILE* outFile = fopen( shellFeatures->outFile, "w+");
				dup2(fileno(outFile), fileno(stdout));
				fclose(outFile);
			}

			if( execve(argv[0], argv, envp) < 0 )
			{
				if ( shellFeatures->inBackground )
				exit(1);		
			}
			else
				exit(0);

		}
		else if ( ! shellFeatures->inBackground ) 
			wait(NULL);
		else
		{
			addJob( child_pid, command );
			printf("[JOBID] %i running in background\n", child_pid);
		}

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
	else if (strcmp( argv[0] , "jobs") == 0)
	{
		int i;
		for(i = 0; i < bgProcessIndex; i++) {
			if(bgProcesses[i])
			{
				printf("[%i] %i %s\n", i, bgProcesses[i], "command");
			}
		}

/*		if(nobackground) {
			printf("No background processes running\n");
		}*/
	}
	else if( access( argv[0], F_OK) != -1 && argv[0][0] == '/')	
		runExec( argv, envp, shellFeatures, command);
	else if( getAbsolute( argv, envp ) )
		runExec( argv, envp, shellFeatures, command );
	else
		printf("Could not find %s in PATH.\n", argv[0], strlen(argv[0]), argv[0][0]);
		//printf("Could not find %s.  Length is %i, first char is %i\n", argv[0], strlen(argv[0]), argv[0][0]);
	
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
