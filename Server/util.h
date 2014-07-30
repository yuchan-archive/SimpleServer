#ifndef UTIL_H
#define UTIL_H
typedef void (*sighandler_t)(int);

typedef struct {
    int port;
    char *mode;
} appconfig_t;

extern int listen_fd;
extern int conn_fd;
extern appconfig_t *conf;
extern void install_signal_handlers(void);
extern void trap_signal(int sig, sighandler_t handler);
extern char* currentDate();
extern void* xmalloc(size_t sz);
#endif
