// Client side C/C++ program to demonstrate Socket
// programming
#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <queue>
#include <thread>

#define PORT 11051
#define MAXSTR 100
#define DISPLAY 10

using std::string;
using std::queue;

void *read(void *socket){
	int sock = *(int*)socket, valread;
	char buffer[MAXSTR+7] = { 0 };
	string str;
	queue<string> chats;
    for(;valread = read(sock, buffer, MAXSTR+7);){
		string prePush(buffer);
		chats.push(buffer);
		if(chats.size()>DISPLAY)
			chats.pop();
		if(valread <= 0)
			break;
		std::cout<< u8"\033[2J\033[1;1H";
		queue<string> display(chats);
		for(;display.size();display.pop())
			std::cout << display.front() << "\n";

		if(valread <= 0)
			break;
		memset(buffer, 0, sizeof buffer);
    }
	if(!~valread) perror("read failed");
	if(!valread) puts("\nServer closed");

	// closing the connected socket
	close(sock);
	return 0;
}

int main(int argc, char const* argv[])
{
	int sock = 0, valread, client_fd;
	struct sockaddr_in serv_addr;
	char buffer[MAXSTR+7] = { 0 };
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
	if(!~read(sock, buffer, MAXSTR))
		perror("read failed");
	std::cout<< u8"\033[2J\033[1;1H";
    std::cout << buffer << "\n";
	memset(buffer, 0, sizeof buffer);

	pthread_t thread_id;
	if( pthread_create( &thread_id , NULL ,  read , (void*) &sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

    string str;
	queue<string> chats;
    for(;std::getline(std::cin, str);){
		for(;str.length();){
			if(str.length()>MAXSTR){
				string part = str.substr(0,MAXSTR);
				send(sock, part.c_str(), part.length(), 0);
				str = str.substr(MAXSTR, str.length()-1);
			}
			else {
				send(sock, str.c_str(), str.length(), 0);
				str="";
			}
		}
    }
	// closing the connected socket
	close(client_fd);
	return 0;
}
