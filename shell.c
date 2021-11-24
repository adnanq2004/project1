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

/*
static void sighandler(int sig) {
	if (sig == SIGINT) {
		exit(0);
	}
}
*/

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
  /*
  char sub[100];
  if(argc > 1) {
    d = opendir(argv[1]);
    if(errno) {
      printf("%s\n", strerror(errno));
      return -1;
    }
  } else {
    printf("Provide a Directory: ");
     scanf("%s",sub);
    d = opendir(sub);
    if(d < 0) {
      printf("Error %d: %s\n", errno, strerror(errno));
      return -1;
    }
  }
 */

  //printf("\ndoggy1\n");
  /*
  n = 0;
  printf("Statistics for directory: ./\n");

  data = readdir(d);
  while(data) {
    stat(data->d_name, &space);
    n += space.st_size;
    data = readdir(d);
  }
  printf("\nTotal Directory Size: %d\n", n);

  rewinddir(d);
  */
  // data = readdir(d);
  //
  // //printf("\ndoggy2\n");
  // printf("\nDirectories: \n");
  // while(data) {
  //   stat(data->d_name, &space);
  //   if(data->d_type == DT_DIR) {
  //     printf("%s \n", data->d_name);
  //   }
  //   data = readdir(d);
  // }
  //
  // rewinddir(d);
  // data = readdir(d);
  //
  // //printf("\ndoggy3\n");
  // printf("\nRegular Files: \n");
  // while(data) {
  //   stat(data->d_name, &space);
  //   if(data->d_type != DT_DIR) {
  //     printf("%s \n", data->d_name);
  //   }
  //   data = readdir(d);
  // }

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
		char ** args = parse_args(data);
		if (!strcmp(args[0], "exit\n")) {
			exit(0);
		}
		else if (!strcmp(args[0], "ls\n")) {
			shell_ls();
		}
		/*
		else if (!strcmp(args[0], "cd\n")) {
			printf("\ngoing to base directory\n");
			direct[0] = 0;
			chdir("");
		}
		else if (!strcmp(args[0], "cd")) {
			printf("\ngoing to directory %s\n", args[1]);
			strcat(args[1], direct);
			chdir(args[1]);
		}
		*/
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

}
