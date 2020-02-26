#ifndef COMMON_FUNCS_H
#define COMMON_FUNCS_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int uniform(int a, int b) {
    return rand() % (b - a) + a;
}

int get_cur_stock(void) {
    int res;
    FILE *fp;
    char *filename = "store.txt";
    int cur_stock;
    fp = fopen(filename, "r");
    if (fp == NULL) puts("file failed to open");
    else {
        res = fscanf(fp, "%d", &cur_stock);
    }
    fclose(fp);
    return cur_stock;
}

void insert(int new_stock) {
    FILE *fp;
    char *filename = "store.txt";
    int cur_stock;
    fp = fopen(filename, "w+");
    if (fp == NULL) puts("file failed to open");
    else {
        fprintf(fp, "%d \n", new_stock);
    }
    fclose(fp);
} 
#endif
