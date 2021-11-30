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

char ** parse_by_string(char * line, char * string); //parsing as a string
int sizeof2d(char ** args); // finds size of a 2d array
int shell_ls(); // ls function of our shell
char * move_back(char * line); //allows us to move back directories
char * shell_cd(char * args_list[], char * currentdir); //cd function of our shell
char * splice(char * line); 
int indexofredirect(char ** args); //finds index of redirect
char ** redirect_helper(int filename, int std, int * fd, char ** args_list); //helper function for redirect
int * redirect(char ** args); //redirect function
int ismath(char * line); //checks if there is a math operation in the line
int operationsnospace(char * line); //simple one operand math
int multipleoperationdetect(char * line); //check if there are multiple operands
int multipleoperations(char * line); //math with multiple operands
