#ifndef PEER_H
#define PEER_H

#include "UDPClientSocket.h"
#include "UDPServerSocket.h"
#include "Message.h"
#include <string.h>
#include <map>
#include <vector>
#include <fstream>
using namespace std;

class Peer
{
    private:
        UDPSocket * UDPCSocket;
        UDPSocket * UDPSSocket;
        UDPSocket * UDPSDSocket;
        struct sockaddr_in myinfo;
        vector<string> onlineusers;
        map <string,struct sockaddr_in> onlineuser_adds;
        map <string,vector<string> > ImagesEachUser;
        map< pair <string,string>, int > SentPictures;
        map< pair <string,string>, string > ReceivedPictures;
        map<string,string> uploadedpics;
        string username;
        string password;
    public:
		Peer(int _myAddr,int _peerAddr, int _port,int port2,bool& binded);
        Peer(Peer &);
        int remove(string filename);
        void refresh();
        int upload(string,string);
		vector<string> read_directory(const string& name);
		int ViewCount(string requser,string filename);
		void ReturnNumOfViews(string msg,struct sockaddr_in client);
		void UpdateCount (string msg, struct sockaddr_in client);
		vector<string> fragment(string);
		bool AnnouncePresence(string,string);
        map< pair <string,string>, string > CheckReceievedPictures();
        map< pair <string,string>, int > CheckSentPictures();
		bool Register(string,string);
		void ViewPicture(string filename);
		struct sockaddr_in ReqAdd(string);
        map <string,struct sockaddr_in> CheckOnlineFirst();
        vector<string> Inquire(string);
		void TerminateThread();
		bool SendPicture(string recvs,struct sockaddr_in client);
		void InquiryReply(string msg,struct sockaddr_in client);
		bool ControlAccess(string requser, string filename, string count);
		int req(string requser, string filename, string views);
		void rec();
		~Peer();
};
#endif // PEER_H
