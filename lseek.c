#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd,num;
	char buf[100];
	fd = open("./file1",O_RDONLY);
	if(fd < 0){
		perror("open failure");
		exit(1);
	}
	num = read(fd,buf,3);
	write(STDOUT_FILENO,buf,num);
	lseek(fd,2,SEEK_CUR);
	num = read(fd,buf,4);
	write(STDOUT_FILENO, buf ,num);
	close(fd);
	return 0;
}
