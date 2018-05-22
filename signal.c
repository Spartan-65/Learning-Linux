#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static void sig_usr(int);
int main(void)
{
	int i;
	for(i=1;i<64;i++)
		if(signal(i,sig_usr)== SIG_ERR)
			printf("cat't catch sig %d\n",i);
	
	if(signal(SIGTSTP,SIG_DFL)==SIG_ERR)
		printf("cat't catch sig %d]n",i);
	
	if(signal(SIGUSR1,SIG_IGN)==SIG_ERR)
	{perror("signal");exit(-1);}
	
	if(signal(SIGUSR2,sig_usr)==SIG_ERR)
	{perror("signal");exit(-1);}
	
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
	{ perror("signal"); exit(-1); }
	for(;;){
		printf("abc\n");
		pause();
	}
}
static void sig_usr(int signo)
{
	if(signo == SIGUSR1)
		printf("received SIGUSR1\n");
	else if (signo == SIGUSR2)
		printf("received SIGUSR2\n");
	else 
		printf("received signal %d\n", signo);
	return ;
}
