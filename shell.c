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

char ** parse_by_string(char * line, char * string) {
  char *temp;
  int counter;
  char **returnline = calloc(5, sizeof(char *));
  counter = 0;
  while ((temp = strsep(&line, string))) {
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
    printf("\n");
    data = readdir(d);
  }
  printf("\n");
  return 0;
}

char * move_back(char * line) {
  char * ret = malloc(sizeof(line));
  char * temp1;
  char ** temp2 = calloc(5, sizeof(char *));
  int i = 0;
  while ((temp1 = strsep(&line, "/"))) {
    temp2[i] = temp1;
    i++;
  }
  int s = sizeof2d(temp2);
  for (i = 0; i < s-1; i++) {
    strcat(ret, temp2[2]);
    strcat(ret, "/");
  }
  return ret;
}

char * shell_cd(char * args_list[], char * currentdir) {
	if (!args_list[1] || args_list[1] == "") {
    strcpy(currentdir, "");
    chdir("");
    return currentdir;
  }
  else if (!strcmp(args_list[1], "..")) {
    currentdir = move_back(args_list[1]);
    chdir("..");
    return currentdir;
  } else if(strchr(args_list[1], '~')) {
    strcpy(currentdir, args_list[1]);
    chdir(args_list[1]);
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
    if (!strcmp(args[i], ">") || !strcmp(args[i], "<") || !strcmp(args[i], ">>") || !strcmp(args[i], "|")) {
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
    else if (!strcmp(*temp, "|")) {
      char ** temppipe = malloc(sizeof(args));
      strcpy(temppipe[0], *(temp-1));
      strcpy(temppipe[1], ">");
      strcpy(temppipe[2], "pipefile");
      redirect(temppipe);

      strcpy(temppipe[0], *(temp+1));
      strcpy(temppipe[1], "<");
      strcpy(temppipe[2], "pipefile");
      redirect(temppipe);
    }
    else {
      temp++;
    }
  }
  fd[0] = -1;
  fd[1] = -1;
  return fd;
}

int ismath(char * line) {
  int i = 0;
  if (strchr(line, '+') || strchr(line, '-') || strchr(line, '*') || strchr(line, '/')) {
    i = 1;
  }
  return i;
}

int operationsnospace(char * line) {
  if (strchr(line, '+')) {
    char ** maths = parse_by_string(line, "+");
    int num = atoi(maths[0]);
    int i;
    for (i = 1; i < sizeof2d(maths); i++) {
      num += atoi(maths[i]);
    }
    return num;
  }
  else if (strchr(line, '-')) {
    char ** maths = parse_by_string(line, "-");
    int num = atoi(maths[0]);
    int i;
    for (i = 1; i < sizeof2d(maths); i++) {
      num -= atoi(maths[i]);
    }
    return num;
  }
  else if (strchr(line, '*')) {
    char ** maths = parse_by_string(line, "*");
    int num = atoi(maths[0]);
    int i;
    for (i = 1; i < sizeof2d(maths); i++) {
      num *= atoi(maths[i]);
    }
    return num;
  }
  else if (strchr(line, '/')) {
    char ** maths = parse_by_string(line, "/");
    int num = atoi(maths[0]);
    int i;
    for (i = 1; i < sizeof2d(maths); i++) {
      num /= atoi(maths[i]);
    }
    return num;
  }
  else {
    int num = atoi(line);
    return num;
  }
}

int multipleoperationdetect(char * line) {
  int count = 0;
  if(strchr(line, '+')) {
    count++;
  }
  if(strchr(line, '-')) {
    count++;
  }
  if(strchr(line, '*')) {
    count++;
  }
  if(strchr(line, '/')) {
    count++;
  }
  if(count > 1) {
    return 1;
  } else {
    return 0;
  }
}

int multipleoperations(char * line) {
  char ** math;
  int num = 0;
  if (multipleoperationdetect(line)) {
    if (strchr(line, '+') != NULL) {
      math = parse_by_string(line, "+");
      int i;
      num += multipleoperations(math[0]);
      for (i = 1; i < sizeof2d(math); i++) {
        num += multipleoperations(math[i]);
      }
    }
    else if (strchr(line, '-') != NULL) {
      math = parse_by_string(line, "-");
      int i;
      num += multipleoperations(math[0]);
      for (i = 1; i < sizeof2d(math); i++) {
        num -= multipleoperations(math[i]);
      }
    }
    else if (strchr(line, '*') != NULL) {
      math = parse_by_string(line, "*");
      int i;
      num += multipleoperations(math[0]);
      for (i = 1; i < sizeof2d(math); i++) {
        num *= multipleoperations(math[i]);
      }
    }
    else if (strchr(line, '/') != NULL) {
      math = parse_by_string(line, "/");
      int i;
      num += multipleoperations(math[0]);
      for (i = 1; i < sizeof2d(math); i++) {
        num /= multipleoperations(math[i]);
      }
    }
  }
  else {
    num = operationsnospace(line);
  }
  return num;
}

int main() {

	int child = 0;
	int i = 1;
	char direct[256];
  strcpy(direct, "");

  while (i) {
		printf("\n\033[0;31mS\033[0;34mA\033[0;35mL\033[0;32mT\033[0m:\033[0;36m~%s\033[0m$ ", direct);
		char data[256];
		fgets(data, sizeof(data), stdin);
    char ** semi_colon_args = parse_by_string(data, ";");
    int len = sizeof2d(semi_colon_args);
    int i;
    for (i = 0; i < len; i++) {
      strcpy(semi_colon_args[i], splice(semi_colon_args[i]));
      char ** args = parse_by_string(semi_colon_args[i], " ");
      if (!strcmp(args[0], "exit")) {
        exit(0);
      }
      else if (!strcmp(args[0], "ls")) {
        shell_ls();
      }
      else if (!strcmp(args[0], "cd")) {
        strcpy(direct, shell_cd(args, direct));
      }
      else if (ismath(args[0])) {
        printf("\n%d\n", multipleoperations(args[0]));
      }
      else if (indexofredirect(args) != -1) {
        int * thing = redirect(args);
        child = fork();
        if (!child) {
          execvp(args[0], args);
          exit(0);
        } else {
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
