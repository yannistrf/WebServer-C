#include "common.h"

char* type[LOG_LEVEL_LEN] = {
    "DEBUG",
    "SUCCESS",
    "WARN",
    "ERR"
}; 

char* colors[LOG_LEVEL_LEN] = {
    "\x1B[0m",
    "\x1B[32m",
    "\x1B[1;33m",
    "\x1B[31m"
};

time_t current_time;
struct tm* m_time; 
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

void get_time() {
    time(&current_time);
    m_time = localtime(&current_time);
}

void log_msg(LOG_LEVEL level, const char* fmt, ...) {
    pthread_mutex_lock(&log_mutex);
    va_list args;
    va_start(args, fmt);
    get_time();
    printf("%s", colors[level]);
    printf("[%d/%d/%d-%d:%d:%d][%s] ", m_time->tm_mday,
                                          m_time->tm_mon,
                                          m_time->tm_year + 1900,
                                          m_time->tm_hour, 
                                          m_time->tm_min, 
                                          m_time->tm_sec,
                                          type[level]); 
    vfprintf(stdout, fmt, args);
    printf("\n%s", colors[LOG_DBG]); // reset color to normal
    fflush(stdout);
    va_end(args);
    pthread_mutex_unlock(&log_mutex);
}