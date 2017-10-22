#include"database.h"


Database* db_open(string name)
{
	Database* db = new Database;
	string idxname = name + ".idx";
	string datname = name + ".dat";
	
	if ((is_file_exist(idxname) && is_file_exist(datname)) == true)
	{
		db->ds.create_stream(name, true);
		db->name = name;
	}
	else
		return NULL;
	return db;
}

Database* db_new(string name)
{
	Database* db = new Database;
	string idxname = name + ".idx";
	string datname = name + ".dat";
	if ((is_file_exist(idxname) || is_file_exist(datname)) == false)
	{
		db->ds.create_stream(name, false);
		db->hash.make_hashtable(name);
		db->name = name;
		return db;
	}
	else
		return NULL;
}

void  db_close(Database* db) 
{
	db->ds.write_all();
	delete db;
}

string Database::db_search(string key)
{
	string str;
	if (db_find(key) == false)
	{
		str = "";
		seaerr++;	
	}
	else
	{
		ds.write_all();
		str = ds.read_dat();
		seaok++;
	}
	return str;
}

bool Database::db_find(string &key)
{
	off_t offset, nextoffset;
	ds.ptroff = hash.cal_chainoff(key);
	offset = ds.read_ptr(ds.ptroff);
	while (offset != 0)
	{
		nextoffset = ds.read_idx(offset);
		if (key == ds.idxbuf)
			break;
		ds.ptroff = offset;
		offset = nextoffset;
	}
	if (offset == 0)
		return false;
	return true;	
}





bool Database::db_delete(string key)
{
	if (db_find(key) == true)
	{
		ds.remove();
		delok++;
		return true;
	}
	else
	{
		delerr++;
		return false;
	}
}

bool Database::db_insert(string key, string data)
{
	if (db_find(key) == false)
	{
		off_t ptrvalue = ds.read_ptr(hash.chainoff);
		int keylen = key.length();
 		int datlen = data.length();
		if (!db_findfree(keylen, datlen))
		{
			ds.write_dat(data, 0, ios::end);
			ds.write_idx(key, 0, ios::end, ptrvalue);
			ds.write_ptr(hash.chainoff, ds.idxoff);
		}
		else
		{
			ds.write_dat(data, ds.datoff, ios::beg);
			ds.write_idx(key, ds.idxoff, ios::beg, ptrvalue);
			ds.write_ptr(hash.chainoff, ds.idxoff);
		}
		insok++;
		return true;
	}
	inserr++;
	return false;
}

bool Database::db_replace(string key, string data)
{
	if (db_find(key) == true)
	{
		int datlen = data.length();
		if (datlen != ds.datlen)
		{
			ds.remove();
			off_t ptrvalue = ds.read_ptr(hash.chainoff);
			ds.write_dat(data, 0, ios::end);
			ds.write_idx(key, 0, ios::end, ptrvalue);
			ds.write_ptr(hash.chainoff, ds.idxoff);
		}
		else
		{
			ds.write_all();
			ds.write_dat(data, ds.datoff, ios::beg);
		}
		repok++;
		return true;
	}
	reperr++;
	return false;
}

void Database::db_store(string key, string data)
{
	if (db_find(key) == true)
		db_replace(key, data);
	else
		db_insert(key, data);
}

bool Database::db_findfree(int keylen, int datlen)
{
	off_t offset, nextoffset, saveoffset;
	saveoffset = FREE_OFF;
	offset = ds.read_ptr(saveoffset);
	while (offset != 0)
	{
		nextoffset = ds.read_idx(offset);
		if (keylen == ds.idxbuf.length() && datlen == ds.datlen)
			break;
		saveoffset = offset;
		offset = nextoffset;
	}
	if (offset == 0)
		return false;
	else
		ds.write_ptr(saveoffset, ds.ptrval);
	return true;
}



