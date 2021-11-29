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

char ** parse_by_string(char * line, char * string);
int sizeof2d(char ** args);
int shell_ls();
char * move_back(char * line);
char * shell_cd(char * args_list[], char * currentdir);
char * splice(char * line);
int indexofredirect(char ** args);
char ** redirect_helper(int filename, int std, int * fd, char ** args_list);
int * redirect(char ** args);
int ismath(char * line);
int operationsnospace(char * line);
int multipleoperationdetect(char * line);
int multipleoperations(char * line);
