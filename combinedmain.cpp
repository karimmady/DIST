#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include "Peer.h"
#include "Message.h"
#include "steg.h"
#include "base64.h"
#include <unistd.h>
#include <thread>
using namespace std;

void call_server(Peer &server)
{
	server.rec();
}
int main(int argc,char** argv)
{
  char * server_po;
  char * client_po;
	bool x;
  int myip=inet_addr(argv[3]);
	int directoryserviceip=inet_addr(argv[4]);
  int clientport=strtol(argv[2],&server_po,10);
  int serverport=strtol(argv[1],&client_po,10);

  Peer cspeer(ntohl(myip),ntohl(directoryserviceip),serverport,clientport,x);

	int regflag;
	cout << "Would you like to register ? (1:Yes , 0:No)" << endl;
	cin >> regflag;
	string u,p;
	cout << "Please enter username : ";
	cin >> u;
	cout << "Please enter password : ";
	cin >> p;
	if(regflag == 1)
			cspeer.Register(u,p);
	cspeer.AnnouncePresence(u,p);

  thread server;
  server = thread(call_server,ref(cspeer));
  server.detach();
	int z;

	string filename;
	string uname;
	string views;
	steg stig;

	while(true)
	{
		cout << "Please enter operation code :" << endl;
		cout << "0 - Inquire About Pictures at User." << endl;
		cout << "1 - Request Picture from User." << endl;
		cout << "2 - Change view count at certain user." << endl;
		cout << "3 - View current counter at certain user." << endl;
		cout << "4 - View Picture." << endl;
		cout << "5 - Terminate Program. " << endl;
		cin >> z;
		if(z == 0)							//Inquire
		{
			cspeer.CheckOnlineFirst();
			cout << "Please enter Username : " << endl;
			cin >> uname;
			cspeer.Inquire(uname);
		}
			else if(z == 1)       //Request Picture
		{
			cout << "Please enter file name " << endl;
			cin >> filename;
			cout << "Please enter Username " << endl;
			cin >> uname;
			cout << "Please Enter Number of Views " << endl;
			cin >> views;
			cspeer.req(uname,filename,views);
		}
		else if(z == 2) //Change Count
		{
			cout << "Received Pictures : " << endl;
			cspeer.CheckReceievedPictures();
			cout << "Sent Pictures : " << endl;
			cspeer.CheckSentPictures();
			cout << "*************************" << endl;
			cout << "Please enter file name " << endl;
			cin >> filename;
			cout << "Please enter Username " << endl;
			cin >> uname;
			cout << "Please Enter Number of Views " << endl;
			cin >> views;
			cspeer.ControlAccess(uname,filename,views);
		}
		else if(z == 3)  //View Count
		{
			cout << "Received Pictures : " << endl;
			cspeer.CheckReceievedPictures();
			cout << "Sent Pictures : " << endl;
			cspeer.CheckSentPictures();
			cout << "*************************" << endl;
			cout << "Please enter file name " << endl;
			cin >> filename;
			cout << "Please enter Username " << endl;
			cin >> uname;
			cspeer.ViewCount(uname,filename);
		}
		else if(z==4)
		{
			try{
				cout << "Please enter filename : " << endl;
				cin >> filename;
				cout << "Please enter username : " << endl;
				cin >> uname;
				stig.viewpic(filename,uname);
			}
			catch(exception &e)
			{	cout << e.what() << endl;}
		}
		else if(z==5)
		{
			return 1;
		}

	}
}
