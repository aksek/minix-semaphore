#include<lib.h>
#include<unistd.h>

int sem_down(int sem_num) {
    message m;
    m.m1_i2 = sem_num;
    _syscall(MM, SEM_DOWN, &m);
}

int sem_up(int sem_num) {
    message m;
    m.m1_i2 = sem_num;
    _syscall(MM, SEM_UP, &m);
}

int sem_init(int sem_num, int value) {
    message m;
    m.m1_i2 = sem_num;
    m.m1_i3 = value;
    _syscall(MM, SEM_INIT, &m);
}

int sem_status(int sem_num, int *value, int *num_blocked) {
    message m;
    m.m1_i2 = sem_num;
    _syscall(MM, SEM_STATUS, &m);
    *value = m.m1_i3;
    *num_blocked = m.m1_i1;
}