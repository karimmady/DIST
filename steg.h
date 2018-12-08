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
void embedinpic(string name, string username, int count);
void embedindefault(string name, string username, int count);
int extractcount(string name, string username);
void reembed(string name, string username, int count);
bool extractfromdefault(string extname, string username);
string viewpic(string name, string username);
void SetCounter(string name, string username, int newcount);
int extractcountfromdefault(string extname, string username);
};
#endif //STEG_H
