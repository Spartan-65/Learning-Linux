//
// Created by spartan on 5/22/18.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

int main(void)
{
    int addrnum;
    char addrstring[16] = "192.168.6.100";
    printf("address in dotted-quad format is %s\n", addrstring);
    inet_pton(AF_INET, addrstring, &addrnum);
    printf("address in network byteorder integer is 0x%x\n",addrnum);
    char addrstring2[16] = "2.2.2.2";
    if(inet_ntop(AF_INET, &addrnum, addrstring2, 16) == NULL)
        perror("inet_ntop");
    printf("address in dotted-quad format is %s\n", addrstring2);
    return 0;
}
