#include<string>
#include<fstream>
#include<iostream>
using namespace std;

typedef unsigned long COUNT;
extern const off_t FREE_OFF;
extern const COUNT DE_AUTO_INPUT_SIZE;
extern COUNT auto_input_size;

class Tempdata
{
public:
	string datbuf = "";
	string idxbuf = "";
	off_t idxoff = 0;
	short keylen = 0;
	off_t datoff = 0;
	short datlen = 0;
	off_t ptrval = 0;
	off_t ptroff = 0;
};



class Datastream:public Tempdata
{
public:
	void create_stream(string &name,bool exist);
	string read_dat();
	off_t read_idx(off_t offset);
	off_t read_ptr(off_t offset);
	void write_dat(string &data, off_t offset, ios::seek_dir whence);
	void write_idx(string &key, off_t offset, ios::seek_dir whence, off_t ptrval);
	void write_ptr(off_t offset, off_t ptrval);
	void remove();
	void write_all();
private:
	string tempdata = "";
	off_t tempdatoff = 0;
	off_t datalength = 0;
	

	fstream fs_idx;
	fstream fs_dat;
};

