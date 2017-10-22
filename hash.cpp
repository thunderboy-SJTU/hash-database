#include"Hash.h"
#include"datastream.h"

const size_t DE_HASH_SIZE = 1999993;

size_t hash_size = DE_HASH_SIZE;

COUNT Hash::hash(string &key)
{
	COUNT hval = 5381;
	for (int i = 1; i <= key.length(); i++)
		hval += (hval * 33) + key[i - 1];
	return hval % hash_size;
}

off_t Hash::cal_chainoff(string &key)
{
	chainoff = ((hash(key)) + 1)*sizeof(off_t);
	return chainoff;
}


void Hash::make_hashtable(string &name)
{
	ofstream ofs(name + ".idx",ios::binary);
	off_t temp_ptr = 0;
	for (int i = 0; i < hash_size + 1; i++)
		ofs.write((char*)(&temp_ptr), sizeof(off_t));
}