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

int main() {

	int ex = 0;
	int waiting = 1;
	while (!ex) {
	
		if (!waiting) {
			printf("\nSALT:~$ ");
			waiting = 0;
		}
		else {
			
		}
	
	}

}
