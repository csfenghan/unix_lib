#ifndef MINISHELL_H
#define MINISHELL_H

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <signal.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/resource.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void unix_error(char *msg);

//文件IO
int Open(const char *path,int oflag,mode_t mode);
int Openat(int fd,const char *path,int oflag,mode_t mode);
int Creat(const char *path,mode_t mode);
int Close(int fd);

off_t Lseek(int fd,off_t offset,int whence);
ssize_t Read(int fd,void *buf,size_t nbytes);
ssize_t Write(int fd,const void *buf,size_t nbytes);
ssize_t Pread(int fd,void *buf,size_t nbytes,off_t offset);
ssize_t Pwrite(int fd,const void *buf,size_t nbytes,off_t offset);

int Dup(int fd);
int Dup2(int fd,int fd2);

int Fsync(int fd);
int Fdatasync(int fd);
void Sync(void);

int Fcntl(int fd,int cmd,...);
int Ioctl(int fd,int request,...);

//文件和目录
int Stat(const char *pathname,struct stat *buf);
int Fstat(int fd,struct stat *buf);
int Lstat(const char *pathname,struct stat *buf);
int Fstatat(int fd,const char *pathname,struct stat *buf,int flag);

int Access(const char *pathname,int mode);
int Faccessat(int fd,const char *pathname,int mode,int flag);

int Chmod(const char *pathname,mode_t mode);
int Fchmod(int fd,mode_t mode);
int Fchmodat(int fd,const char *pathname,mode_t mode,int flag);

int Chown(const char *pathname,uid_t owner,gid_t group);
int Fchown(int fd,uid_t owner,gid_t group);
int Fchownat(int fd,const char *pathname,uid_t owner,gid_t group,int flag);
int Lchown(const char *pathname,uid_t owner,gid_t group);

int Trunncate(const char *pathname,off_t length);
int Ftruncate(int fd,off_t length);

int Link(const char *existingpath,const char *newpath);
int Linkat(int fd,const char *existingpath,int nfd,const char *newpath,int flag);

int Unlink(const char *pathname);
int Unlinkat(int fd,const char *pathname,int flag);
int Remove(const char *pathname);

int Rename(const char *oldname,const char *newname);
int Renameat(int oldfd,const char *oldname,int newfd,const char *newname);

int Symlink(const char *actualpath,const char *sympath);
int Symlinkat(const char *actualpath,int fd,const char *sympath);

ssize_t Readlink(const char *pathname,char *buf,size_t bufsize);
ssize_t Readlinkat(int fd,const char *pathname,char *buf,size_t bufsize);

int Futimens(int fd,const struct timespec times[2]);
int Utimensat(int fd,const char *path,const struct timespec times[2],int flag);

int Mkdir(const char *pathname,mode_t mode);
int Mkdirat(int fd,const char *pathname,mode_t mode);
int Rmdir(const char *pathname);

DIR *Opendir(const char *pathname);
DIR *Fdopendir(int fd);
struct dirent *Readdir(DIR *dp);
void Rewinddir(DIR *dp);
int Closedir(DIR *dp);
long Telldir(DIR *dp);
void Seekdir(DIR *dp,long loc);

int Chdir(const char *pathname);
int Fchdir(int fd);
char *Getcwd(char *buf,size_t size);


//标准IO
char *Fgets(char *buf,int n,FILE *fp);
int Fputs(const char *s,FILE *stream);

//系统文件信息
struct passwd *Getpwuid(uid_t uid);
struct group *Getgrgid(gid_t gid);

//进程环境
int Atexit(void (*func)(void));

char *Getenv(const char *name);
int Putenv(char *str);
int Setenv(const char *name,const char *value,int rewrite);
int Unsetenv(const char *name);
int Getrlimit(int resource,struct rlimit *rlptr);
int Setrlimit(int resource,const struct rlimit *rlptr);

pid_t Fork(void);

pid_t Wait(int *staloc);
pid_t Waitpid(pid_t pid,int *staloc,int options);

//信号
void (*Signal(int signo,void (*func)(int)))(int);

#endif
