
#include "mm.h"
#include <lib.h>
#include <minix/callnr.h>
#include <minix/com.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "mproc.h"
#include "param.h"
#include <limits.h>
#include <stdlib.h>

typedef struct process {
    pid_t id;
    struct process *prev;
    struct process *next;
} process;

void enqueue(process **ptail, process *p) {
    process *tail = *ptail;
    if (tail == NULL) {
        *ptail = p;
        p->prev = NULL;
        p->next = NULL;
    } else {
        tail->next = p;
        p->prev = tail;
        p->next = NULL;
        *ptail = tail;
    }
}

process *dequeue(process **phead) {
    process *head = *phead;
    if (head == NULL) return NULL;
    *phead = head->next;
    if (head->next != NULL) head->next->prev = NULL;
    head->next = NULL;
    return head;
}

typedef struct semaphore {
    int value;
    process *qHead;
    process *qTail;
    int qSize;
} semaphore;

semaphore sems[3];

int do_sem_down() {
    int sem_num = mm_in.m1_i2;
    semaphore *sem = sems + sem_num;
    sem->value--;
    if (sem->value < 0) {
        process *p = (process *)alloc_mem(sizeof(process));
        p->id = who;
        p->next = NULL;
        p->prev = NULL;
        enqueue(&sem->qTail, p);
        sem->qSize++;
        return E_NO_MESSAGE;
    }
    return 0;
}

int do_sem_up() {
    int proc_nr;
    int sem_num = mm_in.m1_i2;
    semaphore *sem = sems + sem_num;
    if (!sem) return EGENERIC;
    sem->value++;
    if (sem->value <= 0) {
        process *p = dequeue(&sem->qHead);
        sem->qSize--;
        proc_nr = p->id;
        free(p);
        setreply(proc_nr, OK);
    }
    return 0;
}

int do_sem_init() {
    int sem_num = mm_in.m1_i2;
    int value = mm_in.m1_i3;
    semaphore *sem = sems + sem_num;
    process *tail;
    if (!sem) return EGENERIC;
    if (sem->value != INT_MAX && value != INT_MAX) {
        sem->value = value;
        sem->qHead = NULL;
        sem->qTail = NULL;
        sem->qSize = 0;
    } else if (value == INT_MAX) {
        while (sem->qHead != NULL) {
            tail = sem->qTail;
            sem->qTail = sem->qTail->prev;
            free(sem->qTail);
        }
    }
    return 0;
}

int do_sem_status() {
    int sem_num = mm_in.m1_i2;
    semaphore *sem = sems + sem_num;
    if (!sem) return EGENERIC;
    mm_in.m1_i3 = sem->value;
    mm_in.m1_i1 = sem->qSize;
    return 0;
}