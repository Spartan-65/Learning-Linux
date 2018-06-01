#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

const int port = 8000;
const int buf_size = 16384;
const int filename_size = 256;
const int addr_size = 20;

char host_name[addr_size];
int get_file = 1;
char local_path[filename_size], remote_path[filename_size];
char file_name[filename_size];

char* push_str(){
    char flag = get_file? 'g':'p';
    char *temp = (char *)calloc( strlen(remote_path)+3, sizeof(char*));
    temp[0] = flag;
    memcpy(&temp[1],remote_path,strlen(remote_path));
    puts(temp);
    return temp;
}

void initsettings(const char *args1, const char *args2){
    int have_colon = 0;
    char temp_str[filename_size];
    int p = strlen(args1), p1=0;

    while (args1[p]!='/'&&args1[p]!=':'&&p>=0) p--;
    p+=1;
    while(args1[p]!='\0') file_name[p1++] = args1[p++];
    file_name[p1] = '\0';
    p = 0,p1=0;
    while(p<strlen(args1)){
        temp_str[p1] = args1[p++];
        if(temp_str[p1] == ':'){
            temp_str[p1] = '\0';
            memcpy(host_name, temp_str, strlen(temp_str)+1);
            have_colon = 1;
            p1 = -1;
        }
        p1++;
    }
    temp_str[p1] = '\0';
    if(have_colon){
        memcpy(remote_path, temp_str, strlen(temp_str));
        memcpy(local_path, args2, strlen(args2));
    } else{
        get_file = 0;
        memcpy(local_path, temp_str, strlen(temp_str));
        p = 0, p1=0;
        while(p<strlen(args2)){
            temp_str[p1] = args2[p++];
            if(temp_str[p1] == ':'){
                temp_str[p1] = '\0';
                memcpy(host_name, temp_str, strlen(temp_str));
                p1 = -1;
            }
            p1++;
        }
        temp_str[p1] = '\0';
        memcpy(remote_path, temp_str, strlen(temp_str));
    }
    puts(local_path);
    if(strlen(local_path)==1) local_path[1] = '/', memcpy(&local_path[2], file_name, strlen(file_name));
    if (local_path[0]!='/'&&local_path[0]!='.'){
        memcpy(temp_str,local_path,strlen(local_path));
        temp_str[strlen(local_path)] = '\0';
        local_path[0]='.',local_path[1]='/';memcpy(&local_path[2],temp_str,strlen(temp_str));
    }
    if(!get_file){
        int len = strlen(remote_path);
        if(!len) remote_path[len]='.',len++;
        if(remote_path[len-1]!='/') remote_path[len] = '/',len++;
        memcpy(&remote_path[len], file_name, strlen(file_name));
    }
}



int main(int argc, char *argv[])
{
    char buf[buf_size];  //, message[256];
    int socket_descriptor;
    struct sockaddr_in pin;
    int len;
    printf("start...\n");



    initsettings(argv[1], argv[2]);

    printf("host name:");
    puts(host_name);
    printf("local path:");
    puts(local_path);
    printf("remote path:");
    puts(remote_path);
    printf("file name:");
    puts(file_name);


    bzero(&pin, sizeof(pin));
    pin.sin_family = AF_INET;
    inet_pton(AF_INET, host_name, &pin.sin_addr);
    pin.sin_port = htons(port);

    if((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error opening socket\n");
        exit(1);
    }
    if (connect(socket_descriptor, (struct sockaddr *)&pin, sizeof(pin)) == -1) {
        perror("Error connecting to socket\n");
        exit(1);
    }

    char *send_str = push_str();
    printf("Sending message %s to server...\n", send_str);

    if (write(socket_descriptor, send_str, strlen(send_str)) == -1) {
        perror("Error in send\n");
        exit(1);
    }
    printf(".. sent message.. wait for response...\n");
    FILE *fp;

    if(get_file){
        printf("get file...\n");
        fp = fopen(local_path, "wb");
        while((len = recv(socket_descriptor,buf,buf_size,0))>0){
            if((fwrite(buf, sizeof(char), len, fp))<len){
                printf("write fail");
            }
        }
    } else{
        printf("push file... %s\n",local_path);
        fp = fopen(local_path,"rb");

        while ((len = fread(buf, sizeof(char),buf_size, fp)) > 0){
            if(send(socket_descriptor, buf, len, 0)<0){
                printf("Send File:\\t%s Failed!\\n", local_path);
            }
        }
    }
    printf("\nResponse from server:\n\n%s\n", buf);
    close(socket_descriptor);
    return 0;
}