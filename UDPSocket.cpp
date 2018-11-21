#include "UDPSocket.h"
using namespace std;

UDPSocket::UDPSocket()
{
	if((sock=socket(AF_INET,SOCK_DGRAM,0))<0)
		cerr << "Socket Creation Failed\n";
}

bool UDPSocket::initializeServer(int _myAddr, int _myPort)
{
	memset(&myAddr, 0, sizeof(myAddr));
  memset(&peerAddr, 0, sizeof(peerAddr));
  myAddr.sin_family=AF_INET;
  myAddr.sin_addr.s_addr=htonl(_myAddr);
  myAddr.sin_port=htons(_myPort);

  if(bind(sock,(sockaddr *) &myAddr, sizeof(myAddr)) < 0)
	{
		cerr<< "Bind Failed\n";
		return 0;
	}

	cout << "Bind Successful\n";
	return 1;
}

bool UDPSocket::initializeClient(int _myAddr, int _peerAddr, int _Port,int x)
{
	memset(&myAddr, 0, sizeof(myAddr));
  memset(&peerAddr, 0, sizeof(peerAddr));
  myAddr.sin_family=AF_INET;
  myAddr.sin_addr.s_addr=htonl(_myAddr);
  myAddr.sin_port=htons(_Port);

  peerAddr.sin_family=AF_INET;
  peerAddr.sin_addr.s_addr=htonl(_peerAddr);
	peerAddr.sin_port=htons(_Port);


	return 1;
}


bool UDPSocket::initializeClient(int _myAddr, int _peerAddr, int _Port)
{
	memset(&myAddr, 0, sizeof(myAddr));
  memset(&peerAddr, 0, sizeof(peerAddr));
  myAddr.sin_family=AF_INET;
  myAddr.sin_addr.s_addr=htonl(_myAddr);
  myAddr.sin_port=htons(_Port);

  peerAddr.sin_family=AF_INET;
  peerAddr.sin_addr.s_addr=htonl(_peerAddr);
	peerAddr.sin_port=htons(_Port);

	if(bind(sock,(sockaddr *) &myAddr, sizeof(myAddr)) < 0)
	{
		cerr<< "Bind Failed\n";
		return 0;
	}

	cout << "Bind Successful\n";

}


void UDPSocket::changepeerip(struct sockaddr_in peerip)
{
	peerAddr=peerip;
}

int UDPSocket::writeToSocket(string buff, int maxBytes)
{
	int amount =-1;
	if ((amount = sendto(sock,buff.c_str(),buff.length(),MSG_CONFIRM,(sockaddr *) &peerAddr, sizeof(peerAddr))) <= 0)
	{
		cerr << "Sending Failed\n";
		return amount;
	}
	return amount;
}

char * UDPSocket::readFromSocketWithTimeout(int timeoutSeconds)
{
	memset(buffer,0,sizeof(buffer));
	socklen_t x= sizeof(struct sockaddr_in);
	int amount=0;
	struct timeval tv;
  	tv.tv_sec = timeoutSeconds;
  	tv.tv_usec = 0;
		setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv,sizeof(struct timeval));
		amount=recvfrom(sock,buffer,1000000,MSG_WAITALL, (sockaddr *) &peerAddr, (&x));
		if(amount==-1)
			cerr << "Timeout\n";


	return buffer;
}

char * UDPSocket::readFromSocketWithTimeout(int timeoutSeconds,struct sockaddr_in &client, bool &flag)
{
	memset(buffer,0,sizeof(buffer));
	socklen_t x= sizeof(struct sockaddr_in);
	int amount=0;
	struct timeval tv;
  	tv.tv_sec = timeoutSeconds;
  	tv.tv_usec = 0;

  	client.sin_family=AF_INET;
  	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv,sizeof(struct timeval));
	amount=recvfrom(sock,buffer,1000000,MSG_WAITALL, (sockaddr *) &client, (&x));
	if(amount==-1)
		{
			cerr << "Timeout\n";
			flag = false;
		}

	return buffer;
}

char * UDPSocket::readFromSocketWithTimeout(int timeoutSeconds,struct sockaddr_in &client)
{
	memset(buffer,0,sizeof(buffer));
	socklen_t x= sizeof(struct sockaddr_in);
	int amount=0;
	struct timeval tv;
  	tv.tv_sec = timeoutSeconds;
  	tv.tv_usec = 0;

  	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv,sizeof(struct timeval));
	amount=recvfrom(sock,buffer,1000000,MSG_WAITALL, (sockaddr *) &client, (&x));
	if(amount==-1)
		cerr << "Timeout\n";

	return buffer;
}

char * UDPSocket::readFromSocketWithTimeout(int timeoutSeconds, bool& flag)
{
	memset(buffer,0,sizeof(buffer));
	socklen_t x= sizeof(struct sockaddr_in);
	int amount=0;
	struct timeval tv;
	tv.tv_sec = timeoutSeconds;
	tv.tv_usec = 0;

	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv,sizeof(struct timeval));
	amount=recvfrom(sock,buffer,1000000,MSG_WAITALL, (sockaddr *) &peerAddr, (&x));
	if(amount==-1)
		{
			cerr << "Timeout\n";
			flag = false;
		}

	return buffer;
}


UDPSocket::~UDPSocket()
{
	close(sock);
}
