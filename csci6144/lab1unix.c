/*
 * Author	- Sathish Ramamoorthy
 * CSCI6144 	- Operation System Design
 * Lab1		- Tiny Command Interpreter
 */

#include <stdio.h> 

int main() { 

	int pid, i; 
	int x, y;
	char prompt[] = "os> ";
	char command[100], *args[100], inputline[100];
	printf("Lab 1 tiny command interpreter process id is %d.\n", getpid() );

	for(;;) {
      i = 0;
      command[0] = '\0';
      printf("%s",prompt);

		if (gets(inputline) == NULL) {
			printf("Exiting\n");
			exit(0);
		}

		if (strchr(inputline, '|') != NULL) {
			printf("a pipe needed\n");
			doPipe(inputline);
			continue;
		}

		parse(inputline, args);

		if (strcmp(inputline, "exit") == 0) {
			printf("Goodbye\n");
			exit(0);
		}

		pid = fork(); 	/* create a child process: fork () system call */ 

		if (pid < 0) {	/* error occurred */ 
			fprintf(stderr,"Fork Failed"); 
			exit (-1);
		} else if (pid==0) {/* child process */ 
			execvp(*args, args);/* load new code*/ 
/*
		perror(*args);
		exit(1);
*/
		} else {/* parent process */ 
			wait(NULL); /* parent will wait for the child to complete */ 
			printf("***Child %i complete***\n", pid); 
		}
	}
}

/* Process command with pipe */
doPipe(char *inputline) {
	char *c1, *c2;
	char *c1args[100], *c2args[100];
	int fd[2], pid;
		FILE *fp1, *fp2;
		char line[130];

	c1 = (char *)strtok(inputline, "|");
	c2 = (char *)strtok(NULL, "");
	printf("%s, %s\n", c1, c2);

	pipe(fd);
	pid = fork();
	if (pid < 0) {	/* error occurred */ 
		fprintf(stderr,"Fork Failed"); 
		exit (-1);
	} else if (pid==0) {/* child process */ 
		close(fd[0]);
		fp1 = popen(c1, "r");
		fp2 = popen(c2, "w");
		while ( fgets( line, sizeof line, fp1)) {
			fprintf(fp2, "%s", line);
  		}
  		pclose(fp1);
  		pclose(fp2);
		exit(1);
	} else {/* parent process */ 
		close(fd[1]);
		wait(NULL); /* parent will wait for the child to complete */ 
		printf("***Child %i complete***\n", pid); 
	}
}

/* Parse input */
parse(buf, args) 
char *buf;
char **args;
{
	while (*buf != NULL) {
      while ((*buf == ' ') || (*buf == '\t'))
      	*buf++ = NULL;
         *args++ = buf;
         while ((*buf != NULL) && (*buf != ' ') && (*buf != '\t'))
            buf++;
   }
   *args = NULL;
}
