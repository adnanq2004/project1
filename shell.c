#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int argcount(char *line){
  int i;
  int c = 1;
  for (i = 0; i < strlen(line); i ++){
    if (line[i] == ' ')
      c ++;
  }
  return c;
}

char **parse_args( char *line ) {
    char *temp;
    int counter;
    char **returnline = calloc(5, sizeof(char *));
    counter = 0;
    while ((temp = strsep(&line, " "))) {
        returnline[counter] = temp;
        counter++;
    }
    return returnline;
}

/*
static void sighandler(int sig) {
	if (sig == SIGINT) {
		exit(0);
	}
}
*/

int main() {

	int ex = 0;
	//int waiting = 0;
	int child = 0;
	int i = 1;
	while (i) {
		printf("\nSALT:~$ ");
		char data[256];
		fgets(data, sizeof(data), stdin);
		char ** args = parse_args(data);
		if (!strcmp(args[0], "exit\n")) {
			printf("\nbacking out\n");
			exit(0);
		}
		else {
			child = fork();
			if (child == 0) {
				execvp(args[0], args);
				exit(0);
			}
			else {
				wait(NULL);
			}
		}
		/*
		if (!waiting) {
			printf("\nSALT:~$ ");
			waiting = 1;
		}
		else {
			char data[256];
			fgets(data, sizeof(data), stdin);
			//int nums = argcount(data);
			char ** args = parse_args(data);
			if (!strcmp(args[0], "cd")) {
				printf("supposed to backout");
			}
			if (!strcmp(args[0], "exit")) {
				printf("supposed to kill");
				execvp("kill", args);
			}
			child = fork();
			execvp(args[0], args);
			waiting = 0;
		}
		*/

		//i++;
	
	}

}
