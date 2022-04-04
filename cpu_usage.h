#include <stdio.h>

#define UPTIME_FILE_PATH "/proc/uptime"
#define STAT_FILE_PATH "/proc/%s/stat"

#define POLL_INTERVAL 2500000 // In microseconds

struct CPU_USAGE {
    double usage; // CPU usage by process only
    double c_usage; // CPU usage by process and child processes
    double seconds; // Time since process started (in seconds)
};

struct CPU_TIME {
    unsigned long utime; // (14)
    unsigned long stime; // (15)

    // child processes
    unsigned long cutime; // (16)
    unsigned long cstime; // (17)

    unsigned long long starttime; // (22)

    long uptime;
    long idle;
};

struct CPU_TIME cpu_time(FILE *uptime_file, FILE *stat_file);
struct CPU_USAGE cpu_usage_poll(char *pid);
