#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cpu_usage.h"
#include "file_io.h"

struct CPU_TIME cpu_time(FILE *uptime_file, FILE *stat_file) {
    struct CPU_TIME c_t;

    fseek(uptime_file, 0, SEEK_SET);
    fseek(stat_file, 0, SEEK_SET);

    fscanf(uptime_file, "%ld %ld", &(c_t.uptime), &(c_t.idle));
    fscanf(stat_file, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %lu %lu %lu %lu %*s %*s %*s %*s %llu", 
        &(c_t.utime), &(c_t.stime), &(c_t.cutime), &(c_t.cstime), &(c_t.starttime));
    
    return c_t;
}

struct CPU_USAGE cpu_usage_poll(char *pid) {
    FILE *uptime_file;
    FILE *stat_file;
    char stat_file_path[sizeof(STAT_FILE_PATH) + 10];

    struct CPU_USAGE r_val = {-1, -1};
    
    sprintf(stat_file_path, STAT_FILE_PATH, pid);

    // Poll for first values
    uptime_file = wrap_fopen(UPTIME_FILE_PATH, "r");
    stat_file = wrap_fopen(stat_file_path, "r");

    if (!uptime_file) {
        return r_val;
    }

    if (!stat_file) {
        return r_val;
    }
    
    struct CPU_TIME c_t_1 = cpu_time(uptime_file, stat_file);

    fclose(stat_file);
    fclose(uptime_file);
    
    usleep(POLL_INTERVAL);

    // poll for second values
    uptime_file = wrap_fopen(UPTIME_FILE_PATH, "r");
    stat_file = wrap_fopen(stat_file_path, "r");

    if (!uptime_file) {
        return r_val;
    }

    if (!stat_file) {
        return r_val;
    }

    struct CPU_TIME c_t_2 = cpu_time(uptime_file, stat_file);

    fclose(stat_file);
    fclose(uptime_file);

    // calculate cpu usage
    long hertz = sysconf(_SC_CLK_TCK);

    unsigned long total_time_1 = c_t_1.utime + c_t_1.stime;
    unsigned long total_time_c_1 = c_t_1.utime + c_t_1.stime + c_t_1.cutime + c_t_1.cstime; // inclusive of child processes
    unsigned long total_time_2 = c_t_2.utime + c_t_2.stime;
    unsigned long total_time_c_2 = c_t_2.utime + c_t_2.stime + c_t_2.cutime + c_t_2.cstime; // inclusive of child processes

    unsigned long diff = total_time_2 - total_time_1;
    unsigned long diff_c = total_time_c_2 - total_time_c_1;
    
    r_val.usage = 100.0 / (double) hertz * (double) diff / ((double) POLL_INTERVAL / 1000000.0);
    r_val.c_usage = 100.0 / (double) hertz * (double) diff_c / ((double) POLL_INTERVAL / 1000000.0);

    return r_val;
}


