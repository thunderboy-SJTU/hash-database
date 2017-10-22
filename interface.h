#include"database.h"
#include"random.h"

int get_number(string markword = " < < ");
string get_string(string markword = " < < ");

void random_insert(size_t size, vector<string>&keys, vector<string>&values, Database* db);
void random_search(vector<string>&keys, Database* db);
void random_delete(vector<string>&keys, Database*db);
void random_replace(vector<string>&keys, vector<string>&values, Database*db, bool mode);

void test_delete(vector<string>&keys, Database* db);
void test_all(Database* db);
void test(Database*db);


void get_info(Database* db);
void reset_info(Database* db);

int find_prime_number(size_t datasize);
void setting();

void open_menu(Database* db);
void main_interface();

