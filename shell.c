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
}

int main() {

	int ex = 0;
	int child = 0;
	int i = 1;
	char direct[256];
  strcpy(direct, "");

  while (i) {
		printf("\nSALT: ~%s$ ", direct);
		char data[256];
		fgets(data, sizeof(data), stdin);

    char ** semi_colon_args = parse_semicolons(data);
    int len = sizeof2d(semi_colon_args);
    int i;
    for (i = 0; i < len; i++) {
      char ** args = parse_args(semi_colon_args[i]);
      int r = sizeof2d(args);
      int s = 0;
      while (!strcmp(args[s], "") && s < r) {
        s++;
      }
      if (s >= r) {
        return 1;
      }
      char ** tempargs = malloc(sizeof(args));
      int counter;
      for(counter = s; counter < r; counter++) {
        strcpy(tempargs[counter - s], args[counter]);
      }
      if (!strcmp(tempargs[0], "exit\n")) {
        exit(0);
      }
      else if (!strcmp(tempargs[0], "ls\n")) {
        shell_ls();
      }
      else if (!strcmp(tempargs[0], "cd") || !strcmp(tempargs[0], "cd\n")) {
        strcpy(direct, shell_cd(args, direct));
      }
      else {
        child = fork();
        if (child == 0) {
          execvp(tempargs[0], temparg);
          exit(0);
        }
        else {
          wait(NULL);
        }
      }
	  }
  }
}
