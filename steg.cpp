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
void steg::embedinpic(string name, string username, int count)
{
  string countfile = "countfile.txt";
  ofstream o;
  o.open(countfile);
  o<<username<<" "<<count;
  o.close();

  string command = "steghide embed -ef " + countfile + " -cf " + name + " -p 0 -f";
  system((char*)(command.c_str()));

}

void steg::reembed(string name, string username, int count)
{
  string outname = "hidden_"+name;
  embedinpic(outname,username,count);

  string command = "steghide embed -ef " + outname + " -cf " + name + " -p 0 -f";
  system((char *)(command.c_str()));
}

void steg::embedindefault(string name, string username, int count)
{
  embedinpic(name, username, count);

  string command = "steghide embed -ef " + name + " -cf default.jpg -p 0 -f";
  system((char*)(command.c_str()));

}

int steg::extractcount(string name, string username)
{
  string countfile = "countfile.txt";
  string command =  "steghide extract -sf " + name + " -xf " + countfile + " -p 0 -f";
  system((char*)(command.c_str()));

  int pos;
  int count;
  string line;
  ifstream input;
  input.open(countfile);

  while(getline(input, line) && line.find(username)==-1);
  pos = line.find(' ');
  count = stoi(line.substr(pos + 1));

  input.close();
  remove("countfile.txt");

  return count;

}

bool steg::extractfromdefault(string extname, string username)
{
  string outname = "hidden_"+extname;
  string command =  "steghide extract -sf " + extname + " -xf " + outname + " -p 0 -f";
  system((char*)(command.c_str()));

  int extracted_count = extractcount(outname, username);
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

int steg::extractcountfromdefault(string extname, string username)
{
  string outname = "hidden_"+extname;
  string command =  "steghide extract -sf " + extname + " -xf " + outname + " -p 0 -f";
  system((char*)(command.c_str()));
  int c = extractcount(outname, username);
  remove(("/home/karim/Desktop/build-DIST-Desktop_Qt_5_11_2_GCC_64bit-Debug/"+outname).c_str());
  return c;
}

string steg::viewpic(string name, string username)
{
  string outname = "hidden_"+name;
  string command =  "steghide extract -sf " + name + " -xf " + outname + " -p 0 -f";
  system((char*)(command.c_str()));
  int extracted_count = extractcount(outname, username);
  cout << extracted_count << '\n';
  if(extracted_count > 0)
  {
//    cout << extracted_count << '\n';
//    string comm = "xdg-open " + outname;
//    system((char * )(comm.c_str()));
    reembed(name, username, extracted_count-1);
   // extractcount(name,username);
    return outname;
  }
  else {
    int s=remove(outname.c_str());
    return("Unauthorized");
  }

}

void steg::SetCounter(string name, string username, int newcount)
{
    string outname = "hidden_"+name;
    string command0 =  "steghide extract -sf " + name + " -xf " + outname + " -p 0 -f";
    system((char*)(command0.c_str()));

    string countfile = "countfile.txt";
    ofstream o;
    o.open(countfile);
    o.clear();
    o<<username<<" "<<newcount;
    o.close();
    cout << "EXTRACTED HIDDEN FILE + " << newcount << endl;
    string command = "steghide embed -ef " + countfile + " -cf " + outname + " -p 0 -f";
    system((char*)(command.c_str()));
    cout << "EMBEDDED TEXT IN HIDDEN + "<< outname << endl;
    string command2 = "steghide embed -ef " + outname + " -cf " + name + " -p 0 -f";
    system((char*)(command2.c_str()));
    cout << "Embedded Hidden in Default + " << name << endl;
    remove("countfile.txt");
    remove(("/home/karim/Desktop/build-DIST-Desktop_Qt_5_11_2_GCC_64bit-Debug/"+outname).c_str());
}
