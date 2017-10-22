#include<iostream>
#include<string>
#include<time.h>
#include<sstream>
#include<vector>
using namespace std;

unsigned int random(int low, int high);

string change(off_t number);
off_t change(string &str);

string make_key();
string make_value(int namesize = 5);

