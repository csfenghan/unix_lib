/* Updated 9/2/2020 :
 *
 */

#include "unix_api.h"
#include <limits.h>
#include <stdarg.h>

/***************************************************************
 *	系统函数封装
 **************************************************************/

// 1.进程控制
pid_t Fork(void) {
        pid_t pid;

        if ((pid = fork()) < 0)
                unix_error("Fork error");
        return pid;
}

void Execve(const char *filename, char *const argv[], char *const envp[]) {
        if (execve(filename, argv, envp) < 0)
                unix_error("Execve error");
}

pid_t Wait(int *status) {
        pid_t pid;

        if ((pid = wait(status)) < 0)
                unix_error("Wait error");
        return pid;
}

pid_t Waitpid(pid_t pid, int *iptr, int options) {
        pid_t retpid;

        if ((retpid = waitpid(pid, iptr, options)) < 0)
                unix_error("Waitpid error");
        return (retpid);
}

void Kill(pid_t pid, int signum) {
        int rc;

        if ((rc = kill(pid, signum)) < 0)
                unix_error("Kill error");
}

void Pause() {
        (void)pause();
        return;
}

unsigned int Sleep(unsigned int secs) {
        unsigned int rc;

        if ((rc = sleep(secs)) < 0)
                unix_error("Sleep error");
        return rc;
}

unsigned int Alarm(unsigned int seconds) { return alarm(seconds); }

void Setpgid(pid_t pid, pid_t pgid) {
        int rc;

        if ((rc = setpgid(pid, pgid)) < 0)
                unix_error("Setpgid error");
        return;
}

pid_t Getpgrp(void) { return getpgrp(); }

// 2.基本IO
int Open(const char *pathname, int flags, mode_t mode) {
        int rc;

        if ((rc = open(pathname, flags, mode)) < 0)
                unix_error("Open error");
        return rc;
}

ssize_t Read(int fd, void *buf, size_t count) {
        ssize_t rc;

        if ((rc = read(fd, buf, count)) < 0)
                unix_error("Read error");
        return rc;
}

ssize_t Write(int fd, const void *buf, size_t count) {
        ssize_t rc;

        if ((rc = write(fd, buf, count)) < 0)
                unix_error("Write error");
        return rc;
}

off_t Lseek(int fildes, off_t offset, int whence) {
        off_t rc;

        if ((rc = lseek(fildes, offset, whence)) < 0)
                unix_error("Lseek error");
        return rc;
}

void Close(int fd) {
        int rc;

        if ((rc = close(fd)) < 0)
                unix_error("Close error");
}

int Select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) {
        int rc;

        if ((rc = select(n, readfds, writefds, exceptfds, timeout)) < 0)
                unix_error("Select error");
        return rc;
}

int Dup2(int fd1, int fd2) {
        int rc;

        if ((rc = dup2(fd1, fd2)) < 0)
                unix_error("Dup2 error");
        return rc;
}

void Stat(const char *filename, struct stat *buf) {
        if (stat(filename, buf) < 0)
                unix_error("Stat error");
}

void Fstat(int fd, struct stat *buf) {
        if (fstat(fd, buf) < 0)
                unix_error("Fstat error");
}

DIR *Opendir(const char *name) {
        DIR *dp;

        if ((dp = opendir(name)) == NULL)
                unix_error("Opendir error");
        return dp;
}

// 3.标准IO
void Fclose(FILE *fp) {
        if (fclose(fp) != 0)
                unix_error("Fclose error");
}

FILE *Fdopen(int fd, const char *type) {
        FILE *fp;

        if ((fp = fdopen(fd, type)) == NULL)
                unix_error("Fdopen error");

        return fp;
}

char *Fgets(char *ptr, int n, FILE *stream) {
        char *rptr;

        if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream))
                app_error("Fgets error");

        return rptr;
}

FILE *Fopen(const char *filename, const char *mode) {
        FILE *fp;

        if ((fp = fopen(filename, mode)) == NULL)
                unix_error("Fopen error");

        return fp;
}

void Fputs(const char *ptr, FILE *stream) {
        if (fputs(ptr, stream) == EOF)
                unix_error("Fputs error");
}

size_t Fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
        size_t n;

        if (((n = fread(ptr, size, nmemb, stream)) < nmemb) && ferror(stream))
                unix_error("Fread error");
        return n;
}

void Fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
        if (fwrite(ptr, size, nmemb, stream) < nmemb)
                unix_error("Fwrite error");
}

// 4. 动态内存管理
void *Malloc(size_t size) {
        void *p;

        if ((p = malloc(size)) == NULL)
                unix_error("Malloc error");
        return p;
}

void *Realloc(void *ptr, size_t size) {
        void *p;

        if ((p = realloc(ptr, size)) == NULL)
                unix_error("Realloc error");
        return p;
}

void *Calloc(size_t nmemb, size_t size) {
        void *p;

        if ((p = calloc(nmemb, size)) == NULL)
                unix_error("Calloc error");
        return p;
}

void Free(void *ptr) { free(ptr); }

void *Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset) {
        void *ptr;

        if ((ptr = mmap(addr, len, prot, flags, fd, offset)) == ((void *)-1))
                unix_error("mmap error");
        return (ptr);
}

void Munmap(void *start, size_t length) {
        if (munmap(start, length) < 0)
                unix_error("munmap error");
}

// 5.信号
handler_t *Signal(int signum, handler_t *handler) {
        struct sigaction action, old_action;

        action.sa_handler = handler;
        sigemptyset(&action.sa_mask); /* Block sigs of type being handled */
        action.sa_flags = SA_RESTART; /* Restart syscalls if possible */

        if (sigaction(signum, &action, &old_action) < 0)
                unix_error("Signal error");
        return (old_action.sa_handler);
}

void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset) {
        if (sigprocmask(how, set, oldset) < 0)
                unix_error("Sigprocmask error");
        return;
}

void Sigemptyset(sigset_t *set) {
        if (sigemptyset(set) < 0)
                unix_error("Sigemptyset error");
        return;
}

void Sigfillset(sigset_t *set) {
        if (sigfillset(set) < 0)
                unix_error("Sigfillset error");
        return;
}

void Sigaddset(sigset_t *set, int signum) {
        if (sigaddset(set, signum) < 0)
                unix_error("Sigaddset error");
        return;
}

void Sigdelset(sigset_t *set, int signum) {
        if (sigdelset(set, signum) < 0)
                unix_error("Sigdelset error");
        return;
}

int Sigismember(const sigset_t *set, int signum) {
        int rc;
        if ((rc = sigismember(set, signum)) < 0)
                unix_error("Sigismember error");
        return rc;
}

// 6.线程控制
void Pthread_create(pthread_t *tidp, pthread_attr_t *attrp, void *(*routine)(void *), void *argp) {
        int rc;

        if ((rc = pthread_create(tidp, attrp, routine, argp)) != 0)
                posix_error(rc, "Pthread_create error");
}

void Pthread_cancel(pthread_t tid) {
        int rc;

        if ((rc = pthread_cancel(tid)) != 0)
                posix_error(rc, "Pthread_cancel error");
}

void Pthread_join(pthread_t tid, void **thread_return) {
        int rc;

        if ((rc = pthread_join(tid, thread_return)) != 0)
                posix_error(rc, "Pthread_join error");
}

void Pthread_detach(pthread_t tid) {
        int rc;

        if ((rc = pthread_detach(tid)) != 0)
                posix_error(rc, "Pthread_detach error");
}

void Pthread_exit(void *retval) { pthread_exit(retval); }

pthread_t Pthread_self(void) { return pthread_self(); }

void Pthread_once(pthread_once_t *once_control, void (*init_function)()) {
        pthread_once(once_control, init_function);
}

// 7.进程间通信
void Sem_init(sem_t *sem, int pshared, unsigned int value) {
        if (sem_init(sem, pshared, value) < 0)
                unix_error("Sem_init error");
}

int Pipe(int pipefd[2]) {
        int result;
        if ((result = pipe(pipefd)) < 0)
                unix_error("Pipe error");
        return result;
}

key_t Ftok(const char *path, int id) {
        key_t key;
        if ((key = ftok(path, id)) < 0)
                unix_error("Ftok error");
        return key;
}
int Shmget(key_t key, size_t size, int flag) {
        int shmid;
        if ((shmid = shmget(key, size, flag)) < 0)
                unix_error("Shmget error");
        return shmid;
}
int Shmctl(int shmid, int cmd, struct shmid_ds *buf) {
        int result;

        if ((result = shmctl(shmid, cmd, buf)) < 0)
                unix_error("Shmctl error");
        return result;
}
void *Shmat(int shmid, const void *addr, int flag) {
        void *result;

        if ((result = shmat(shmid, addr, flag)) == (void *)-1)
                unix_error("Shmat error");
        return result;
}
int Shmdt(const void *addr) {
        int result;

        if ((result = shmdt(addr)) < 0)
                unix_error("shmdt error");
        return result;
}

// 8.socket接口封装
int Socket(int domain, int type, int protocol) {
        int rc;

        if ((rc = socket(domain, type, protocol)) < 0)
                unix_error("Socket error");
        return rc;
}

void Setsockopt(int s, int level, int optname, const void *optval, int optlen) {
        int rc;

        if ((rc = setsockopt(s, level, optname, optval, optlen)) < 0)
                unix_error("Setsockopt error");
}

void Bind(int sockfd, struct sockaddr *my_addr, int addrlen) {
        int rc;

        if ((rc = bind(sockfd, my_addr, addrlen)) < 0)
                unix_error("Bind error");
}

void Listen(int s, int backlog) {
        int rc;

        if ((rc = listen(s, backlog)) < 0)
                unix_error("Listen error");
}

int Accept(int s, struct sockaddr *addr, socklen_t *addrlen) {
        int rc;

        if ((rc = accept(s, addr, addrlen)) < 0)
                unix_error("Accept error");
        return rc;
}

void Connect(int sockfd, struct sockaddr *serv_addr, int addrlen) {
        int rc;

        if ((rc = connect(sockfd, serv_addr, addrlen)) < 0)
                unix_error("Connect error");
}

struct hostent *Gethostbyname(const char *name) {
        struct hostent *p;

        if ((p = gethostbyname(name)) == NULL)
                dns_error("Gethostbyname error");
        return p;
}

struct hostent *Gethostbyaddr(const char *addr, int len, int type) {
        struct hostent *p;

        if ((p = gethostbyaddr(addr, len, type)) == NULL)
                dns_error("Gethostbyaddr error");
        return p;
}

int Getaddrinfo(const char *hostname, const char *service, const struct addrinfo *hints,
                struct addrinfo **result) {
        int n;

        if ((n = getaddrinfo(hostname, service, hints, result)) != 0)
                err_quit("tcp_listen error for %s, %s: %s", hostname, service, gai_strerror(n));
        return n;
}
/***************************************************************
 *	辅助函数包
 **************************************************************/
// 1.错误处理(来自csapp)
void unix_error(char *msg) /* Unix-style error */
{
        fprintf(stderr, "%s: %s\n", msg, strerror(errno));
        exit(0);
}

void posix_error(int code, char *msg) /* Posix-style error */
{
        fprintf(stderr, "%s: %s\n", msg, strerror(code));
        exit(0);
}

void dns_error(char *msg) /* DNS-style error */
{
        fprintf(stderr, "%s: DNS error %d\n", msg, h_errno);
        exit(0);
}

void app_error(char *msg) /* Application error */
{
        fprintf(stderr, "%s\n", msg);
        exit(0);
}

// 2.错误处理(来自unp)
static void err_doit(int, int, const char *, va_list);

/*
 * Nonfatal error related to a system call.
 * Print a message and return.
 */
void err_ret(const char *fmt, ...) {
        va_list ap;

        va_start(ap, fmt);
        err_doit(1, errno, fmt, ap);
        va_end(ap);
}

/*
 * Fatal error related to a system call.
 * Print a message and terminate.
 */
void err_sys(const char *fmt, ...) {
        va_list ap;

        va_start(ap, fmt);
        err_doit(1, errno, fmt, ap);
        va_end(ap);
        exit(1);
}

/*
 * Fatal error unrelated to a system call.
 * Error code passed as explict parameter.
 * Print a message and terminate.
 */
void err_exit(int error, const char *fmt, ...) {
        va_list ap;

        va_start(ap, fmt);
        err_doit(1, error, fmt, ap);
        va_end(ap);
        exit(1);
}

/*
 * Fatal error related to a system call.
 * Print a message, dump core, and terminate.
 */
void err_dump(const char *fmt, ...) {
        va_list ap;

        va_start(ap, fmt);
        err_doit(1, errno, fmt, ap);
        va_end(ap);
        abort(); /* dump core and terminate */
        exit(1); /* shouldn't get here */
}

/*
 * Nonfatal error unrelated to a system call.
 * Print a message and return.
 */
void err_msg(const char *fmt, ...) {
        va_list ap;

        va_start(ap, fmt);
        err_doit(0, 0, fmt, ap);
        va_end(ap);
}

/*
 * Fatal error unrelated to a system call.
 * Print a message and terminate.
 */
void err_quit(const char *fmt, ...) {
        va_list ap;

        va_start(ap, fmt);
        err_doit(0, 0, fmt, ap);
        va_end(ap);
        exit(1);
}

/*
 * Print a message and return to caller.
 * Caller specifies "errnoflag".
 */
static void err_doit(int errnoflag, int error, const char *fmt, va_list ap) {
        char buf[MAXLINE];
        vsnprintf(buf, MAXLINE, fmt, ap);
        if (errnoflag)
                snprintf(buf + strlen(buf), MAXLINE - strlen(buf), ": %s", strerror(error));
        strcat(buf, "\n");
        fflush(stdout); /* in case stdout and stderr are the same */
        fputs(buf, stderr);
        fflush(NULL); /* flushes all stdio output streams */
}

// 3.信号量
void P(sem_t *sem) {
        if (sem_wait(sem) < 0)
                unix_error("P error");
}

void V(sem_t *sem) {
        if (sem_post(sem) < 0)
                unix_error("V error");
}

// 4.用于socket的IO包
ssize_t readn(int fd, void *vptr, size_t n) {
        size_t nleft;
        ssize_t nread;
        char *ptr;

        ptr = (char *)vptr;
        nleft = n;
        while (nleft > 0) {
                if ((nread = read(fd, ptr, nleft)) < 0) {
                        if (errno == EINTR)
                                nread = 0;
                        else
                                return -1;
                } else if (nread == 0)
                        break;
                nleft -= nread;
                ptr += nread;
        }
        return n - nleft;
}

ssize_t written(int fd, const void *vptr, size_t n) {
        size_t nleft;
        ssize_t nwritten;
        const char *ptr;

        ptr = (const char *)vptr;
        nleft = n;
        while (nleft > 0) {
                if ((nwritten = write(fd, ptr, nleft)) <= 0) {
                        if (nwritten < 0 && errno == EINTR)
                                nwritten = 0;
                        else
                                return -1;
                }
                nleft -= nwritten;
                ptr += nwritten;
        }
        return n;
}
ssize_t rio_readn(int fd, void *usrbuf, size_t n) {
        size_t nleft = n;
        ssize_t nread;
        char *bufp = usrbuf;

        while (nleft > 0) {
                if ((nread = read(fd, bufp, nleft)) < 0) {
                        if (errno == EINTR) /* Interrupted by sig handler return */
                                nread = 0;  /* and call read() again */
                        else
                                return -1; /* errno set by read() */
                } else if (nread == 0)
                        break; /* EOF */
                nleft -= nread;
                bufp += nread;
        }
        return (n - nleft); /* return >= 0 */
}
/* $end rio_readn */

/*
 * rio_writen - robustly write n bytes (unbuffered)
 */
/* $begin rio_writen */
ssize_t rio_writen(int fd, void *usrbuf, size_t n) {
        size_t nleft = n;
        ssize_t nwritten;
        char *bufp = usrbuf;

        while (nleft > 0) {
                if ((nwritten = write(fd, bufp, nleft)) <= 0) {
                        if (errno == EINTR)   /* Interrupted by sig handler return */
                                nwritten = 0; /* and call write() again */
                        else
                                return -1; /* errno set by write() */
                }
                nleft -= nwritten;
                bufp += nwritten;
        }
        return n;
}
/* $end rio_writen */

/*
 * rio_read - This is a wrapper for the Unix read() function that
 *    transfers min(n, rio_cnt) bytes from an internal buffer to a user
 *    buffer, where n is the number of bytes requested by the user and
 *    rio_cnt is the number of unread bytes in the internal buffer. On
 *    entry, rio_read() refills the internal buffer via a call to
 *    read() if the internal buffer is empty.
 */
/* $begin rio_read */
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n) {
        int cnt;

        while (rp->rio_cnt <= 0) { /* Refill if buf is empty */
                rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
                if (rp->rio_cnt < 0) {
                        if (errno != EINTR) /* Interrupted by sig handler return */
                                return -1;
                } else if (rp->rio_cnt == 0) /* EOF */
                        return 0;
                else
                        rp->rio_bufptr = rp->rio_buf; /* Reset buffer ptr */
        }

        /* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
        cnt = n;
        if (rp->rio_cnt < n)
                cnt = rp->rio_cnt;
        memcpy(usrbuf, rp->rio_bufptr, cnt);
        rp->rio_bufptr += cnt;
        rp->rio_cnt -= cnt;
        return cnt;
}
/* $end rio_read */

/*
 * rio_readinitb - Associate a descriptor with a read buffer and reset buffer
 */
/* $begin rio_readinitb */
void rio_readinitb(rio_t *rp, int fd) {
        rp->rio_fd = fd;
        rp->rio_cnt = 0;
        rp->rio_bufptr = rp->rio_buf;
}
/* $end rio_readinitb */

/*
 * rio_readnb - Robustly read n bytes (buffered)
 */
/* $begin rio_readnb */
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n) {
        size_t nleft = n;
        ssize_t nread;
        char *bufp = usrbuf;

        while (nleft > 0) {
                if ((nread = rio_read(rp, bufp, nleft)) < 0)
                        return -1; /* errno set by read() */
                else if (nread == 0)
                        break; /* EOF */
                nleft -= nread;
                bufp += nread;
        }
        return (n - nleft); /* return >= 0 */
}
/* $end rio_readnb */

/*
 * rio_readlineb - robustly read a text line (buffered)
 */
/* $begin rio_readlineb */
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) {
        int n, rc;
        char c, *bufp = usrbuf;

        for (n = 1; n < maxlen; n++) {
                if ((rc = rio_read(rp, &c, 1)) == 1) {
                        *bufp++ = c;
                        if (c == '\n') {
                                n++;
                                break;
                        }
                } else if (rc == 0) {
                        if (n == 1)
                                return 0; /* EOF, no data read */
                        else
                                break; /* EOF, some data was read */
                } else
                        return -1; /* Error */
        }
        *bufp = 0;
        return n - 1;
}
/* $end rio_readlineb */

/**********************************
 * Wrappers for robust I/O routines
 **********************************/
ssize_t Rio_readn(int fd, void *ptr, size_t nbytes) {
        ssize_t n;

        if ((n = rio_readn(fd, ptr, nbytes)) < 0)
                unix_error("Rio_readn error");
        return n;
}

void Rio_writen(int fd, void *usrbuf, size_t n) {
        if (rio_writen(fd, usrbuf, n) != n)
                unix_error("Rio_writen error");
}

void Rio_readinitb(rio_t *rp, int fd) { rio_readinitb(rp, fd); }

ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n) {
        ssize_t rc;

        if ((rc = rio_readnb(rp, usrbuf, n)) < 0)
                unix_error("Rio_readnb error");
        return rc;
}

ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) {
        ssize_t rc;

        if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0)
                unix_error("Rio_readlineb error");
        return rc;
}

// 5.用于信号处理函数的异步信号安全函数
void sio_error(char s[]) {
        write(STDERR_FILENO, s, sio_strlen(s));
        _exit(1);
}

/* 将整形转换为字符串 */
// warning:如果把n声明成有符号数的话，和v进行操作时会将结果转换成无符号数，导致错误
void sio_ltoa(long v, char *s, int n) {
        const char ascii[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                              '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
        long base;

        //将v转换成负数，以防止LONG_MIN溢出
        if (v < 0)
                *s++ = '-';
        else
                v = -v;

        //转换
        base = 1;
        while (v / base <= -n)
                base *= n;

        while (base != 1) {
                *s++ = ascii[v / (-base)]; //必须这么写，因为-LONG_MIN=LONG_MIN
                v %= base;
                base /= n;
        }
        *s++ = ascii[-v];
        *s = '\0';
}

size_t sio_strlen(const char *s) {
        int n;

        n = 0;
        while (*s++ != '\0')
                n++;
        return n;
}

ssize_t sio_puts(char s[]) { return write(STDOUT_FILENO, s, sio_strlen(s)); }

ssize_t sio_putl(long v) {
        char str[128];
        sio_ltoa(v, str, 10);
        return sio_puts(str);
}

//不用手动检测的版本
ssize_t Sio_puts(char s[]) {
        ssize_t n;

        if ((n = sio_puts(s)) < 0)
                sio_error("Sio_puts error\n");
        return n;
}

ssize_t Sio_putl(long v) {
        ssize_t n;

        if ((n = sio_putl(v)) < 0)
                sio_error("Sio_putl error\n");
        return n;
}

// 6.辅助网络包
int open_clientfd(char *hostname, char *port) {
        int client_fd;
        struct addrinfo hints, *listp, *p;

        // 1. 获取hostname和port的值
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_ADDRCONFIG;
        Getaddrinfo(hostname, port, &hints, &listp);

        // 2.连接
        for (p = listp; p; p = p->ai_next) {
                if ((client_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
                        continue;
                if (connect(client_fd, p->ai_addr, p->ai_addrlen) != -1)
                        break;
                Close(client_fd);
        }
        // 3.清理，返回
        freeaddrinfo(listp);

        if (!p)
                return -1;
        else
                return client_fd;
}

int open_listenfd(char *port) {
        struct addrinfo hints, *listp, *p;
        int listen_fd, optval = 1;

        // 1.获取服务器地址
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
        Getaddrinfo(NULL, port, &hints, &listp);

        // 2.bind并listen
        for (p = listp; p; p = p->ai_next) {
                if ((listen_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
                        continue;
                Setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));
                if (bind(listen_fd, p->ai_addr, sizeof(int)) == 0)
                        break;
                Close(listen_fd);
        }

        // 3.清理，listen
        freeaddrinfo(listp);
        if (!p)
                return -1;
        if (listen(listen_fd, LISTENQ) < 0) {
                Close(listen_fd);
                return -1;
        }
        return listen_fd;
}

int Open_clientfd(char *hostname, char *port) {
        int rc;

        if ((rc = open_clientfd(hostname, port)) < 0) {
                if (rc == -1)
                        unix_error("Open_clientfd Unix error");
                else
                        dns_error("Open_clientfd DNS error");
        }
        return rc;
}

int Open_listenfd(char *port) {
        int rc;

        if ((rc = open_listenfd(port)) < 0)
                unix_error("Open_listenfd error");
        return rc;
}
