#include"random.h"

static void initRandomSeed() {
	static bool initialized = false;
	if (!initialized) {
		srand(int(time(NULL)));
		initialized = true;
	}
}

unsigned long ulrand(void) {
	return (
		(((unsigned long)rand() << 24) & 0xFF000000ul)
		| (((unsigned long)rand() << 12) & 0x00FFF000ul)
		| (((unsigned long)rand()) & 0x00000FFFul));
}

unsigned int random(int low, int high) {
	initRandomSeed();
	return (ulrand() % (high - low + 1)) + low;
}


string change(off_t number)
{
	stringstream ss;
	string str;
	ss << number;
	ss >> str;
	return str;
}

off_t change(string &str)
{
	stringstream ss;
	off_t number;
	ss << str;
	ss >> number;
	return number;
}

string make_key()
{
	string key = "";
	off_t tempkey = random(10000000, 99999999);
	key = change(tempkey);
	return key;
}

string make_value(int namesize)
{
	string str = "";
	for (int i = 0; i < namesize; i++)
	{
		int number = random(97, 122);
		char ch = number;
		str += ch;
	}
	int math = random(0, 100);
	int english = random(0, 100);
	int cs = random(0, 100);
	str = str + " " + change(math) + " " + change(english) + " " + change(cs);
	return str;
}



