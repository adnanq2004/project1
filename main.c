#include "shell.h"

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
