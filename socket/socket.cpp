//Client - Server communication over socket
//Aliens from different planets are trying to connect Earth over TCP/IP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <thread>

using namespace std;

#define PORT 8080 //aliens communication port

//  SERVER          CLIENT
//  socket          socket
//  setsockopt
//  bind
//  listen <------- connect
//  accept
//  send/recv <---> send/recv

//Aliens life searcher from Earth behaves like a server which listens aliens
class LifeSearcher {
public:
    bool initializeServer() {
        //socket creation
        //AF_INET is IPv4 protocol
        //SOCK_STREAM is TCP (reliable, connection oriented)
        int socketDescriptor;
        if ((socketDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            return false;
        }

        //Adjusting socket options
        struct sockaddr_in address;
        int addrlen = sizeof(address);
        int option = 1;
        if (setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option))) {
            return  false;
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        //Bind socket to address and port
        if (bind(socketDescriptor, (struct sockaddr *)&address, sizeof(address)) < 0) {
            return false;
        }

        //second parameter is backlog defines max number of pending connections
        if (listen(socketDescriptor, 10) < 0) {
            return false;
        }

        //accept new socket
        int newSocket;
        if ((newSocket = accept(socketDescriptor, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            return false;
        }
        char buffer[1024] = {0};
        int valread = read(newSocket , buffer, 1024);
        cout << buffer << endl;
        string helloText = "Hello stranger, we are your friends!";
        char hello[1024];
        strcpy(hello, helloText.c_str());
        send(newSocket , hello , helloText.length() , 0 );
        close(socketDescriptor);

        return true;
    }
};

//an alien client from unknown planet tries to communicate with server
class Alien {
public:
    bool connectToEarth() {
        this_thread::sleep_for(chrono::milliseconds(100));
        //socket creation
        //AF_INET is IPv4 protocol
        //SOCK_STREAM is TCP (reliable, connection oriented)
        int socketDescriptor;
        if ((socketDescriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            return false;
        }

        struct sockaddr_in serverAddress;
        memset(&serverAddress, '0', sizeof(serverAddress));
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(PORT);

        //Convert IPv4 address from text to binary
        if(inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0) {
            return false;
        }

        if (connect(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
            return false;
        }
        string helloText = "Hello from planet XX-YY15F45!";
        char hello[1024];
        strcpy(hello, helloText.c_str());
        send(socketDescriptor , hello , helloText.length() , 0 );
        char buffer[1024] = {0};
        int valread = read(socketDescriptor, buffer, 1024);
        cout << buffer << endl;

        close(socketDescriptor);

        return true;
    }
};

int main()
{
    LifeSearcher searcher;
    Alien alien;

    thread threadServer(&LifeSearcher::initializeServer, &searcher);
    thread threadClient(&Alien::connectToEarth, &alien);

    threadClient.join();
    threadServer.join();

    return 0;
}