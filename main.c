#include <stdio.h>
#include <string.h>

#include "cpu_usage.h"

#define USAGE_TEXT "Usage:\npet pid\n"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        perror("No pid provided.\n");
        return 1;
    }

    if (argc > 2) {
        perror("Too many arguments.\n");
        return 1;
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        printf(USAGE_TEXT);
        return 0;
    }

    char *pid = argv[1];

    if (sizeof(pid) > 8) {
        perror("Process ID too large!\n");
        return 1;
    }

    struct CPU_USAGE cpu_usage = cpu_usage_poll(pid);

    if (cpu_usage.c_usage < 0) {
        perror("Process with given pid does not exist.\n");
        return 1;
    }

    printf("CPU usage by process:%.2f\n", cpu_usage.usage);
    printf("CPU usage by inclusive of children:%.2f\n", cpu_usage.usage);

    return 0;
}