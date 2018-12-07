
#include "ServiceDirectory.h"
using namespace std;

#define sendsize 1000000

ServiceDirectory::ServiceDirectory(int myaddr,int myport)
{
	UDPSSocket=new UDPServerSocket;
	UDPCSocket=new UDPClientSocket;
	myAdd=myaddr;
	myPort=myport;
	UDPSSocket->initializeServer(myaddr,myport);
    int x;
	UDPCSocket->initializeClient(myaddr,0,myport,x);
	string ln;
	string username,password;
	authTableIn.open("authtable.txt");

	if(!authTableIn)
		cout << "mafata7sh\n";

	while(authTableIn >> username >> password)
	{
		users[username]=password;
	}

	authTableIn.close();
}

bool ServiceDirectory::Register(string username, string password)
{
	if(users.find(username)==users.end())
	{
			users[username]=password;
			cout << "Registered: " << username << " " << password << endl;
			authTableOut.open("authtable.txt");

			if(!authTableOut)
				cout << "mafata7sh\n";

			for(auto it: users)
			{
				authTableOut << it.first << " " << it.second << "\n";
			}

			authTableOut.close();
			return 1;
	}
	else
		{
			cout << "Username already exists\n";
			return 0;
		}
}

void ServiceDirectory::Listen()
{
	while(true)
	{
		struct sockaddr_in client;
		bool flag = true;
		string rec=UDPSSocket->readFromSocketWithTimeout(90,client,flag);
		if(!flag)
			continue;
		Message me((char *)(rec.c_str()));
		rec=me.demarshal();

		string op=rec.substr(0,rec.find(' '));
		int opi=stoi(op);
		cout << opi << endl;

		rec.erase(0,rec.find(' ')+1);

		string rpci=rec.substr(0,rec.find(' '));
		int rpcid=stoi(rpci);
		rec.erase(0,rec.find(' ')+1);

		UDPSSocket->changepeerip(client);

		if(opi==0) //Register
		{
			string username=rec.substr(0,rec.find(' '));
			rec.erase(0,rec.find(' ')+1);
			string password=rec.substr(0,rec.length());
			if(Register(username,password))
			{
				string s="1";
				Message reg(0,(char * )(s.c_str()),s.length(),rpcid,Up);
				string s_marsh=reg.marshal(2+2+s.length());
				int amountsent=UDPSSocket->writeToSocket(s_marsh,sendsize);
			}
			else
			{
				string s="0";
				Message reg(0,(char * )(s.c_str()),s.length(),rpcid,Up);
				string s_marsh=reg.marshal(2+2+s.length());
				int amountsent=UDPSSocket->writeToSocket(s_marsh,sendsize);
			}

		}
		else if(opi==1) //Authenticate for login
		{

			string username=rec.substr(0,rec.find(' '));
			rec.erase(0,rec.find(' ')+1);
			string password=rec.substr(0,rec.find(' '));
			rec.erase(0,rec.find(' ')+1);
			int serverport=stoi(rec);
			if(Authenticate(username,password,client,serverport))
			{
				string s="1";
				Message auth(1,(char * )(s.c_str()),s.length(),rpcid,Up);
				string s_marsh=auth.marshal(2+2+s.length());
				int amountsent=UDPSSocket->writeToSocket(s_marsh,sendsize);

				string onlineusers="";
				for(auto it: user_addresses)
				{
					onlineusers.append(it.first);
					onlineusers.append(" ");
				}
				Message onusers(1,(char *)(onlineusers.c_str()),onlineusers.length(),rpcid,Up);
				string onusers_marsh = onusers.marshal(2+2+onlineusers.length());
				amountsent = UDPSSocket->writeToSocket(onusers_marsh,sendsize);
			}
			else
			{
				string s="0";
				Message auth(1,(char * )(s.c_str()),s.length(),rpcid,Up);
				string s_marsh=auth.marshal(2+2+s.length());
				int amountsent=UDPSSocket->writeToSocket(s_marsh,sendsize);
			}
		}
		else if(opi==2) //Request address of certain user
		{
			string username=rec.substr(0,rec.length());
			struct sockaddr_in peerip=user_addresses[username];
			string ip= to_string(peerip.sin_addr.s_addr) + " " + to_string(peerip.sin_port);
			Message rep(2,(char *)(ip.c_str()),ip.length(),rpcid, Reply);
			string rep_marsh=rep.marshal(2+2+ip.length());
			int amountsent=UDPSSocket->writeToSocket(rep_marsh,sendsize);
		}
        else if(opi==6)
                {
                    string user=rec;
                    string ping="ping";
                    for(auto it : user_addresses)
                    {
                        if(user!=it.first)
                        {
                            Message m(6,(char *)(ping.c_str()),ping.length(),0,Up);
                            string l=m.marshal(4+ping.length());
                            UDPSSocket->changepeerip(it.second);
                            int amountsent=UDPSSocket->writeToSocket(l,sendsize);
                            bool x;
                            string s=UDPSSocket->readFromSocketWithTimeout(100,x);
                            Message mm((char *)(s.c_str()));
                            string reply=mm.demarshal();
                            if(reply!="ok")
                            {
                                users.erase(it.first);
                            }
                        }
                    }
                }

	}
}


bool ServiceDirectory::Authenticate(string username, string password,struct sockaddr_in add,int serverport)
{
	if(users.find(username)!=users.end())
	{
		if(users[username]==password)
		{
			cout << username <<" "<< password << endl;
			cout << "Logged in\n";
			user_addresses[username].sin_family=AF_INET;
			user_addresses[username].sin_addr.s_addr=add.sin_addr.s_addr;
			user_addresses[username].sin_port=serverport;
			return true;
		}
	}
	return false;
}

ServiceDirectory::~ServiceDirectory()
{
	delete UDPSSocket;
	delete UDPCSocket;

	authTableOut.open("authtable.txt");

	if(!authTableOut)
		cout << "mafata7sh\n";

	for(auto it: users)
	{
		authTableOut << it.first << " " << it.second << "\n";
	}

	authTableOut.close();
}
