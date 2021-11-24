#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
#include <fcntl.h>
#include <pwd.h>

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

char ** parse_semicolons(char * line) {
  char *temp;
  int counter;
  char **returnline = calloc(5, sizeof(char *));
  counter = 0;
  while ((temp = strsep(&line, ";"))) {
    returnline[counter] = temp;
    counter++;
  }
  return returnline;
}

int sizeof2d(char ** args) {
  int i = 0;
  while(args[i]) {
    i++;
  }
  return i;
}

int shell_ls() {
	struct dirent *data;
  struct stat space;
  DIR *d;
  unsigned int n;
  d = opendir("./");
  data = readdir(d);
  printf("\n");
  while(data) {
    printf("%s  ", data->d_name);
    data = readdir(d);
  }
  printf("\n");
  return 0;
}

char * shell_cd(char * args_list[], char * currentdir) {
	if (!args_list[1]) {
    strcpy(currentdir, "");
		chdir("");
    return currentdir;
	}
	else {
		/*
		if (!strcmp(args_list[1], "..")) {

		}
		*/
    printf("\ndoggy1\n");
    chdir(args_list[1]);
    printf("\ndoggy2\n");
    if (!errno) {
      printf("\ndoggy3\n");
      strcat("/", currentdir);
      strcat(currentdir, args_list[1]);
    }
    else {
      printf("\ndoggy5\n");
      printf("\n%d\n", errno);
    }
    printf("\ndoggy4\n");
    return currentdir;
	}
  // strcat("temp", currentdir);
  // chdir(args_list[1]);

  // return 0;
}

int main() {

	int ex = 0;
	//int waiting = 0;
	int child = 0;
	int i = 1;
	char direct[256];
  strcpy(direct, "");

  while (i) {
		printf("\nSALT: ~%s$ ", direct);
		char data[256];
		fgets(data, sizeof(data), stdin);

    //this bit works all the way...

    // char ** args = parse_args(data);
    // if (!strcmp(args[0], "exit\n")) {
    //   exit(0);
    // }
    // else if (!strcmp(args[0], "ls\n")) {
    //   shell_ls();
    // }
    // else if (!strcmp(args[0], "cd") || !strcmp(args[0], "cd\n")) {
    //   strcpy(direct, shell_cd(args, direct));
    // }
    // else {
    //   child = fork();
    //   if (child == 0) {
    //     execvp(args[0], args);
    //     exit(0);
    //   }
    //   else {
    //     wait(NULL);
    //   }
    // }

    //until about here

    //this is all testing code, to remove semicolons

    char ** semi_colon_args = parse_semicolons(data);
    int len = sizeof2d(semi_colon_args);
    int i;
    for (i = 0; i < len; i++) {
      char ** args = parse_args(semi_colon_args[i]);
      if (!strcmp(args[0], "exit\n")) {
  			exit(0);
  		}
  		else if (!strcmp(args[0], "ls\n")) {
  			shell_ls();
  		}
  		else if (!strcmp(args[0], "cd") || !strcmp(args[0], "cd\n")) {
  			strcpy(direct, shell_cd(args, direct));
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
    }

    // char ** semi_colon_args = parse_semicolons(data);

    // char ** args;
    // char ** semi_args = parse_semicolons(data);
    // // char ** args = parse_args(data);
    // // int i = 0;
    // // while (args[i]) {
    //   args = parse_args(semi_args[0]);
    //   if (!strcmp(args[0], "exit\n")) {
  	// 		exit(0);
  	// 	}
  	// 	else if (!strcmp(args[0], "ls\n")) {
  	// 		shell_ls();
  	// 	}
  	// 	/*
  	// 	else if (!strcmp(args[0], "cd\n")) {
  	// 		printf("\ngoing to base directory\n");
  	// 		direct[0] = 0;
  	// 		chdir("");
  	// 	}
  	// 	else if (!strcmp(args[0], "cd")) {
  	// 		printf("\ngoing to directory %s\n", args[1]);
  	// 		strcat(args[1], direct);
  	// 		chdir(args[1]);
  	// 	}
  	// 	*/
  	// 	else if (!strcmp(args[0], "cd") || !strcmp(args[0], "cd\n")) {
  	// 		strcpy(direct, shell_cd(args, direct));
  	// 	}
  	// 	else {
  	// 		child = fork();
  	// 		if (child == 0) {
  	// 			execvp(args[0], args);
  	// 			exit(0);
  	// 		}
  	// 		else {
  	// 			wait(NULL);
  	// 		}
  	// 	}
    // }

	}

}
