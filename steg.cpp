#include<iostream>
#include<string>
#include<cstring>
#include <fstream>
#include "steg.h"
#include<stdio.h>

using namespace std;

steg::steg()
{

}
void steg::embedinpic(string name, string username, int count,string prefix)
{
  string countfile = "countfile.txt";
  ofstream o;
  o.open(prefix+countfile);
  o<<username<<" "<<count;
  o.close();

  string command = "steghide embed -ef " + prefix+ countfile + " -cf " + prefix+ name + " -p 0 -f";
  system((char*)(command.c_str()));


}

void steg::reembed(string name, string username, int count,string prefix)
{
  string outname = "hidden_"+name;
  embedinpic(outname,username,count,prefix);

  string command = "steghide embed -ef " + prefix+ outname + " -cf " + prefix + name + " -p 0 -f";
  system((char *)(command.c_str()));
}

void steg::embedindefault(string name, string username, int count)
{
  embedinpic(name, username, count,"");

  string command = "steghide embed -ef " + name + " -cf default.jpg -p 0 -f";
  system((char*)(command.c_str()));

}

int steg::extractcount(string name, string username,string prefix)
{
  string countfile = "countfile.txt";
  string command =  "steghide extract -sf " + prefix + name + " -xf " + prefix + countfile + " -p 0 -f";
  system((char*)(command.c_str()));

  int pos;
  int count;
  string line;
  ifstream input;
  input.open(prefix+countfile);

  while(getline(input, line) && line.find(username)==-1);
  pos = line.find(' ');
  count = stoi(line.substr(pos + 1));

  input.close();
  remove("countfile.txt");

  return count;

}

bool steg::extractfromdefault(string extname, string username,string prefix)
{
  string outname = "hidden_"+extname;
  string command =  "steghide extract -sf " + extname + " -xf " + outname + " -p 0 -f";
  system((char*)(command.c_str()));

  int extracted_count = extractcount(outname, username,prefix);
  if(extracted_count > 0)
    {
      cout << "Extracted count from default : " << extracted_count << '\n';
      return true;
    }
  else
    {
      cout << "Access Denied " << endl;
      return false;
    }

}

int steg::extractcountfromdefault(string extname, string username,string prefix)
{
  string outname = "hidden_"+extname;
  string command =  "steghide extract -sf " + prefix+ extname + " -xf " + prefix+outname + " -p 0 -f";
  system((char*)(command.c_str()));
  int c = extractcount(outname, username,prefix);
  remove((prefix+outname).c_str());
  return c;
}

string steg::viewpic(string name, string username,string prefix)
{
  string outname = "hidden_"+name;
  string command =  "steghide extract -sf " + prefix + name + " -xf " + prefix +outname + " -p 0 -f";
  system((char*)(command.c_str()));
  int extracted_count = extractcount(outname, username,prefix);
  cout << extracted_count << '\n';
  if(extracted_count > 0)
  {
//    cout << extracted_count << '\n';
//    string comm = "xdg-open " + outname;
//    system((char * )(comm.c_str()));
    reembed(name, username, extracted_count-1,prefix);
   // extractcount(name,username);
    remove((prefix+outname).c_str());
    remove((prefix+"countfile.txt").c_str());
    return outname;
  }
  else {
      remove((prefix+outname).c_str());
      remove((prefix+"countfile.txt").c_str());
    return("Unauthorized");
  }


}

void steg::SetCounter(string name, string username, int newcount,string prefix)
{
    string outname = "hidden_"+name;
    string command0 =  "steghide extract -sf " + prefix+ name + " -xf " + prefix+outname + " -p 0 -f";
    system((char*)(command0.c_str()));

    string countfile = "countfile.txt";
    ofstream o;
    o.open(prefix+countfile);
    o.clear();
    o<<username<<" "<<newcount;
    o.close();
    cout << "EXTRACTED HIDDEN FILE + " << newcount << endl;
    string command = "steghide embed -ef " + prefix+ countfile + " -cf " + prefix+ outname + " -p 0 -f";
    system((char*)(command.c_str()));
    cout << "EMBEDDED TEXT IN HIDDEN + "<< outname << endl;
    string command2 = "steghide embed -ef " + prefix+ outname + " -cf " + prefix +name + " -p 0 -f";
    system((char*)(command2.c_str()));
    cout << "Embedded Hidden in Default + " << name << endl;
    remove((prefix+"countfile.txt").c_str());
    remove((prefix+outname).c_str());
}
