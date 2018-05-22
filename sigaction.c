#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
static void sig_int(int);

int main(void)
{
	//struct sigaction act;
	sigset_t newmask, oldmask,pendmask;
	//printf("%d\n",SIGQUIT);
	if(signal(SIGQUIT, sig_int) == SIG_ERR)
	{printf("cat't catch SIGQUIT"); exit(2);}
	if(signal(SIGINT, sig_int) == SIG_ERR)
	{printf("cat't catch SIGINT"); exit(2);}
/*
   sigemptyset(&act.sa_mask);
   sigaddset(&act.sa_mask, SIGQUIT);
   act.sa_handler = sig_int;
   act.sa_flags = 0;
   if (sigaction(SIGINT, &act, NULL) == -1)
   { printf("can't catch SIGINT"); exit(3); }
 */
	sleep(10);
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGQUIT);
	//Enter Critical Area.block SIGQUIT and save current signal mask
	if(sigprocmask(SIG_BLOCK, &newmask,&oldmask)<0)
	{printf("SIG_BLOCK, error"); exit(4);}
	sleep(15);
	if(sigpending(&pendmask)<0)
	{printf("sigpending error");exit(5);}
	if(sigismember(&pendmask, SIGQUIT))
		printf("\nSIGQUIT pending\n");
	
	if(sigprocmask(SIG_SETMASK, &oldmask, NULL)<0)
	{printf("SIG_SETMASK error"); exit(6);}
	printf("SIGQUIT unblocked\n");
	sleep(10);
	exit(0);
}

static void sig_int(int signo)
{
	int i;
	if(signo==SIGINT){
		printf("received SIGINT\n");
		//for(i=1e9;i>0;i--);
		printf("processed SIGINT\n");
	}else if(signo == SIGQUIT){
		printf("caught SIGQUIT\n");
	}else {
		printf("received signal %d \n",signo);
	}
	return ;
}
