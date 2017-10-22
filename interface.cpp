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
				cout << "������������䣡\n" << endl;
			else
			{
				if ((ss >> std::ws).eof())
					break;
				else
					cout << "������������䣡\n" << endl;
			}
		}
		else
			cout << "������������䣡\n" << endl;
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
				cout << "������������䣡\n" << endl;
		}
		else
			cout << "������������䣡\n" << endl;
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
	cout << "����ֵ��׼���ã�\n";
	clock_t prepared = clock();
	cout << "׼��ʱ�䣺" << (prepared - begin) / CLOCKS_PER_SEC << "s" << endl;
	for (int i = 0; i < size; i++)
		db->db_insert(keys[i + old_size], values[i]);
	clock_t end = clock();
	cout << "����ʱ�䣺" << (end - prepared) / CLOCKS_PER_SEC << "s" << endl;
}

void random_search(vector<string>&keys, Database* db)
{
	clock_t begin = clock();
	for (int i = 0; i < keys.size(); i++)
		db->db_search(keys[i]);
	clock_t end = clock();
	cout << "����ʱ�䣺" << (end - begin) / CLOCKS_PER_SEC << "s" << endl;
}

void random_delete(vector<string>&keys, Database*db)
{
	clock_t begin = clock();
	for (int i = 0; i < keys.size(); i++)
		db->db_delete(keys[i]);
	clock_t end = clock();
	cout << "ɾ��ʱ�䣺" << (end - begin) / CLOCKS_PER_SEC << "s" << endl;
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
	cout << "����ֵ��׼���ã�\n";
	clock_t prepared = clock();
	cout << "׼��ʱ�䣺" << (prepared - begin) / CLOCKS_PER_SEC << "s" << endl;
	for (int i = 0; i < keys.size(); i++)
		db->db_replace(keys[i], values[i]);
	clock_t end = clock();
	cout << "�滻ʱ�䣺" << (end - prepared) / CLOCKS_PER_SEC << "s" << endl;
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
	size = get_number("���������������");
	clock_t begin = clock();
	for (int i = 0; i < size; i++)
	{
		keys.push_back(make_key());
		values.push_back(make_value());
	}
	clock_t prepared = clock();
	cout << "����ֵ��׼���ã�\n";
	cout << "׼��ʱ�䣺" << (prepared - begin) / CLOCKS_PER_SEC << "s" << endl;
	for (int i = 0; i < size; i++)
		db->db_insert(keys[i], values[i]);
	clock_t step1 = clock();
	cout << "����ʱ�䣺" << (step1 - prepared) / CLOCKS_PER_SEC << "s" << endl;
	for (int i = 0; i < size; i++)
		db->db_search(keys[i]);
	clock_t step2 = clock();
	cout << "����ʱ�䣺" << (step2 - step1) / CLOCKS_PER_SEC << "s" << endl;
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
	cout << "ѭ��ʱ�䣺" << (step3 - step2) / CLOCKS_PER_SEC << "s" << endl;
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
	cout << "ɾ��ʱ�䣺" << (step4 - step3) / CLOCKS_PER_SEC << "s" << endl;
	cout << "������ʱ�䣨������׼������" << (step4 - prepared) / CLOCKS_PER_SEC << "s" << endl;
}

void test(Database*db)
{
	system("cls");
	cout << "��ǰ�򿪵����ݿ�Ϊ��" << db->name << endl;
	cout << "--------------------";
	for (int i = 0; i < db->name.size(); i++)
		cout << "-";
	cout << "\n\n";
	cout << "���Բ˵�\n";
	cout << "\n";
	cout << "1--�����������\n";
	cout << "2--�����������\n";
	cout << "3--����滻���ݣ���ͬ���ȣ�\n";
	cout << "4--����滻���ݣ��������ȣ�\n";
	cout << "5--���ɾ������\n";
	cout << "6--�ۺϲ���\n";
	cout << "7--ͳ����Ϣ\n";
	cout << "8--����ͳ����Ϣ\n";
	cout << "9--������һ��\n";
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
			size = get_number("���������������");
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
			cout << "������������䣡\n";
			break;
		}
		}
		if (number == 9)
			break;
	}
}

void get_info(Database* db)
{
	cout << "ͳ����Ϣ\n";
	cout << "����ɹ���" << db->insok << endl;
	cout << "����ʧ�ܣ�" << db->inserr << endl;
	cout << "ɾ���ɹ���" << db->delok << endl;
	cout << "ɾ��ʧ�ܣ�" << db->delerr << endl;
	cout << "�滻�ɹ���" << db->repok << endl;
	cout << "�滻ʧ�ܣ�" << db->reperr << endl;
	cout << "���ҳɹ���" << db->seaok << endl;
	cout << "����ʧ�ܣ�" << db->seaerr << endl;
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
	cout << "���óɹ���\n";
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
	cout << "���ò˵�\n";
	cout << "\n";
	cout << "1--Hash����\n";
	cout << "2--�����������Զ�����Hash����\n";
	cout << "3--�����Զ�������\n";
	cout << "4--�ָ�Ĭ������\n";
	cout << "5--������һ��\n";
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
			cout << "��ǰHash������Ϊ��" << hash_size << endl;
			size = get_number("�����������޸ĵĴ�С:");
			hash_size = size;
			cout << "�޸ĳɹ���\n";
			break;
		}
		case(2) :
		{
			size_t datasize;
			cout << "��ǰHash������Ϊ��" << hash_size << endl;
			datasize = get_number("����������Ҫ�������������");
			hash_size = find_prime_number(2 * datasize);
			cout << "�޸ĳɹ���\n";
			break;
		}
		case(3) :
		{
			int input_size;
			cout << "��ǰ�����Զ�������Ϊ��" << auto_input_size << endl;
			input_size = get_number("�����������޸ĵ�ֵ��");
			auto_input_size = input_size;
			cout << "�޸ĳɹ���\n";
			break;
		}
		case(4) :
		{
			hash_size = DE_HASH_SIZE;
			auto_input_size = DE_AUTO_INPUT_SIZE;
			cout << "�ѻָ�Ĭ�����ã�\n";
			break;
		}
		case(5) :
			break;
		default:
		{
			cout << "������������䣡\n";
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
		cout << "��ǰ�򿪵����ݿ�Ϊ��" << db->name << endl;
		cout << "--------------------";
		for (int i = 0; i < db->name.size(); i++)
			cout << "-";
		cout << "\n\n";
		cout << "�����˵�\n";
		cout << "\n";
		cout << "1--�������\n";
		cout << "2--ɾ������\n";
		cout << "3--�޸�����\n";
		cout << "4--��ѯ\n";
		cout << "5--ͳ����Ϣ\n";
		cout << "6--����ͳ����Ϣ\n";
		cout << "7--���ܲ���\n";
		cout << "8--�ر����ݿ�\n";
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
				ID = get_string("������ѧ�ţ�");
				name = get_string("������������");
				math = get_number("��������ѧ�ɼ���");
				english = get_number("������Ӣ��ɼ���");
				cs = get_number("�����������ɼ���");
				value = name + " " + change(math) + " " + change(english) + " " + change(cs);
				if (db->db_insert(ID, value))
					cout << "����ɹ���\n";
				else
					cout << "ѧ���Ѵ��ڣ�\n";
				break;
			}
			case(2) :
			{
				string ID;
				ID = get_string("������ѧ�ţ�");
				if (db->db_delete(ID))
					cout << "ɾ���ɹ���\n";
				else
					cout << "ѧ�Ų����ڣ�\n";
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
				ID = get_string("������ѧ�ţ�");
				name = get_string("������������");
				math = get_number("��������ѧ�ɼ���");
				english = get_number("������Ӣ��ɼ���");
				cs = get_number("�����������ɼ���");
				value = name + " " + change(math) + " " + change(english) + " " + change(cs);
				if (db->db_replace(ID, value))
					cout << "�滻�ɹ���\n";
				else
					cout << "ѧ�Ų����ڣ�\n";
				break;
			}
			case(4) :
			{
				string ID;
				string name, value;
				int math;
				int english;
				int cs;
				ID = get_string("������ѧ�ţ�");
				if ((value = db->db_search(ID)) != "")
				{
					stringstream ss(value);
					ss >> name;
					ss >> math;
					ss >> english;
					ss >> cs;
					cout << "������" << name << endl;
					cout << "��ѧ�ɼ���" << math << endl;
					cout << "Ӣ��ɼ���" << english << endl;
					cout << "������ɼ���" << cs << endl;
				}
				else
					cout << "ѧ�Ų����ڣ�\n";
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
				cout << "������������䣡\n";
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
		cout << "��ӭ��½���ݿ�ϵͳ\n";
		cout << "------------------\n";
		cout << "\n";
		cout << "�򿪲˵�\n";
		cout << "\n";
		cout << "1--�½����ݿ�" << endl;
		cout << "2--�����ݿ�" << endl;
		cout << "3--����" << endl;
		cout << "4--��ϵ����" << endl;
		cout << "5--�˳�" << endl;
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
				name = get_string("�������½����ݿ�����ƣ�");
				Database* db = db_new(name);
				if (db == NULL)
				{
					cout << "�ļ��Ѵ��ڣ�\n";
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
				name = get_string("������Ҫ�����ݿ�����ƣ�");
				Database* db = db_open(name);
				if (db == NULL)
				{
					cout << "�ļ������ڻ�ȱʧ��\n";
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
				cout << "�����������������Ϣ��\n";
				cout << "��������ߣ��¿�\n";
				cout << "�༶��F1403703\n";
				cout << "ѧ�ţ�5140379064\n";
				cout << "���䣺897162163@qq.com\n";
				cout << "�ֻ���13588808229\n";
				break;
			}
			case(5) :
				exit(0);
				break;
			default:
			{
				cout << "������������䣡\n";
				break;
			}
			}
			if ((number == 3) || ((number == 1 || number == 2) && (file == true)))
				break;
		}
	}
}
