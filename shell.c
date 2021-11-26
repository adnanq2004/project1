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
	if (!args_list[1] || args_list[1] == "") {
    strcpy(currentdir, "");
    chdir("");
    return currentdir;
  }
  else {
    strcat(currentdir, "/");
    strcat(currentdir, args_list[1]);
    chdir(args_list[1]);
    return currentdir;
  }
}

char * splice(char * line) {
  char * temp = malloc(strlen(line));
  int i;
  int count = 0;
  for (i = 0; i < strlen(line); i++) {
    if (i == 0 && line[i] == ' ') {}
    else if (i == strlen(line)-1 && (line[i] == ' '  || line[i] == '\n')) {}
    else {
      temp[count] = line[i];
      count++;
    }
  }
  temp[count] = '\0';
  return temp;
}

int indexofredirect(char ** args) {
  int n = sizeof2d(args);
  int ret = -1;
  int i;
  for (i = 0; i < n; i++) {
    if (!strcmp(args[i], ">") || !strcmp(args[i], "<") || !strcmp(args[i], ">>")) {
      ret = i;
      break;
    }
  }
  return ret;
}

char ** redirect_helper(int filename, int std, int * fd, char ** args_list) {
  dup2(filename, std);
  *args_list = NULL;
  args_list[1] = NULL;
  args_list += 2;
  fd[0] = std;
  return args_list;
}

int * redirect(char ** args) {
  char ** temp = args;
  int * fd = malloc(2 * sizeof(int));
  while (*temp && temp) {
    if (!strcmp(*temp, ">")) {
      int stdout = dup(1);
      int file = open(*(temp + 1), O_CREAT | O_TRUNC | O_WRONLY, 0644);
      temp = redirect_helper(file,1,fd,temp);
      fd[1] = stdout;
      return fd;
    }
    else if (!strcmp(*temp, ">>")) {
      int stdout = dup(1);
      int file = open(*(temp + 1), O_CREAT | O_APPEND | O_WRONLY, 0644);
      temp = redirect_helper(file,1,fd,temp);
      fd[1] = stdout;
      return fd;
    }
    else if (!strcmp(*temp, "<")) {
      int stdin = dup(0);
      int file = open(*(temp + 1), O_RDONLY, 0644);
      temp = redirect_helper(file,0,fd,temp);
      fd[1] = stdin;
      return fd;
    }
    else {
      temp++;
    }
  }
  fd[0] = -1;
  fd[1] = -1;
  return fd;
}

// void shell_pipe(char* process1, char* process2) {
//   char temp[255];
//   FILE *input, *output;
//   input = popen(args1, "w");
//   output = popen(args2, "r");
//   while(fgets(temp, 255, input)) {
//     fputs(temp, output);
//   }
//   pclose(input);
//   pclose(output);
// }

int main() {

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
      strcpy(semi_colon_args[i], splice(semi_colon_args[i]));
      char ** args = parse_args(semi_colon_args[i]);
      if (!strcmp(args[0], "exit")) {
        exit(0);
      }
      else if (!strcmp(args[0], "ls")) {
        shell_ls();
      }
      else if (!strcmp(args[0], "cd")) {
        strcpy(direct, shell_cd(args, direct));
      }
      else if (indexofredirect(args) != -1) {
        int * thing = redirect(args);
        child = fork();
        if (!child) {
          execvp(args[0], args);
          exit(0);
        }
        // else if (!(strcmp(args[1], "||" )) {
        //   shell_pipe(args);
        // }
        else {
          if (thing[0] != -1) {
            dup2(thing[1], thing[0]);
          }
        }
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
}
