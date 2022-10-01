// Server side C/C++ program to demonstrate Socket
// programming
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>

#define PORT 11051
#define MAXSTR 1024
#define DISPLAY 100

using std::string;
using std::vector;

vector<int> socketList;

void *work(void *socket){
    int sock = *(int*)socket;
    int read_size;
    char *message , buffer[MAXSTR+1];
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);

    message="Welcome to the server";
    send(sock, message, strlen(message), 0);

    int res = getpeername(sock, (struct sockaddr *)&addr, &addr_size);
    char *clientip = new char[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), clientip, INET_ADDRSTRLEN);
    int port=ntohs(addr.sin_port);

    for(;(read_size = recv(sock , buffer , MAXSTR , 0)) > 0;){
        buffer[read_size] = '\0';
        printf("%s:%d> %s\n", clientip, port, buffer);
        string name=std::to_string(port*1234%99999);
        for(int i=1;i<name.length();i++)
            name[i]=name[i]-'0'+'a';
        name[0]=name[0]-'0'+'A';
        char str[MAXSTR+7];
        sprintf(str, "%s> %s", name.c_str(), buffer);
        for(vector<int>::iterator iter = socketList.begin(); iter < socketList.end(); iter++ )
            send(*iter, str, strlen(str), 0);
        memset(buffer, 0, sizeof buffer);
    }

    if(read_size == 0)
    {
        for(vector<int>::iterator iter = socketList.begin(); iter < socketList.end(); iter++ )
            if( *iter == sock )
            {
                socketList.erase( iter );
                break;
            }
        printf("> %s:%d disconnected\n",clientip,port);
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

	// closing the connected socket
	close(sock);
	return 0;
}

int main(int argc, char const* argv[])
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[MAXSTR] = { 0 };

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    pthread_t thread_id;

    for(;(new_socket = accept(server_fd, (struct sockaddr*)&address,(socklen_t*)&addrlen));){
        socketList.push_back(new_socket);
        if( pthread_create( &thread_id , NULL ,  work , (void*) &new_socket) < 0)
        {
            perror("could not create thread");
            return 1;
        }
    }

    if ((new_socket < 0)) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
