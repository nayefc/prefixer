#include <stdio.h>
#include <stdlib.h>
#include "stack.c"

#define OPEN_BRACKET "("
#define CLOSE_BRACKET ")"
#define ADDITION "+"
#define SUBTRACTION "-"
#define MULTIPLICATION "*"
#define DIVISION "/"

int main(int argc, char *argv[]) {

    // test stack
    test();

    /*    FILE *fp;
    fp = fopen(argv[1], "r");

    char reading;
    while (!feof(fp)) {

	fscanf(fp, "%c", &reading);
	
	// skip if whitspace
	if (reading == ' ')
	    continue;

	push(reading);

	if (count() == 3) {
	    printf("count is now 3 with top: %c", peek());
	}
	
    }
    */
    
    return 0;
}
