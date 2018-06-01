#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include<signal.h>

const int port = 8000;
const int buf_size = 16384;
const int filename_size = 256;

char host_name[20];

void do_service(int temp_sock_descriptor, struct sockaddr_in pin)
{
    int len = 0;
    char buf[buf_size];
    char file_path[filename_size];
    int i;
    if (recv(temp_sock_descriptor, buf, buf_size, 0) == -1) {
        perror("call to recv");
    }

    for(i=1;i<strlen(buf);i++){
        file_path[i-1] = buf[i];
    }
    file_path[i-1] = '\0';

    FILE *fp;

    switch (buf[0]){
        case 'g' :
            fp = fopen(file_path,"rb");
            printf("get file start\n");
            printf("%s", file_path);
            while((len = fread(buf, sizeof(char), buf_size, fp))>0)
            {
                printf("%s\n",buf);
                if(send(temp_sock_descriptor, buf, len, 0)<0){
                    printf("Send File:\\t%s Failed!\\n", file_path);
                }
            }
            break;

        case 'p':
            fp = fopen(file_path,"wb");
            printf("put file start\n");
            while((len = recv(temp_sock_descriptor, buf, buf_size, 0))>0){
                if((fwrite(buf, sizeof(char), len, fp))<len){
                    printf("write fail");
                }
            }
            break;
        default:
            printf("unknow error");
    }
    fclose(fp);
}

int main(void)
{
    struct sockaddr_in sin, pin;
    int sock_descriptor, temp_sock_descriptor, address_size;
    int i, len, on=1;
    pid_t pid;

    sock_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_descriptor == -1) {
        perror("call to socket");
        exit(1);
    }
    setsockopt(sock_descriptor,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    bzero(&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);
    if (bind(sock_descriptor, (struct sockaddr*)&sin, sizeof(sin)) == -1) {
        perror("call to bind");
        exit(1);
    }
    if(listen(sock_descriptor, 100) == -1) {
        perror("call to listen");
        exit(1);
    }
    printf("Accepting connections ...\n");

    while(1) {
        address_size = sizeof(pin);
        temp_sock_descriptor = accept(sock_descriptor, (struct sockaddr *)&pin, (socklen_t*)&address_size);
        if(temp_sock_descriptor == -1) {
            perror("call to accept");
            exit(1);
        }


        if((pid=fork())<0){
            perror("fork fail");
            exit(1);
        }else if(pid == 0){
            // main work
            close(sock_descriptor);
            do_service(temp_sock_descriptor, pin);
            exit(0);
        }
        close(temp_sock_descriptor);
    }
}