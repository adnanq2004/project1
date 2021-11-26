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


// int writetofile(char * filename) {
//   char statement[256];
//   read(STDIN_FILENO, statement, sizeof(statement));
//   int filetowrite = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//   write(filetowrite, statement, sizeof(statement));
//
// }
//
// int redirectout(char ** args, char * filename) {
//   // int s = sizeof2d(args);
//   // int n = indexofredirectout(args);
//   // // printf("%d\n", n);
//
//   // int child = fork();
//   // if (child == 0) {
//   //   char temp[4096];
//   //   execvp(args[0], args);
//   //
//   //   // int temp = dup(STDIN_FILENO);
//   //   // dup2(child, STDIN_FILENO);
//   //   // execvp(args[0], args);
//   //   // dup2(temp, STDIN_FILENO);
//   //   // return 0;
//   // }
//   // else {
//   //   return 0;
//   // }
//   // // writetofile(filename);
//
//   // char ** tempargs = malloc((s - n) * 64);
//   // int t = s-n;
//   // int i;
//   // for (i = 0; i < t; i++) {
//   //   printf("\npidgeon04\n");
//   //   strcpy(tempargs[i], args[i]);
//   //   printf("\npidgeon1\n");
//   // }
//   //
//
//   // int filedes[2];
//   // // pipe(filedes, 0);
//   // int child = fork();
//   // if (child == 0) {
//   //   while ((dup2(filedes[1], STDOUT_FILENO) == -1) && (errno == EINTR)) {}
//   //   close(filedes[1]);
//   //   close(filedes[0]);
//   //   execvp(args[0], args);
//   //   exit(1);
//   // }
//   // else {
//   //   wait(NULL);
//   //   close(filedes[1]);
//   // }
//   // char output[4096];
//   // // printf("\n%s\n", filedes[0]);
//   // read(filedes[0], output, sizeof(output));
//   // close(filedes[0]);
//   // // printf("\n%s\n", output);
//   // // return 1;
//   // //
//   // int filetowrite = open(filename, O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0644);
//   // write(filetowrite, output, sizeof(output));
//
//   int fd[2];
//   int child = fork();
//   if (!child) {
//     execvp(args[0], args[]);
//   }
//
// }

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
//
// int indexofredirectcatout(char ** args) {
//   int n = sizeof2d(args);
//   int ret = -1;
//   int i;
//   for (i = 0; i < n; i++) {
//     if (!strcmp(args[i], ">>")) {
//       ret = i;
//       break;
//     }
//   }
//   return ret;
// }
//
// int indexofredirectin(char ** args) {
//   int n = sizeof2d(args);
//   int ret = -1;
//   int i;
//   for (i = 0; i < n; i++) {
//     if (!strcmp(args[i], "<")) {
//       ret = i;
//       break;
//     }
//   }
//   return ret;
// }

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
      int file = open(*(temp + 1), O_CREAT | O_TRUNC | O_APPEND | O_WRONLY, 0644);
      temp = redirect_helper(file,1,fd,temp);
      fd[1] = stdout;
      return fd;
    }
    else if (!strcmp(*temp, ">")) {
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
