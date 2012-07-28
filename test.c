#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main () {

    FILE *fp;
    fp = fopen("file.txt", "w");
    if (fp == NULL) {
	fprintf(stderr, "Unable to create a new file");
	exit(1);
    }

    fprintf(fp, "8 + 5 / 2\n");
    int fid = fileno(fp);
    fclose(fp);

    FILE *fpOut;
    

    dup2(1, fid);

    if (execl("./a.out", "./a.out", "file.txt", NULL) == -1) {
	fprintf(stderr, "Error forking program\n");
	exit(1);
    }

    return 0;
}
