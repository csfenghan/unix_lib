#include "unix_api.h"

int fstatat(int fd,const char *pathname,struct stat *buf,int flag);
int openat(int fd,const char *path,int oflag,...);
int faccessat(int fd,const char *pathname,int mode,int flag);

void unix_error(char *msg)
{
	fprintf(stderr,"%s: %s\n",msg,strerror(errno));
	exit(0);
}

////////////////////////////////////////////////////////////////
int Open(const char *path,int oflag,mode_t mode)
{
	int fd;
	if((fd=open(path,oflag,mode))<0)
		unix_error("open error");
	return fd;
}

int Openat(int fd,const char *path,int oflag,mode_t mode)
{
	int n;
	if((n=openat(fd,path,oflag,mode))<0)
		unix_error("openat error");
	return n;
}

ssize_t Read(int fd,void *buf,size_t nbytes)
{
	ssize_t n;
	if((n=read(fd,buf,nbytes))<0)
		unix_error("read error");

	return n;
}

ssize_t Write(int fd,const void *buf,size_t nbytes)
{
	ssize_t n;
	if((n=write(fd,buf,nbytes))<0)
		unix_error("write error");
		
	return n;
}

////////////////////////////////////////////////////////////
int Stat(const char *pathname,struct stat *buf)
{
	int n;
	if((n=stat(pathname,buf))<0)
		unix_error("stat error");

	return n;
}

int Fstatat(int fd,const char *pathname,struct stat *buf,int flag)
{
	int n;
	if((n=fstatat(fd,pathname,buf,flag))<0)
		unix_error("fstat error");
	return n;
}

int Access(const char *pathname,int mode)
{
	int n;
	if((n=access(pathname,mode))<0)
		unix_error("access error");

	return n;
}

int Faccessat(int fd,const char *pathname,int mode,int flag)
{
	if(faccessat(fd,pathname,mode,flag)<0)
		unix_error("faccessat error");
	return 0;
}

int Mkdir(const char *pathname,mode_t mode)
{
	if(mkdir(pathname,mode)<0)
		unix_error("mkdir error");
	return 0;
}

int Mkdirat(int fd,const char *pathname,mode_t mode)
{
	if(mkdirat(fd,pathname,mode)<0)
		unix_error("mkdirat error");
	return 0;
}

DIR *Opendir(const char *pathname)
{
	DIR *dp;
	if((dp=opendir(pathname))==NULL)
		unix_error("opendir error");
	return dp;
}

DIR *Fdopendir(int fd)
{
	DIR *dp;
	if((dp=fdopendir(fd))==NULL)
		unix_error("fopendir error");
	return dp;
}

struct dirent *Readdir(DIR *dp)
{
	struct dirent *dirp;
	dirp=readdir(dp);
	return dirp;
}

int Chdir(const char *pathname)
{
	if((chdir(pathname))<0)
		unix_error("chdir error");	
	return 0;
}

char *Getcwd(char *buf,size_t size)
{
	if((buf=getcwd(buf,size))==NULL)
		unix_error("getcwd error");
	return buf;
}

///////////////////////////////////////////////////////////

char *Fgets(char *buf,int n,FILE *fp)
{
	if(fgets(buf,n,fp)==NULL)
		unix_error("fgets error");
	return buf;
}


///////////////////////////////////////////////////////////
struct passwd *Getpwuid(uid_t uid)
{
	struct passwd *pd;
	if((pd=getpwuid(uid))==NULL)
		unix_error("getpwuid error");
	return pd;
}

struct group *Getgrgid(gid_t gid)
{
	struct group *gp;
	if((gp=getgrgid(gid))==NULL)
		unix_error("getgrgid error");
	return gp;
}

//进程环境
int Atexit(void (*func)(void))
{
	if(atexit(func)!=0)
		unix_error("atexit error");
	return 0;
}

char *Getenv(const char *name)
{
	char *n;
	if((n=getenv(name))==NULL)
		unix_error("getenv error");
	return n;
}

int Putenv(char *str)
{
	if(putenv(str)!=0)
		unix_error("putenv error");
	return 0;
}

int Setenv(const char *name,const char *value,int rewrite)
{
	if(setenv(name,value,rewrite)<0)
		unix_error("setenv error");
	return 0;
}

int Unsetenv(const char *name)
{
	if(unsetenv(name)<0)
		unix_error("unsetenv error");
	return 0;
}

int Getrlimit(int resource,struct rlimit *rlptr)
{
	if(getrlimit(resource,rlptr)!=0)
		unix_error("getrlimit error");
	return 0;
}

int Setrlimit(int resource,const struct rlimit *rlptr)
{
	if(setrlimit(resource,rlptr)!=0)
		unix_error("setrlimit error");
	return 0;
}

////////////////////////////////////////////////
pid_t Fork(void)
{
	int n;
	if((n=fork())<0)
		unix_error("fork error");
	return n;
}

pid_t Wait(int *staloc)
{
	pid_t n;
	if((n=wait(staloc))<=0)
		unix_error("wait error");

	return n;
}

pid_t Waitpid(pid_t pid,int *staloc,int options)
{
	pid_t n;
	if((n=waitpid(pid,staloc,options))<=0)
		unix_error("waitpid error");
	
	return n;
}



