# Eric Guo and Adnan Quayyum

## Functions We Have
- semicolons
- cd
  - absolute directories are accounted for
  - **to test, run "cd [directory name]"**
    - a repo may or may not already be present, due to github issues.
- ls
  - ls prints new line for everything found, not as compact
  - **to test, run "ls"**
- pipes
- redirection
- mathematical operations
  - there must be no spaces between numbers and operands
  - operations with only 1 operand works fine
  - equations with more than 1 operand has a chance of returning wrong answer
  - division in general can be very stubborn because the functions used rely on ints
  - **to test, run "number1[operation]number2[operation]...[operation]numbern"**
    - ex: "1+2-3" or "3*4+2"
    - it is crucial that there are no spaces

## Features We Tried
- up arrow key to go through history of shell
  - We failed when we did research and found that it was very difficult to make C recognize the up arrow key signal properly, and as such we gave up on it

## Bugs
- As stated previously, our shell's mathematical capabilities are questionable when there are multiple operands involved
- it's possible to try and cd into a regular file, and have that displayed as part of the directory, but no change in your actual file directory

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
