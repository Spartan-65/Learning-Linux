#include < unistd.h >
#include < signal.h >
#include < sys/param.h >
#include < sys/types.h >
#include < sys/stat.h >

void init_deamon(void)
{
	pid_t pid,i;
	
	if((pid=fork())<0)
		return (-1);
	else if(pid != 0)
		exit(0);

	setsid();
	if((pid=fork())) exit(0);
	chdir("./tmp");
	umask(0);
	for(i=0;i<sysconf(_SC_OPEN_MAX);i++)
		close(i);
	return(0);
}
