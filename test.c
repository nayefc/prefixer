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
 * To reduce, pass in 1 as the reduce argument. Otherwise, pass 0.
 */
void run_child(char expression[50], char prefixed[50], int reduce);

int main () {
    
    run_child("8", "8 \n", 0);
    run_child("3 * x + ( 9 + y ) / 4", "( + ( * 3 x ) ( / ( + 9 y ) 4 ) ) \n", 0);
    run_child("y", "y \n", 0);
    run_child("10 * 5", "( * 10 5 ) \n", 0);
    run_child("10 - 5 - 1", "( - 10 ( - 5 1 ) ) \n", 0);
    run_child("10 - 5 * 1 ", "( - 10 ( * 5 1 ) ) \n", 0);
    run_child("10 * 5 - 1", "( - ( * 10 5 ) 1 ) \n", 0);
    run_child("15 / ( 3 * 1 )", "( / 15 ( * 3 1 ) ) \n", 0);
    run_child("12 - a * ( 45 * 2 ) / 1", "( - 12 ( * a ( / ( * 45 2 ) 1 ) ) ) \n", 0);
    run_child("1 / ( 2 * 2 ) + ( 12 + ( x + 0 ) )", "( + ( / 1 ( * 2 2 ) ) ( + 12 ( + x 0 ) ) ) \n", 0);

    // Reductions

    run_child("1", "1 \n", 1);
    run_child("5 + 1", "6 \n", 1);
    run_child("5 + 1 * 8", "13 \n", 1);
    run_child("3 + 5 * 4", "23 \n", 1);
    run_child("5 - 5", "0 \n", 1);
    run_child("3 + 5 * 25 - 2 / 2", "127 \n", 1);
    run_child("49 / ( 4 * 1)", "12 \n", 1);

    run_child("a * 1", "a \n", 1);
    run_child("1 * b", "b \n", 1);
    run_child("c * 0", "0 \n", 1);
    run_child("0 * d", "0 \n", 1);
    run_child("0 / e", "0 \n", 1);
    run_child("f / 1", "f \n", 1);
    run_child("x / x", "1 \n", 1);
    run_child("y - y", "0 \n", 1);
    run_child("z - 0", "z \n", 1);

    run_child("( 5 - b ) - ( 5 - b )", "0 \n", 1);
    run_child("( a - a ) - ( a - a )", "0 \n", 1);
    run_child("( y / 1 ) / ( y / 1 )", "1 \n", 1);
    run_child("( x / 5 ) / 1", "( / x 5 ) \n", 1);
    run_child("( y * 1 ) * 1 ", "y \n", 1);
    run_child("1 * ( x * 1 )", "x \n", 1);
    run_child("( y - 1 ) * 0 ", "0 \n", 1);
    run_child("0 * (y - 1 )", "0 \n", 1);
    run_child("( a * b - 4 / 1 ) - 0", "( - ( * a b ) 4 ) \n", 1);
         
    printf("\n---------------Automated testing passed---------------\n\n");
    return 0;
}

void run_child(char expression[50], char prefixed[50], int reduce) {
    
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

	if (reduce == 0) {
	    if (execl("prefixer", "prefixer", "file.txt", NULL) == -1 ) {
		fprintf(stderr, "Error executing program\n");
		exit(1);
	    }
	}

	else if (reduce == 1) {
	    if (execl("prefixer", "prefixer", "file.txt", "-r", NULL) == -1 ) {
		fprintf(stderr, "Error executing program\n");
		exit(1);
	    }
	}
    }

    else if (child < 1) {
	fprintf(stderr, "Fork error.\n");
	exit(1);
    }

    // wait for child process to return
    waitpid(-1, &status, 0);

    // read forked process output from pipe
    FILE *fp;
    fp = fdopen(fd[0], "r");
    rewind(fp);
    char buffer[50];
    if (fgets(buffer, 50, fp) == NULL) {
	fprintf(stderr, "fgets error in test program. Error: %s.\n", strerror(errno));
	exit(1);
    }

    // printf("Buffer: %s\n", buffer);

    fclose(fp);
    assert(strcmp(buffer, prefixed) == 0);
}
