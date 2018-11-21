#include "ServiceDirectory.h"
#include <iostream>
using namespace std;

int main(int argc, char ** argv)
{
	char * po;
	int myip=inet_addr(argv[2]);
	int port=strtol(argv[1],&po,10);

	ServiceDirectory SD(ntohl(myip),port);
	SD.Listen();
	SD.Listen();
	SD.Listen();
	return 0;
}