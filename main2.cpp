#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include "Peer.h"
#include "Message.h"
#include "base64.h"
using namespace std;


int main(int argc,char** argv)
{
	char * po;
	int myip=inet_addr(argv[2]);
	int serverip=inet_addr(argv[3]);
	int port=strtol(argv[1],&po,10);
	bool flag;
	Peer ana(ntohl(myip),ntohl(serverip),port,flag);

	ana.Register("omar","ashraf");
	ana.AnnouncePresence("omar","ashraf");
	ana.rec();

	return 0;
}
