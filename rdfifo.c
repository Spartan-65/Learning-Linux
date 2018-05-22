
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>

int main(void)
{
	int fd, fd2;
	int len;
	char buf[PIPE_BUF] = "abc\n";
	mode_t mode=0666;
	if (mkfifo("/tmp/fifo1",mode) <0)
		{ printf("mkfifo error\n"); exit(1); }
	if ((fd = open("/tmp/fifo1", O_RDONLY)) < 0)
		{ printf("open error\n"); exit(1); }
	while((len = read(fd, buf, PIPE_BUF-1)) > 0)
	{
		printf("rdfifo read: %s", buf);
	}
	close(fd);
	if (len == 0)
		printf("read end\n");
	exit(EXIT_SUCCESS);
}
