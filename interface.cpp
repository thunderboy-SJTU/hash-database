#include"interface.h"

int get_number(string markword)
{
	int number;
	string str;
	while (true)
	{
		cout << markword;
		cin.sync();
		getline(cin, str);
		stringstream ss(str);
		if (ss.str() != "")
		{
			if (!(ss >> number))
				cout << "输入错误，请重输！\n" << endl;
			else
			{
				if ((ss >> std::ws).eof())
					break;
				else
					cout << "输入错误，请重输！\n" << endl;
			}
		}
		else
			cout << "输入错误，请重输！\n" << endl;
	}
	return number;
}

string get_string(string markword)
{
	string str;
	string str_2;
	while (true)
	{
		cout << markword;
		cin.sync();
		getline(cin, str);
		stringstream ss(str);
		if (ss.str() != "")
		{
			ss >> str_2;
			if ((ss >> std::ws).eof())
				break;
			else
				cout << "输入错误，请重输！\n" << endl;
		}
		else
			cout << "输入错误，请重输！\n" << endl;
	}
	return str_2;
}

void random_insert(size_t size, vector<string>&keys, vector<string>&values, Database* db)
{
	clock_t begin = clock();
	int old_size = keys.size();
	for (int i = 0; i < size; i++)
	{
		keys.push_back(make_key());
		values.push_back(make_value());
	}
	cout << "键、值已准备好！\n";
	clock_t prepared = clock();
	cout << "准备时间：" << (prepared - begin) / CLOCKS_PER_SEC << "s" << endl;
	for (int i = 0; i < size; i++)
		db->db_insert(keys[i + old_size], values[i]);
	clock_t end = clock();
	cout << "插入时间：" << (end - prepared) / CLOCKS_PER_SEC << "s" << endl;
}

void random_search(vector<string>&keys, Database* db)
{
	clock_t begin = clock();
	for (int i = 0; i < keys.size(); i++)
		db->db_search(keys[i]);
	clock_t end = clock();
	cout << "查找时间：" << (end - begin) / CLOCKS_PER_SEC << "s" << endl;
}

void random_delete(vector<string>&keys, Database*db)
{
	clock_t begin = clock();
	for (int i = 0; i < keys.size(); i++)
		db->db_delete(keys[i]);
	clock_t end = clock();
	cout << "删除时间：" << (end - begin) / CLOCKS_PER_SEC << "s" << endl;
}

void random_replace(vector<string>&keys, vector<string>&values, Database*db, bool mode)
{
	clock_t begin = clock();
	values.clear();
	for (int i = 0; i < keys.size(); i++)
	{
		if (mode == false)
			values.push_back(make_value());
		else
			values.push_back(make_value(6));
	}
	cout << "键、值已准备好！\n";
	clock_t prepared = clock();
	cout << "准备时间：" << (prepared - begin) / CLOCKS_PER_SEC << "s" << endl;
	for (int i = 0; i < keys.size(); i++)
		db->db_replace(keys[i], values[i]);
	clock_t end = clock();
	cout << "替换时间：" << (end - prepared) / CLOCKS_PER_SEC << "s" << endl;
}

void test_delete(vector<string>&keys, Database* db)
{
	int number = random(0, keys.size() - 1);
	db->db_delete(keys[number]);
	string temp = keys[number];
	keys[number] = keys[keys.size() - 1];
	keys[keys.size() - 1] = temp;
	keys.pop_back();
}

void test_all(Database* db)
{
	int size;
	bool count = true;
	vector<string>keys;
	vector<string>values;
	size = get_number("输入的数据总量：");
	clock_t begin = clock();
	for (int i = 0; i < size; i++)
	{
		keys.push_back(make_key());
		values.push_back(make_value());
	}
	clock_t prepared = clock();
	cout << "键、值已准备好！\n";
	cout << "准备时间：" << (prepared - begin) / CLOCKS_PER_SEC << "s" << endl;
	for (int i = 0; i < size; i++)
		db->db_insert(keys[i], values[i]);
	clock_t step1 = clock();
	cout << "插入时间：" << (step1 - prepared) / CLOCKS_PER_SEC << "s" << endl;
	for (int i = 0; i < size; i++)
		db->db_search(keys[i]);
	clock_t step2 = clock();
	cout << "查找时间：" << (step2 - step1) / CLOCKS_PER_SEC << "s" << endl;
	for (int i = 0; i < 5 * size; i++)
	{
		db->db_search(keys[random(0, keys.size() - 1)]);
		if (i % 37 == 0)
			test_delete(keys, db);
		if (i % 11 == 0)
		{
			string key = make_key();
			string value = make_value();
			db->db_insert(key, value);
			keys.push_back(key);
			db->db_search(key);
		}
		if (i % 17 == 0)
		{
			string key = keys[random(0, keys.size() - 1)];
			if (count == true)
			{
				db->db_replace(key, make_value());
				count = false;
			}
			else
			{
				db->db_replace(key, make_value(6));
				count = true;
			}
		}
	}
	clock_t step3 = clock();
	cout << "循环时间：" << (step3 - step2) / CLOCKS_PER_SEC << "s" << endl;
	while (!keys.empty())
	{
		test_delete(keys, db);
		if (!keys.empty())
		{
			for (int i = 0; i < 10; i++)
				db->db_search(keys[random(0, keys.size() - 1)]);
		}
	}
	clock_t step4 = clock();
	cout << "删除时间：" << (step4 - step3) / CLOCKS_PER_SEC << "s" << endl;
	cout << "共花费时间（不包括准备）：" << (step4 - prepared) / CLOCKS_PER_SEC << "s" << endl;
}

void test(Database*db)
{
	system("cls");
	cout << "当前打开的数据库为：" << db->name << endl;
	cout << "--------------------";
	for (int i = 0; i < db->name.size(); i++)
		cout << "-";
	cout << "\n\n";
	cout << "测试菜单\n";
	cout << "\n";
	cout << "1--随机插入数据\n";
	cout << "2--随机查找数据\n";
	cout << "3--随机替换数据（相同长度）\n";
	cout << "4--随机替换数据（更长长度）\n";
	cout << "5--随机删除数据\n";
	cout << "6--综合测试\n";
	cout << "7--统计信息\n";
	cout << "8--重置统计信息\n";
	cout << "9--返回上一级\n";
	int number;
	vector<string>keys;
	while (1)
	{
		cout << "\n";
		number = get_number();
		switch (number)
		{
		case(1) :
		{
			int size;
			vector<string>values;
			size = get_number("输入的数据总量：");
			random_insert(size, keys, values, db);
			break;
		}
		case(2) :
		{
			random_search(keys, db);
			break;
		}
		case(3) :
		{
			vector<string>values;
			random_replace(keys, values, db, 0);
			break;
		}
		case(4) :
		{
			vector<string>values;
			random_replace(keys, values, db, 1);
			break;
		}
		case(5) :
		{
			random_delete(keys, db);
			keys.clear();
			break;
		}
		case(6) :
		{
			test_all(db);
			break;
		}
		case(7) :
		{
			get_info(db);
			break;
		}
		case(8) :
		{
			reset_info(db);
			break;
		}
		case(9) :
		{
			break;
		}
		default:
		{
			cout << "输入错误，请重输！\n";
			break;
		}
		}
		if (number == 9)
			break;
	}
}

void get_info(Database* db)
{
	cout << "统计信息\n";
	cout << "插入成功：" << db->insok << endl;
	cout << "插入失败：" << db->inserr << endl;
	cout << "删除成功：" << db->delok << endl;
	cout << "删除失败：" << db->delerr << endl;
	cout << "替换成功：" << db->repok << endl;
	cout << "替换失败：" << db->reperr << endl;
	cout << "查找成功：" << db->seaok << endl;
	cout << "查找失败：" << db->seaerr << endl;
}

void reset_info(Database* db)
{
	db->insok = 0;
	db->inserr = 0;
	db->delok = 0;
	db->delerr = 0;
	db->repok = 0;
	db->reperr = 0;
	db->seaok = 0;
	db->seaerr = 0;
	cout << "重置成功！\n";
}

int find_prime_number(size_t datasize)
{
	vector<int>numbers;
	if (datasize <= 1)
		return 1;
	else
	{
		for (int i = 0; i < datasize + 1; i++)
			numbers.push_back(1);      
		numbers[0] = 0;                      
		numbers[1] = 0;               
		for (int j = 2; j < datasize + 1; j++)
			if (numbers[j] == 1 && j < sqrt(datasize)) 
				for (int times = 2; times*j <= datasize; times++)
					numbers[j*times] = 0;    
		for (int t = datasize; t > 0; t--)
			if (numbers[t] == 1)
				return t;
	}
}

void setting()
{
	system("cls");
	cout << "设置菜单\n";
	cout << "\n";
	cout << "1--Hash容量\n";
	cout << "2--根据数据量自动生成Hash容量\n";
	cout << "3--数据自动存入量\n";
	cout << "4--恢复默认设置\n";
	cout << "5--返回上一级\n";
	while (1)
	{
		cout << "\n";
		int number;
		number = get_number();
		switch (number)
		{
		case(1) :
		{
			int size;
			cout << "当前Hash的容量为：" << hash_size << endl;
			size = get_number("请输入你想修改的大小:");
			hash_size = size;
			cout << "修改成功！\n";
			break;
		}
		case(2) :
		{
			size_t datasize;
			cout << "当前Hash的容量为：" << hash_size << endl;
			datasize = get_number("请输入你想要输入的数据量：");
			hash_size = find_prime_number(2 * datasize);
			cout << "修改成功！\n";
			break;
		}
		case(3) :
		{
			int input_size;
			cout << "当前数据自动存入量为：" << auto_input_size << endl;
			input_size = get_number("请输入你想修改的值：");
			auto_input_size = input_size;
			cout << "修改成功！\n";
			break;
		}
		case(4) :
		{
			hash_size = DE_HASH_SIZE;
			auto_input_size = DE_AUTO_INPUT_SIZE;
			cout << "已恢复默认设置！\n";
			break;
		}
		case(5) :
			break;
		default:
		{
			cout << "输入错误，请重输！\n";
			break;
		}
		}
		if (number == 5)
			break;
	}
}

void open_menu(Database* db)
{
	while (1)
	{
		system("cls");
		cout << "当前打开的数据库为：" << db->name << endl;
		cout << "--------------------";
		for (int i = 0; i < db->name.size(); i++)
			cout << "-";
		cout << "\n\n";
		cout << "操作菜单\n";
		cout << "\n";
		cout << "1--添加数据\n";
		cout << "2--删除数据\n";
		cout << "3--修改数据\n";
		cout << "4--查询\n";
		cout << "5--统计信息\n";
		cout << "6--重置统计信息\n";
		cout << "7--性能测试\n";
		cout << "8--关闭数据库\n";
		int number;
		while (1)
		{
			cout << "\n";
			number = get_number();
			switch (number)
			{
			case(1) :
			{
				string ID;
				string name;
				int math;
				int english;
				int cs;
				string value;
				ID = get_string("请输入学号：");
				name = get_string("请输入姓名：");
				math = get_number("请输入数学成绩：");
				english = get_number("请输入英语成绩：");
				cs = get_number("请输入计算机成绩：");
				value = name + " " + change(math) + " " + change(english) + " " + change(cs);
				if (db->db_insert(ID, value))
					cout << "插入成功！\n";
				else
					cout << "学号已存在！\n";
				break;
			}
			case(2) :
			{
				string ID;
				ID = get_string("请输入学号：");
				if (db->db_delete(ID))
					cout << "删除成功！\n";
				else
					cout << "学号不存在！\n";
				break;
			}
			case(3) :
			{
				string ID;
				string name;
				int math;
				int english;
				int cs;
				string value;
				ID = get_string("请输入学号：");
				name = get_string("请输入姓名：");
				math = get_number("请输入数学成绩：");
				english = get_number("请输入英语成绩：");
				cs = get_number("请输入计算机成绩：");
				value = name + " " + change(math) + " " + change(english) + " " + change(cs);
				if (db->db_replace(ID, value))
					cout << "替换成功！\n";
				else
					cout << "学号不存在！\n";
				break;
			}
			case(4) :
			{
				string ID;
				string name, value;
				int math;
				int english;
				int cs;
				ID = get_string("请输入学号：");
				if ((value = db->db_search(ID)) != "")
				{
					stringstream ss(value);
					ss >> name;
					ss >> math;
					ss >> english;
					ss >> cs;
					cout << "姓名：" << name << endl;
					cout << "数学成绩：" << math << endl;
					cout << "英语成绩：" << english << endl;
					cout << "计算机成绩：" << cs << endl;
				}
				else
					cout << "学号不存在！\n";
				break;
			}
			case(5) :
			{
				get_info(db);
				break;
			}
			case(6) :
			{
				reset_info(db);
				break;
			}
			case(7) :
			{
				test(db);
				break;
			}
			case(8) :
			{
				db_close(db);
				break;
			}
			default:
			{
				cout << "输入错误，请重输！\n";
				break;
			}
			}
			if (number == 7 || number == 8)
			{
				break;
			}
		}
		if (number == 8)
			break;
	}
}

void main_interface()
{
	while (1)
	{
		system("cls");
		cout << "欢迎登陆数据库系统\n";
		cout << "------------------\n";
		cout << "\n";
		cout << "打开菜单\n";
		cout << "\n";
		cout << "1--新建数据库" << endl;
		cout << "2--打开数据库" << endl;
		cout << "3--设置" << endl;
		cout << "4--联系我们" << endl;
		cout << "5--退出" << endl;
		while (1)
		{
			cout << "\n";
			int number;
			bool file = true;
			number = get_number();
			switch (number)
			{
			case(1) :
			{
				string name;
				name = get_string("请输入新建数据库的名称：");
				Database* db = db_new(name);
				if (db == NULL)
				{
					cout << "文件已存在！\n";
					file = false;
				}
				else
				{
					file = true;
					open_menu(db);
				}
				break;
			}
			case(2) :
			{
				string name;
				name = get_string("请输入要打开数据库的名称：");
				Database* db = db_open(name);
				if (db == NULL)
				{
					cout << "文件不存在或缺失！\n";
					file = false;
				}
				else
				{
					file = true;
					open_menu(db);
				}
				break;
			}
			case(3) :
			{
				setting();
				break;
			}
			case(4) :
			{
				cout << "以下是软件开发者信息：\n";
				cout << "软件开发者：陈俊\n";
				cout << "班级：F1403703\n";
				cout << "学号：5140379064\n";
				cout << "邮箱：897162163@qq.com\n";
				cout << "手机：13588808229\n";
				break;
			}
			case(5) :
				exit(0);
				break;
			default:
			{
				cout << "输入错误，请重输！\n";
				break;
			}
			}
			if ((number == 3) || ((number == 1 || number == 2) && (file == true)))
				break;
		}
	}
}
