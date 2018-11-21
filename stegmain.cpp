#include "steg.h"
#include <string>
using namespace std;

int main()
{
  string name = "dog.jpg";
  string username = "yamao";
  int count = 5;
  int extracted_count;

  steg stig;


  //stig.embedindefault(name, username, count);
  //stig.viewpic(name, username)
  //stig.extractfromdefault("dog.jpg", username);
  //stig.SetCounter(name,username,207);

  return 0;
}
