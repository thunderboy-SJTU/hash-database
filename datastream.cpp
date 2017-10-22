#include"datastream.h"
#include"functions.h"

const off_t FREE_OFF = 0;
const COUNT DE_AUTO_INPUT_SIZE = 400000;
COUNT auto_input_size = DE_AUTO_INPUT_SIZE;


void Datastream::create_stream(string &name, bool exist)
{
	if (exist == false)
	{
		ofstream ofs(name + ".idx");
		ofstream ofs2(name + ".dat");
	}
	fs_idx.open(name + ".idx",ios::in|ios::out|ios::binary);
	fs_dat.open(name + ".dat",ios::in|ios::out|ios::binary);
}
string Datastream::read_dat()
{
	fs_dat.seekg(datoff, ios::beg);
	read(fs_dat, datbuf, datlen);
	return datbuf;
}

off_t Datastream::read_idx(off_t offset)
{
	fs_idx.seekg(offset, ios::beg);
	idxoff = fs_idx.tellg();
	fs_idx.read((char*)(&ptrval), sizeof(off_t));
	fs_idx.read((char*)(&keylen), sizeof(short));
	read(fs_idx, idxbuf, keylen);
	fs_idx.read((char*)(&datoff), sizeof(off_t));
	fs_idx.read((char*)(&datlen), sizeof(short));
	return ptrval;
}

off_t Datastream::read_ptr(off_t offset)
{
	off_t ptr;
	fs_idx.seekg(offset, ios::beg);
	fs_idx.read((char*)(&ptr), sizeof(off_t));
	return ptr;
}

void Datastream::write_dat(string &data, off_t offset, ios::seek_dir whence)
{
	if (offset == 0 && whence == ios::end)
	{
		datoff = tempdatoff + datalength;
		tempdata += data;
		datlen = data.length();
		datalength += datlen;
		if (datalength > auto_input_size)
			write_all();
	}
	else
	{
		fs_dat.seekp(offset, whence);
		datoff = fs_dat.tellp();
		datlen = data.length();
		write(fs_dat, data);
	}
}

void Datastream::write_idx(string&key, off_t offset, ios::seek_dir whence, off_t ptrval)
{
	this->ptrval = ptrval;
	idxbuf = key;
	fs_idx.seekp(offset, whence);;
	idxoff = fs_idx.tellp();
	keylen = key.size();
	fs_idx.write((char*)(&ptrval), sizeof(off_t));
	fs_idx.write((char*)(&keylen), sizeof(short));
	write(fs_idx, key);
	fs_idx.write((char*)(&datoff), sizeof(off_t));
	fs_idx.write((char*)(&datlen), sizeof(short));
}

void Datastream::write_ptr(off_t offset, off_t ptrval)
{
	fs_idx.seekp(offset, ios::beg);
	fs_idx.write((char*)(&ptrval), sizeof(off_t));
}

void Datastream::write_all()
{
	if (datalength > 0)
	{
		fs_dat.seekp(0, ios::end);
		write(fs_dat, tempdata);
		tempdatoff = fs_dat.tellp();
		tempdata = "";
		datalength = 0;
	}
}


void Datastream::remove()
{
	write_all();
	off_t freeptr = read_ptr(FREE_OFF);
	fs_idx.seekp(idxoff,ios::beg);;
	idxoff = fs_idx.tellp();
	fs_idx.write((char*)(&ptrval), sizeof(off_t));
	write_ptr(FREE_OFF, idxoff);
	write_ptr(ptroff, ptrval);
}




