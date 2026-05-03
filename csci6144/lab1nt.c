/* Author:  Sathish Ramamoorthy
 * Course:  CSCI6144
 * Lab1:    Tiny command Interpreter in C on Windows NT
 */

#include <windows.h> 
#include<stdio.h> 
#include<string.h> 

int executeCommand(char *cmdLine) {

	/* CreateProcess parameters */ 
	STARTUPINFO startInfo; 
	PROCESS_INFORMATION procInfo; 

	/* Create processes */ 
	ZeroMemory(&startInfo, sizeof(startInfo)); 
	startInfo.cb = sizeof(startInfo); 

	if(!CreateProcess( 

		NULL,				/* File name of executable*/ 
		cmdLine,			/* Command line*/ 
		NULL,				/* Process inherited security*/ 
		NULL,				/* Thread inherited security*/ 
		TRUE,				/* Rights propagation */ 
		CREATE_NEW_CONSOLE,	/* Various creation flags*/ 
		NULL,				/* Environment variabkesr*/ 
		NULL,				/* Child's current directory*/ 
		&startInfo, 
		&procInfo ))

	{
		if (GetLastError() == 2) {
			printf("Unkown command\n");
			return 0;
		}
		fprintf(stderr, "CreateProcess failed on error %d\n", GetLastError()); 
		ExitProcess(0);
	} 

	/* Clean up, then terminate */ 
	printf("Parent process returns without waiting for child process.\n"); 
	return 0; 
}

int main(int argc, char *argv[]) { 

	char prompt[] = "> ";
	char cmdLine[80];
	int c, i; 

	for(;;){
		i = 0;
		cmdLine[0] = '\0';
		printf("%s",prompt);
		while((c = getchar()) != EOF && c != '\n'){
			cmdLine[i++] = tolower(c);
		}
		cmdLine[i] = '\0';

		if (strcmp(cmdLine, "") == 0) {
			continue;
		} else if (strcmp(cmdLine, "exit") == 0) {
			printf("Exiting shell"); 
			ExitProcess(0);
		} 

		/* strcpy(cmdLine, "c:\\winnt\\system32\\write.exe"); */
		/* strcpy(cmdLine, "notepad"); */
		executeCommand(cmdLine); 
	}
}