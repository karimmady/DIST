#include "Peer.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
	char * po;
	int myip=inet_addr(argv[2]);
	int serverip=inet_addr(argv[3]);
	int port=strtol(argv[1],&po,10);
	bool x;
	Peer ana(ntohl(myip),ntohl(serverip),port,x);
	x=ana.Register("hawas","gamed");
	if(x)
		cout << "Registered hawas gamed\n";
	else 
		cout << "Already exists man\n";
	x=ana.Register("hawas","3anteel");
	if(!x)
		cout << "Already exists man\n";
	x=ana.AnnouncePresence("hawas","gamed");
	return 0;
}