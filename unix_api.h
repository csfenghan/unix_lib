#ifndef __UNIX_LIB_H
#define __UNIX_LIB_H

#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//支持c++接口
#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 *	定义的全局变量和结构体
 **********************************************************************/
// 1.默认的文件权限
#define DEF_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
#define DEF_UMASK S_IWGRP | S_IWOTH

// 2.缩写
typedef struct sockaddr SA;

// 3.用于socket通信包
#define RIO_BUFSIZE 8192
typedef struct {
        int rio_fd;                /* Descriptor for this internal buf */
        int rio_cnt;               /* Unread bytes in internal buf */
        char *rio_bufptr;          /* Next unread byte in internal buf */
        char rio_buf[RIO_BUFSIZE]; /* Internal buffer */
} rio_t;

// 4.外部变量
extern int h_errno;    /* Defined by BIND for DNS errors */
extern char **environ; /* Defined by libc */

// 5.定义的常数
#define MAXLINE 8192 /* Max text line length */
#define MAXARGS 128  /* 最大参数长度 */
#define MAXBUF 8192  /* Max I/O buffer size */
#define LISTENQ 1024 /* Second argument to listen() */

/***********************************************************************
 *	封装的系统函数
 **********************************************************************/
// 1.进程控制
pid_t Fork(void);
void Execve(const char *filename, char *const argv[], char *const envp[]);
pid_t Wait(int *status);
pid_t Waitpid(pid_t pid, int *iptr, int options);
void Kill(pid_t pid, int signum);
unsigned int Sleep(unsigned int secs);
void Pause(void);
unsigned int Alarm(unsigned int seconds);
void Setpgid(pid_t pid, pid_t pgid);
pid_t Getpgrp();

// 2.基本IO
int Open(const char *pathname, int flags, mode_t mode);
ssize_t Read(int fd, void *buf, size_t count);
ssize_t Write(int fd, const void *buf, size_t count);
off_t Lseek(int fildes, off_t offset, int whence);
void Close(int fd);
int Select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
int Dup2(int fd1, int fd2);
void Stat(const char *filename, struct stat *buf);
void Fstat(int fd, struct stat *buf);
DIR *Opendir(const char *name);

// 3.标准IO
void Fclose(FILE *fp);
FILE *Fdopen(int fd, const char *type);
char *Fgets(char *ptr, int n, FILE *stream);
FILE *Fopen(const char *filename, const char *mode);
void Fputs(const char *ptr, FILE *stream);
size_t Fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
void Fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

// 4.内存管理
void *Malloc(size_t size);
void *Realloc(void *ptr, size_t size);
void *Calloc(size_t nmemb, size_t size);
void Free(void *ptr);
void *Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
void Munmap(void *start, size_t length);

// 5.信号
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);
void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
void Sigemptyset(sigset_t *set);
void Sigfillset(sigset_t *set);
void Sigaddset(sigset_t *set, int signum);
void Sigdelset(sigset_t *set, int signum);
int Sigismember(const sigset_t *set, int signum);

// 6.线程控制
void Pthread_create(pthread_t *tidp, pthread_attr_t *attrp, void *(*routine)(void *), void *argp);
void Pthread_join(pthread_t tid, void **thread_return);
void Pthread_cancel(pthread_t tid);
void Pthread_detach(pthread_t tid);
void Pthread_exit(void *retval);
pthread_t Pthread_self(void);
void Pthread_once(pthread_once_t *once_control, void (*init_function)());

// 7.进程间通信
void Sem_init(sem_t *sem, int pshared, unsigned int value);
int Pipe(int pipefd[2]);
key_t Ftok(const char *path, int id);
int Shmget(key_t key, size_t size, int flag);
int Shmctl(int shmid, int cmd, struct shmid_ds *buf);
void *Shmat(int shmid, const void *addr, int flag);
int Shmdt(const void *addr);

// 8.socket接口封装
int Socket(int domain, int type, int protocol);
void Setsockopt(int s, int level, int optname, const void *optval, int optlen);
void Bind(int sockfd, struct sockaddr *my_addr, int addrlen);
void Listen(int s, int backlog);
int Accept(int s, struct sockaddr *addr, socklen_t *addrlen);
void Connect(int sockfd, struct sockaddr *serv_addr, int addrlen);
struct hostent *Gethostbyname(const char *name);
struct hostent *Gethostbyaddr(const char *addr, int len, int type);
int Getaddrinfo(const char *hostname, const char *service, const struct addrinfo *hints,
                struct addrinfo **result);

/***********************************************************************
 *	三、实现的辅助功能包
 **********************************************************************/
// 1.错误处理函数(来自csapp)
void unix_error(char *msg);
void posix_error(int code, char *msg);
void dns_error(char *msg);
void app_error(char *msg);

// 2.错误处理函数(来自unp)
void err_ret(const char *fmt, ...);
void err_sys(const char *fmt, ...);
void err_exit(int error, const char *fmt, ...);
void err_dump(const char *fmt, ...);
void err_msg(const char *fmt, ...);
void err_quit(const char *fmt, ...);

// 3.信号量
void P(sem_t *sem);
void V(sem_t *sem);

// 4.用于socket的IO包
ssize_t readn(int filedes, void *buff, size_t nbytes);
ssize_t written(int filedes, const void *buff, size_t nbytes);

ssize_t rio_readn(int fd, void *usrbuf, size_t n);
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
void rio_readinitb(rio_t *rp, int fd);
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

ssize_t Rio_readn(int fd, void *usrbuf, size_t n);
void Rio_writen(int fd, void *usrbuf, size_t n);
void Rio_readinitb(rio_t *rp, int fd);
ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

// 5.用于信号处理函数的异步信号安全的IO
void sio_error(char s[]);
void sio_ltoa(long v, char *s, int n);
size_t sio_strlen(const char *s);
ssize_t sio_puts(char s[]);
ssize_t sio_putl(long v);

ssize_t Sio_puts(char s[]);
ssize_t Sio_putl(long v);

// 6.网络包
int open_clientfd(char *hostname, char *portno);
int open_listenfd(char *portno);
int Open_clientfd(char *hostname, char *port);
int Open_listenfd(char *port);

//支持c++接口
#ifdef __cplusplus
}
#endif

#endif
