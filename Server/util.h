#ifndef UTIL_H
#define UTIL_H
typedef void (*sighandler_t)(int);
extern int listen_fd;
extern void install_signal_handlers(void);
extern void trap_signal(int sig, sighandler_t handler);
extern char* currentDate();
extern void* xmalloc(size_t sz);
#endif
