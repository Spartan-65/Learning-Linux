#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	pid_t pid;
	if((pid=fork())==0){
		getchar();
		exit(0);
	}
	getchar();
}
