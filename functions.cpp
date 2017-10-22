#include"functions.h"

bool is_file_exist(string name)
{
	ifstream ifs(name);
	if (!ifs)
		return false;
	return true;
}

bool isblank(string key)
{
	for (int i = 0; i < key.length(); i++)
		if (key[i] != ' ')
			return false;
	return true;
}


bool read(fstream &fs, string &str, size_t length)
{
	char ch;
	str = "";
	for (int i = 0; i <length; i++)
	{
		if (fs.get(ch))
			str += ch;
		else
			return false;
	}
	return true;
}

void write(fstream&fs, string &str)
{
	for (int i = 0; i < str.length(); i++)
		fs.put(str[i]);

}