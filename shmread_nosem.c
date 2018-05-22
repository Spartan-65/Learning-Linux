#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <unistd.h>
#include <errno.h>



#define BUFSZ 4096 /* Size of the segment */

int main(void)
{
	int shmid;
	char *shmbuf; /* Address in process */
	
	if((shmid = shmget(888, BUFSZ, 0666|IPC_CREAT)) < 0) {
		perror("shmget");
		exit(EXIT_FAILURE);
	}
	 /* Attach the segment */
	if((shmbuf = shmat(shmid, 0, 0)) < (char *)0) {
		perror("shmat");
		exit(EXIT_FAILURE);
	}
	
	write(STDOUT_FILENO, shmbuf, 1);
	
	 /*
				  29 while(1) {
				  30 printf("in while\n");
				  31 write(STDOUT_FILENO,shmbuf,1);
				  32 sleep(3);
				  33 }
				  34 */
	write(STDOUT_FILENO, "\n", 1);
	exit(EXIT_SUCCESS);
}
