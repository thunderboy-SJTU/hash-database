#include<string>
using namespace std;

typedef unsigned long COUNT;
extern const size_t DE_HASH_SIZE;

extern size_t hash_size;



class Hash
{
public:
	off_t cal_chainoff(string &key);
	void make_hashtable(string &name);
	off_t chainoff = 0;
private:
	COUNT hash(string &key);
};


