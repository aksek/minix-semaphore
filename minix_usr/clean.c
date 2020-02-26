#include <stdio.h>
#include <limits.h>
#include <minix/semaphore.h>

int main(void) {
    int i; 
    FILE *fp;
    char *filename = "store.txt";
    int cur_stock;
    fp = fopen(filename, "w+");
    if (fp == NULL) puts("file failed to open");
    else {
        fprintf(fp, "%d\n", 0);
    }
    fclose(fp);
    for (i = 0; i < 3; ++i)
        sem_init(i, INT_MAX);
}
