#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CMD_LENGTH 100
#define ARG_LIMIT 100

char * readCommand()
{
	char * command = (char * ) malloc(CMD_LENGTH + 1);
	printf("quash> ");
	fgets( command, CMD_LENGTH, stdin );
	return command;				
}

char ** getArgv( char * command )
{
	char * curArg = strtok(command, "= \n");
	char ** argv = (char**) malloc(ARG_LIMIT + 1);
	int argv_index = 0;
	while( curArg != NULL )
	{
		argv[ argv_index ] = ( char * ) malloc( strlen( curArg ) );
		strcpy (argv[ argv_index++ ], curArg);
		curArg = strtok(NULL, "= \n");
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
	char ** argv = getArgv( command );
	if( strcmp( command, "\n" ) == 0 )
		return;
	else if (strcmp( command , "exit") == 0 || strcmp( command , "quit") == 0)
		exit(0);
	else if (strcmp( command , "set") == 0) 
	{
		setenv( argv[1], argv[2], 1 );
		printf("set %s to %s\n", argv[1], argv[2]);


		/*strcpy( argv[0], "export" );
		getAbsolute( argv, envp );
		runExec( argv, envp );*/
	}
	else if( access( argv[0], F_OK) != -1 && argv[0][0] == '/')	
		runExec( argv, envp );
	else if( getAbsolute( argv, envp ) )
		runExec( argv, envp );
	else
		printf("Could not find %s.  Length is %i, first char is %i\n", argv[0], strlen(argv[0]), argv[0][0]);
	
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
