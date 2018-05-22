#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

int main(void)
{
	int msgid;
	struct mymesg{
		long mytype;
		char mdata[512];
	}message;
	if((msgid =msgget(999,0666|IPC_CREAT))<0)
	{perror("msgget");exit(1);}
	message.mytype=1;
	strcpy(message.mdata,"hello\n");
	if (msgsnd(msgid, (void *)&message, strlen(message.mdata) + 1, 0) < 0)
	{perror("msgsnd");exit(2);}
	return 0;
}
