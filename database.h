#include"datastream.h"
#include"Hash.h"
#include"functions.h"


class Database
{
public:
	Datastream ds;
	Hash hash;
	string name;
	string db_search(string key);
	bool db_insert(string key, string data);
	bool db_replace(string key, string data);
	void db_store(string key, string data);
	bool db_delete(string key);	
	
	COUNT delok = 0;
	COUNT delerr = 0;
	COUNT seaok = 0;
	COUNT seaerr = 0;
	COUNT insok = 0;
	COUNT inserr = 0;
	COUNT repok = 0;
	COUNT reperr = 0;

private:	
	bool db_find(string &key);
	bool db_findfree(int keylen, int datlen);
};



Database* db_open(string name);
Database* db_new(string name);
void db_close(Database* db);




