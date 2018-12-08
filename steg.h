#ifndef STEG_H
#define STEG_H

#include<iostream>
#include<string>
#include<cstring>
#include <fstream>
#include<string>
using namespace std;

class steg
{
public:
steg();
void embedinpic(string name, string username, int count,string prefix);
void embedindefault(string name, string username, int count);
int extractcount(string name, string username,string prefix);
void reembed(string name, string username, int count,string prefix);
bool extractfromdefault(string extname, string username,string prefix);
string viewpic(string name, string username,string prefix);
void SetCounter(string name, string username, int newcount,string prefix);
int extractcountfromdefault(string extname, string username,string prefix);
};
#endif //STEG_H
