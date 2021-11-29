# Eric Guo and Adnan Quayyum

## Functions We Have
- semicolons
- cd
  - absolute directories are accounted for
- ls
  - ls prints new line for everything found, not as compact
- pipes
- redirection
- mathematical operations
  - there must be no spaces between numbers and operands
  - operations with only 1 operand works fine
  - equations with more than 1 operand has a chance of returning wrong answer

## Features We Tried
- up arrow key to go through history of shell
  - We failed when we did research and found that it was very difficult to make C recognize the up arrow key signal properly, and as such we gave up on it

## Bugs
- As stated previously, our shell's mathematical capabilities are questionable when there are multiple operands involved

## Function Headers
- char ** parse_by_string(char * line, char * string);
- int sizeof2d(char ** args);
- int shell_ls();
- char * move_back(char * line);
- char * shell_cd(char * args_list[], char * currentdir);
- char * splice(char * line);
- int indexofredirect(char ** args);
- char ** redirect_helper(int filename, int std, int * fd, char ** args_list);
- int * redirect(char ** args);
- int ismath(char * line);
- int operationsnospace(char * line);
- int multipleoperationdetect(char * line);
- int multipleoperations(char * line);
- int main(); 
