#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include "util.h"

int listen_fd = 0;
int conn_fd = 0;
appconfig_t *conf = NULL;

void log_exit(char *fmt, ...) {
    if(conf)free(conf);
    close(listen_fd);
    close(conn_fd);
    
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fputc('\n', stderr);
    va_end(ap);
    exit(1);
}

void* xmalloc(size_t sz)
{
    void *p;

    p = malloc(sz);
    if (!p) log_exit("failed to allocate memory");
    return p;
}

void signal_exit(int sig)
{
   
   log_exit("exit by signal %d", sig);
}

void install_signal_handlers(void)
{
    trap_signal(SIGINT, signal_exit);
    trap_signal(SIGTERM, signal_exit);
}

void trap_signal(int sig, sighandler_t handler)
{
    struct sigaction act;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;
    if (sigaction(sig, &act, NULL) < 0){
        log_exit("sigaction() failed: %s", strerror(errno));
    }
}

char* currentDate(){
    time_t t;
    static char str[256];
    struct tm *utc;

    t = time(NULL);
    utc = gmtime(&t);
    
    snprintf(str, sizeof(str) - 1, "%04d/%02d/%02d %02d:%02d:%02d %d", utc->tm_year + 1900, utc->tm_mon + 1, utc->tm_mday, utc->tm_hour, utc->tm_min, utc->tm_sec, utc->tm_isdst);

    return str;
}

