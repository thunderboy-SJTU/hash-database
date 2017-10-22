#include<string>
#include<fstream>
using namespace std;


bool is_file_exist(string name);
bool isblank(string key);

bool read(fstream &fs, string &str, size_t length);
void write(fstream&fs, string &str);