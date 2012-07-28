#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

/*
 * run_child takes an expression as the first argument and a expected outcome expression as the second argument.
 * The expressions must match the format convention. All operators/operands are separated by a whitespace, except
 * for the first operator/operand which has no whitespace before it.
 * The README file defines the format of the output (the second argument).
 * Examples are provided here.
 */
void run_child(char expression[50], char prefixed[50]);

int main () {

    run_child("8", "8 \n");
    run_child("3 * x + ( 9 + y ) / 4", "( + ( * 3 x ) ( / ( + 9 y ) 4 ) ) \n");
    run_child("y", "y \n");
    run_child("10 * 5", "( * 10 5 ) \n");
    run_child("10 - 5 - 1", "( - 10 ( - 5 1 ) ) \n");
    run_child("10 - 5 * 1 ", "( - 10 ( * 5 1 ) ) \n");
    run_child("10 * 5 - 1", "( - ( * 10 5 ) 1 ) \n");
	      
    printf("\n---------------Automated testing passed---------------\n\n");
    return 0;
}

void run_child(char expression[50], char prefixed[50]) {
    
    pid_t child;
    int fd[2];
    int status;
    
    // set up a pipe for child and parent to communicate
    if (pipe(fd)) {
	fprintf(stderr, "Pipe Failed.\n");
	exit(1);
    }
    
    child = fork();
    
    // child process
    if (child == 0) {

	// duplicate child stdout into fd 
	if (dup2(fd[1], 1) < 0) {
	    fprintf(stderr, "Duplicating File Descriptors Error.\n");
	    exit(1);
	}
	
	// close loose ends
	close(fd[1]);
	close(fd[0]);

	FILE *In;
	In = fopen("file.txt", "w");
	if (In == NULL) {
	    fprintf(stderr, "Error opening file in test program.\n");
	    exit(1);
	}

	fprintf(In, "%s\n", expression);
	fclose(In);

	if (execl("./a.out", "./a.out", "file.txt", NULL) == -1 ) {
	    fprintf(stderr, "Error executing program");
	    exit(1);
	}
    }

    else if (child < 1) {
	fprintf(stderr, "Fork error.\n");
	exit(1);
    }

    waitpid(-1, &status, 0);

    FILE *fp;
    fp = fdopen(fd[0], "r");
    rewind(fp);
    char buffer[50];
    if (fgets(buffer, 50, fp) == NULL) {
	fprintf(stderr, "fgets error in test program. Error: %s.\n", strerror(errno));
	exit(1);
    }
    fclose(fp);
    //    printf("Output: '%s'", buffer);
    assert(strcmp(buffer, prefixed) == 0);
}
