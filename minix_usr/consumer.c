#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <minix/minlib.h>
#include <minix/semaphore.h>

#include "common_funcs.h"

void log(char *mess, int n_elements, int cons_nr) {
    FILE *fp;
    char filename[] = "cons";
    char *cons_nr_string = itoa(cons_nr);
    strcat(filename, cons_nr_string);
    strcat(filename, ".txt");
    fp = fopen(filename, "a");
    if (fp == NULL) puts("file failed to open");
    else {
        fprintf(fp, mess);
        fprintf(fp, "%d \n", n_elements);
    }
    fclose(fp);
}

int main(int argc, char *argv[]) {
    int current_stock;

    int k;
    int cons_nr;
    int c;
    int d;

    int to_consume;
    char *mess1 = "Number of elements to be consumed: ";
    char *mess2 = "Number of elements consumed successfuly: ";

    k = atoi(argv[1]);
    cons_nr = atoi(argv[2]);
    c = atoi(argv[3]);
    d = atoi(argv[4]);


    if (argc != 5) return 1;

    srand(time(0));

    while(1) {
        sleep(1);
        to_consume = uniform(c, d);
        log(mess1, to_consume, cons_nr);
        sem_init(0, 1);
        sem_init(1, 0);
        sem_init(2, 0);
        sem_down(0);
        current_stock = get_cur_stock();
        while (current_stock < to_consume) {
            sem_up(0);
            sem_down(1);
        }
        current_stock -= to_consume;
        insert(current_stock);
        log(mess2, to_consume, cons_nr);
        if (current_stock > k / 2) sem_up(2);
        else if (current_stock < k / 2) sem_up(1);
        else sem_up(0);
    }
    return 0;
}
