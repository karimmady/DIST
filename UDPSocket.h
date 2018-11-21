#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
using namespace std;

class UDPSocket
{
	protected:
		int sock;
		struct sockaddr_in myAddr;
		struct sockaddr_in peerAddr;
		char * myAddress;
		char * peerAddress;
		int myPort;
		int peerPort;
		bool enabled;
		pthread_mutex_t mutex;
		char buffer[1000000]= "";
	public:
		UDPSocket();
		bool initializeServer(int _myAddr, int _myPort);
		bool initializeClient(int _myAddr, int _peerAddr, int _Port);
		bool initializeClient(int _myAddr, int _peerAddr, int _Port, int x);
		char * readFromSocketWithTimeout(int timeoutSeconds,struct sockaddr_in &client, bool &flag);
		void changepeerip(struct sockaddr_in peerip);
		int writeToSocket(string buff, int maxBytes);
		char * readFromSocketWithTimeout(int timeoutSeconds);
		char * readFromSocketWithTimeout(int timeoutSeconds,struct sockaddr_in &);
		char * readFromSocketWithTimeout(int timeoutSeconds, bool&);
		~UDPSocket ( );
};
#endif // UDPSOCKET_H
