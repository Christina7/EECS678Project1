#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CMD_LENGTH 100
#define ARG_LIMIT 100

char * readCommand()
{
	char * command = (char * ) malloc(CMD_LENGTH + 1);
	printf("quash>");
	fgets( command, CMD_LENGTH, stdin );
	return command;				
}

char ** getArgv( char * command )
{
	char * curArg = strtok(command, " \n");
	char ** argv = (char**) malloc(ARG_LIMIT + 1);
	int argv_index = 0;
	while( curArg != NULL )
	{
		argv[ argv_index ] = ( char * ) malloc( strlen( curArg ) );
		strcpy (argv[ argv_index++ ], curArg);
		curArg = strtok(NULL, " \n");
	} 
	argv[argv_index] = NULL;
	return argv;
}

void runExec( char ** argv, char ** envp )
{
		if( fork() == 0 )
		{
			if( execve(argv[0], argv, envp) < 0 )
				exit(1);		
			else
				exit(0);
		}
		else 
			wait(NULL);

}

char getAbsolute(char ** argv, char ** envp)
{
	char * pathIndex;
	pathIndex = strtok(getenv("PATH"),":\n");

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
	if( strcmp( command, "\n" ) == 0 )
		return;
	char ** argv = getArgv( command );
	if( access( argv[0], F_OK) != -1 )	
		runExec( argv, envp );
	else if( getAbsolute( argv, envp ) )
		runExec( argv, envp );
	else
		printf("Could not find %s.  Length is %i, first char is %i\n", command, strlen(command), command[0]);
	
}


int main(int argc, char * argv[], char **envp) 
{
	char * currentCommand;
	int i = 0;
	while (i++ < 100)
	{
		currentCommand = readCommand();
		runCommand(currentCommand, envp);
	}
}
