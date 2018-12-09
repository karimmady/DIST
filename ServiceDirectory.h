#ifndef SERVICEDIRECTORY_H
#define SERVICEDIRECTORY_H

#include <fstream>
#include <map>
#include <string>
#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string>
#include<arpa/inet.h>
#include<unistd.h>
#include "UDPServerSocket.h"
#include "UDPClientSocket.h"
#include "Message.h"
using namespace std;




class ServiceDirectory
{
	private:
		ofstream authTableOut;
		ifstream authTableIn;
		map <string,string> users;
        map <string,string> sysusers;
		map <string,struct sockaddr_in> user_addresses;
		UDPSocket * UDPSSocket;
		UDPSocket * UDPCSocket;
		int myAdd;
		int myPort;
		bool Authenticate(string username, string password, struct sockaddr_in add,int serverport);
		bool Register(string username, string password);
	public:
		ServiceDirectory(int myaddr,int myport);
		void Listen();
		void Serve();
		~ServiceDirectory();
};
#endif //SERVICEDIRECTORY_H
