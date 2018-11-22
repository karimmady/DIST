#include "Peer.h"
#include <sstream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include "math.h"
#include "dirent.h"
#include "steg.h"
using namespace std;

#define sendsize 1000000
#define packsizep 60000
vector<string> Peer::read_directory(const string& name)
{
    vector<string> v;
    DIR* dirp = opendir(name.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        if(string(dp->d_name).find(".jpg") < string(dp->d_name).size())
            v.push_back(dp->d_name);
    }
    closedir(dirp);
    return v;
}

Peer::Peer(Peer &x)
{
    UDPCSocket=x.UDPCSocket;
    UDPSSocket=x.UDPSSocket;
    UDPSDSocket=x.UDPSDSocket;
    myinfo=x.myinfo;
    onlineusers=x.onlineusers;
    onlineuser_adds=x.onlineuser_adds;
    ImagesEachUser=x.ImagesEachUser;
    SentPictures=x.SentPictures;
    ReceivedPictures=x.ReceivedPictures;
    username=x.username;
    password=x.password;
}

vector<string> Peer::fragment(string s)
{
  vector<string> v;
  string temp = "";
  for(int i = 0 ; i < s.length(); i++)
    {
      temp += s[i];
      if(((i % 60000 == 0)&&(i != 0))||(i == s.length()-1))
      {
        v.push_back(temp);
        temp = "";
      }

    }
  cout << "RETURNING\n";
  return v;
}

Peer::Peer(int _myAddr,int serverAddr, int _port,int port2,bool &binded)
{
	UDPCSocket=new UDPClientSocket;
	UDPSSocket=new UDPServerSocket;
  UDPSDSocket=new UDPClientSocket;
	UDPCSocket->initializeClient(_myAddr,serverAddr, port2);
  int x;
  UDPSDSocket->initializeClient(_myAddr,serverAddr,_port,x);
	binded=UDPSSocket->initializeServer(_myAddr,_port);
  myinfo.sin_family=AF_INET;
  myinfo.sin_addr.s_addr=htonl(_myAddr);
  myinfo.sin_port=htons(_port);
  binded=true;

  // cout << htonl(_myAddr) << " " << htons(_port) << endl;
}

bool is_number(string s)
{
    cout << s << endl;
    cout << s.length() << endl;
    int i=0;
    while(i<s.length())
    {
      if(!isdigit(s[i]))
        return false;
      i++;
    }

    return true;
}


bool Peer::Register(string username,string password)
{
    int rpcid=0;
    string rpc=to_string(rpcid);
    string account=username + " " + password;
    Message registration(0,(char *)(account.c_str()),account.length(),rpcid,Up);
    string regmarsh=registration.marshal(2 + rpc.length()+ 1 +account.length());
    int amountsent=UDPSDSocket->writeToSocket(regmarsh,sendsize);
    string flag=UDPSDSocket->readFromSocketWithTimeout(10);
    Message reg((char *)(flag.c_str()));
    string flag_demarsh=reg.demarshal();
    cout << flag_demarsh << endl;

    flag_demarsh.erase(0,flag_demarsh.find(' ')+1);
    flag_demarsh.erase(0,flag_demarsh.find(' ')+1);
    int fldem;
    if(!is_number(flag_demarsh))
      return 0;
    else
      fldem=stoi(flag_demarsh);
    return fldem;
}

struct sockaddr_in Peer::ReqAdd(string username)
{
    struct sockaddr_in peedr;

    int rpcid=0;
    string rpc=to_string(rpcid);
    string un=username;
    Message rq(2,(char *)(un.c_str()),un.length(),rpcid,Up);
    string rq_marsh=rq.marshal(2+rpc.length()+1+un.length());
    int amountsent=UDPSDSocket->writeToSocket(rq_marsh,sendsize);
    string ip=UDPSDSocket->readFromSocketWithTimeout(10);
    Message addr((char *)(ip.c_str()));
    string addr_demarsh=addr.demarshal();

    addr_demarsh.erase(0,addr_demarsh.find(' ')+1);
    addr_demarsh.erase(0,addr_demarsh.find(' ')+1);

    peedr.sin_family=AF_INET;
    peedr.sin_addr.s_addr=stol(addr_demarsh.substr(0,addr_demarsh.find(' ')));
    addr_demarsh.erase(0,addr_demarsh.find(' ')+1);
    if(is_number(addr_demarsh))
      peedr.sin_port=stoi(addr_demarsh);

    return peedr;
}

bool Peer::AnnouncePresence(string user,string passw)
{
    int rpcid=0;
    string rpc=to_string(rpcid);
    username = user;
    password=passw;
    string account = username + " " + password + " " +to_string(myinfo.sin_port);
    cout << account << endl;
    Message announcement(1,(char *)(account.c_str()),account.length(),rpcid,Up);
    string announ=announcement.marshal(2+rpc.length()+1+account.length());
    int amountsent=UDPSDSocket->writeToSocket(announ,sendsize);

    string flag=UDPSDSocket->readFromSocketWithTimeout(10);
    Message auth((char *)(flag.c_str()));
    string flag_demarsh=auth.demarshal();
    cout << flag_demarsh << endl;


    flag_demarsh.erase(0,flag_demarsh.find(' ')+1);
    flag_demarsh.erase(0,flag_demarsh.find(' ')+1);
    int fldem;
    if(is_number(flag_demarsh))
      fldem=stoi(flag_demarsh);
    else
      return 0;
    if(fldem)
    {
      cout << "Logged in\n";
      string onusers=UDPSDSocket->readFromSocketWithTimeout(10);
      Message onuser((char *)(onusers.c_str()));
      string onusers_demarsh=onuser.demarshal();

      onusers_demarsh.erase(0,onusers_demarsh.find(' ')+1);
      onusers_demarsh.erase(0,onusers_demarsh.find(' ')+1);


      while(onusers_demarsh.length()!=0)
      {
        onlineusers.push_back(onusers_demarsh.substr(0,onusers_demarsh.find(' ')));
        onusers_demarsh.erase(0,onusers_demarsh.find(' ')+1);

      }

      for (int i=0;i<onlineusers.size();i++)
      {
        struct sockaddr_in peedr=ReqAdd(onlineusers[i]);
        onlineuser_adds[onlineusers[i]]=peedr;
      }
    }
    return fldem;
}

//Inquiring about Pictures at certain user
void Peer::Inquire(string requser)
{
  onlineuser_adds[requser] = ReqAdd(requser);
  if(onlineuser_adds.find(requser) != onlineuser_adds.end())
  {
    UDPCSocket->changepeerip(onlineuser_adds[requser]);
    string s = username;
  	Message inq(0,(char *)(s.c_str()),s.length(),0,Inquiry);
  	string marshs = inq.marshal(4+s.length());
  	int amountrep = UDPCSocket->writeToSocket(marshs,sendsize);
  	string inquired;
  	cout << "Start of Inquiry\n";
    //Watch for packet drop /*******************************************
    vector<string> v;
    do {
  		inquired=UDPCSocket->readFromSocketWithTimeout(90);
  		Message receivedinq((char*)(inquired.c_str()));
  		inquired=receivedinq.demarshal();
      if(inquired != "eof")
  		 {
         v.push_back(inquired);
        cout << inquired << endl;
       }
  	} while(inquired != "eof");
    ImagesEachUser[requser] = v;
    UDPCSocket->changepeerip(onlineuser_adds[requser]);
  	cout << "End of Inquiry\n";
  }

}

int Peer::ViewCount(string requser,string filename)
{
  if(onlineuser_adds.find(requser) != onlineuser_adds.end())
  {
    UDPCSocket->changepeerip(onlineuser_adds[requser]);
    string m = requser + " " + filename;
    Message request(3,(char *)(m.c_str()),m.length(),0,Request);
    int amountreq = -1;
    string marshalled_message=request.marshal(4+m.length());
    amountreq=UDPCSocket->writeToSocket(marshalled_message,sendsize);
    bool flag = false;
    string remaincount = UDPCSocket->readFromSocketWithTimeout(30,flag);
    Message Rc((char*)(remaincount.c_str()));
    string remain = Rc.demarshal();
    if(is_number(remain))
    {
      cout << "Current Count : " << stoi(remain) << endl;
      return stoi(remain);
    }
    else
      return 0;
  }
  else
  cout<< "User is not online " << endl;
}

map <string,struct sockaddr_in> Peer::CheckOnlineFirst()
{
 return onlineuser_adds;
}
map< pair <string,string>, int > Peer::CheckSentPictures()
{
  return SentPictures;
}

map< pair <string,string>, string > Peer::CheckReceievedPictures()
{
    return ReceivedPictures;
}
void Peer::ViewPicture(string filename)
{
  steg stig;
  stig.viewpic(filename,username);

}
bool Peer::ControlAccess(string requser, string filename, string count)
{
  steg stig;
  if(onlineuser_adds.find(requser) != onlineuser_adds.end())
  {
    UDPCSocket->changepeerip(onlineuser_adds[requser]);
    string m = requser + " " + filename + " " + count;
    Message request(2,(char *)(m.c_str()),m.length(),0,Request);
    int amountreq = -1;
    string marshalled_message=request.marshal(4+m.length());
    amountreq=UDPCSocket->writeToSocket(marshalled_message,sendsize);

    cout << m << endl;
    bool flag = true;
    cout << "control access\n";
    do
    {
      string ack = UDPCSocket->readFromSocketWithTimeout(30,flag);
      cout << "flag received is " << ack << endl;
      Message acknow((char*)(ack.c_str()));
      string ack_demarshal = acknow.demarshal();
      cout << ack_demarshal << endl;
    }while(!flag);

    UDPCSocket->changepeerip(onlineuser_adds[requser]);
    return 1;
  }

  cout << "User Is not Online " << endl;
  return 0;
}

// Request Picture from username x with filename y
int Peer::req(string requser,string filename,string views)
{
  steg stig;
  if(onlineuser_adds.find(requser) != onlineuser_adds.end())
  {
    UDPCSocket->changepeerip(onlineuser_adds[requser]);
    string msg = username + " " + filename + " " + views;
    Message request(1,(char *)(msg.c_str()),msg.length(),0,Request);
    int amountreq = -1;
    string marshalled_message=request.marshal(4+msg.length());
    amountreq=UDPCSocket->writeToSocket(marshalled_message,sendsize);
    cout << "req\n";
    string packets = UDPCSocket->readFromSocketWithTimeout(90);
    Message receivedpacks((char*)(packets.c_str()));
    string no_of_packs = receivedpacks.demarshal();
    if(no_of_packs != "Image not found\n")
    {
        string imgm = "";
        string img = "";
        int no_of_packets;
        cout <<"no of packs "<< no_of_packs << endl;
        cout << "is number "<< is_number(no_of_packs) << endl;
        if(is_number(no_of_packs))
          no_of_packets = stoi(no_of_packs);
        else
          return 0;
        int i = 0;
        bool flag;
        while(true)
        {
          flag = true;
          string packno;
          cout << "READ " << i << endl;
          string temp = UDPCSocket->readFromSocketWithTimeout(2,flag);
          if(flag)
            {
              imgm += temp;
              packno=to_string(i);
              Message PacketsNo((char*)(packno.c_str()));
              string marshalledpacksno = PacketsNo.marshal(packno.length());
              cout << "Packet Number " << packno << endl;
              UDPCSocket->changepeerip(onlineuser_adds[requser]);
              int am = UDPCSocket->writeToSocket(marshalledpacksno ,sendsize);
              i++;
            }
            if(i >= no_of_packets)
              break;

          }

      Message receivedimg((char*)(imgm.c_str()));
      img = receivedimg.demarshal();

      ofstream fileo(filename,ios::binary);
      fileo.write(img.c_str(),img.length());
      cout << "Image Received with size " << img.length() << " bytes" << endl;
      pair<string,string> p1;
      p1 = make_pair(requser,filename);
      ReceivedPictures[p1] = views;
      UDPCSocket->changepeerip(onlineuser_adds[requser]);
      return 1;
    }
    else
      {
        cout << no_of_packs;
        return 0;
      }
  }
  cout << "User is not Online " << endl;
  return 0;
}


void Peer::InquiryReply(string msg,struct sockaddr_in client)
{
  vector<string> vec;
	string end = "eof";
  vec = read_directory(".");
  UDPSSocket->changepeerip(client);
	for(int i = 0; i<vec.size(); i++)
		{
      //Watch for Dropped Packets
			Message names((char *)(vec[i].c_str()));
			string marshname=names.marshal(vec[i].length());
			int amountrep=UDPSSocket->writeToSocket(marshname,sendsize);
		}
	Message endf((char *)(end.c_str()));
	string marshend = endf.marshal(end.length());
	int amountrep=UDPSSocket->writeToSocket(marshend,sendsize);
}


bool Peer::SendPicture(string recvs,struct sockaddr_in client)
{
  string uname = recvs.substr(0,recvs.find(' '));
  recvs.erase(0,recvs.find(' ')+1);
  string filename = recvs.substr(0,recvs.find(' '));
  recvs.erase(0,recvs.find(' ')+1);
  int views;
  if(is_number(recvs))
    views = stoi(recvs);
  else
    return 0;
  ifstream di;
  di.open(filename, ios::binary);

  UDPSSocket->changepeerip(client);

  if(!di)
  {
    cout << "Image not Found " << endl;
    string nf="Image not found\n";
    Message notfound((char *)(nf.c_str()));
    string marshnf=notfound.marshal(nf.length());
    int amountrep=UDPSSocket->writeToSocket(marshnf,sendsize);
  }
  else
  {
    cout << "filename :" << filename << endl;
    steg stig;
    stig.embedindefault(filename,uname,views);
    string def = "default.jpg";
    di.close();
    di.open(def,ios::binary);
    ostringstream oss;
    oss << di.rdbuf();
    string s(oss.str());

    Message picture((char*)(s.c_str()));
    int picbytes=-1;
    string marshalled_image=picture.marshal(s.length());

    int no_of_packets = ceil(marshalled_image.length()/60000.0);
    string packs=to_string(no_of_packets);
    Message packets((char *)(packs.c_str()));
    string marshalledpacks = packets.marshal(packs.length());
    int amountr = UDPSSocket->writeToSocket(marshalledpacks,sendsize);
    vector<string> vec;
    vec = fragment(marshalled_image);
    int i = 0;
    bool flag;
    while(true)
    {
      cout << "loop " << i << endl;
      flag = true;
      int amountrep = UDPSSocket->writeToSocket(vec[i],sendsize);

      string approved = UDPSSocket->readFromSocketWithTimeout(2,flag);
      UDPCSocket->changepeerip(client);
      Message packsnumber((char *)(approved.c_str()));
      string ack = packsnumber.demarshal();
      cout << "ACKNOWLEDGED " << ack << endl;
      if(is_number(ack))
      {
        if((flag)&&(stoi(ack) == i))
          i++;
        if(i > vec.size()-1)
          break;
      }
    }
    di.close();
    pair<string,string> p1;
    p1 = make_pair(uname,filename);   // making pair of uname/filename
    SentPictures[p1] = views;
  }

}
void Peer::ReturnNumOfViews(string msg, struct sockaddr_in client)
{
  string user_name;
  string file_name;
  steg stig;
  int pos= msg.find(" ");
  user_name=msg.substr(0,pos);
  file_name= msg.substr(pos+1, msg.length());
  int viewcount = stig.extractcountfromdefault(file_name,user_name);
  cout << "Extracted Count " << viewcount << endl;
  string v=to_string(viewcount);
  //sprintf(v,"%d",viewcount);
  Message countv((char *)(v.c_str()));
  string marshc=countv.marshal(v.length());
  UDPSSocket->changepeerip(client);
  int amountrep=UDPSSocket->writeToSocket(marshc,sendsize);
}
void Peer::UpdateCount (string msg, struct sockaddr_in client)
{
  string user_name;
  string file_name;
  int count;
  steg stig;
  user_name = msg.substr(0,msg.find(' '));
  msg.erase(0,msg.find(' ')+1);
  file_name=msg.substr(0,msg.find(' '));
  msg.erase(0,msg.find(' ')+1);
  if(is_number(msg))
    count=stoi(msg);
  else
    return;
  stig.reembed(file_name,user_name,count);
  cout << " RE EMBEDDED " << endl;
  string ack="Views updated!\n";
  UDPSSocket->changepeerip(client);
  Message Acknowledge((char *)(ack.c_str()));
  string marshnf=Acknowledge.marshal(ack.length());
  int amountrep=UDPSSocket->writeToSocket(marshnf,sendsize);

}

void Peer::rec()
{
	while(true)
	{
    struct sockaddr_in client;
    bool flag = true;
    cout << "rec\n";
		string req=UDPSSocket->readFromSocketWithTimeout(90,client,flag);
    if(!flag)
      continue;
		Message receivedmessage((char *)(req.c_str()));
		string recvs=receivedmessage.demarshal();
    string recvs_opcode = recvs.substr(0,recvs.find(' '));
    if(recvs_opcode.length() != 1)
    {
      cout << "Incorrect OPCODE " << endl;
      continue;
    }
    int opcode;
    if(is_number(recvs_opcode))
      opcode=stoi(recvs_opcode);
    else
      continue;
    recvs.erase(0,recvs.find(' ')+1);
    string recvs_rpcid = recvs.substr(0,recvs.find(' '));
    if(recvs_opcode.length() > 2)
    {
      cout << "Incorrect RPCID " << endl;
      continue;
    }
    if(is_number(recvs_rpcid))
      int rpcid=stoi(recvs_rpcid);
    else
      continue;
    recvs.erase(0,recvs.find(' ')+1);

    if (opcode==0)
    {
      //Threaded Function
        InquiryReply(recvs,client);
    }
    else if(opcode==1)
    {
        cout << recvs << endl;
        SendPicture(recvs,client);
    }
    else if(opcode==2)
    {
      //REVOKE
       cout << recvs << endl;
        UpdateCount(recvs,client);
    }
    else if(opcode==3)
    {
      //Reply with No. Of Views of Certain Picture
        cout << recvs << endl;
        ReturnNumOfViews(recvs,client);
    }
	}
}

Peer::~Peer()
{
	delete UDPCSocket;
	delete UDPSSocket;
}
