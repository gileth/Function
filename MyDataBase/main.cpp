#include "MyDataBase.h"
#include <vector>
using namespace std;

int main(int argc, char const *argv[])
{
	MyDataBase mdb;
	mdb.connect("localhost", "root", "1234");
	mdb.createdb("mydb");
	mdb.usedb("mydb");
	mdb.createtb("tst", "id int default 0, password varchar(255) default \"000000\"");
	mdb.insertitem("tst", "12345, \"hshsh\"");
	mdb.insertitem("tst", "\"hshsh\"", "password");
	mdb.insertitem("tst", "12346", "id");
	mdb.updateitem("tst", "password = 99999", "id = 12346");
	vector<vector<string>> ret = mdb.selectitem("tst", "*");
	for (auto temp: ret) {
		for (auto &str: temp)
			cout << str << " ";
		cout << endl;
	}
	mdb.deleteitem("tst", "id = 0");
	ret = mdb.selectitem("tst", "*");
	for (auto temp: ret) {
		for (auto &str: temp)
			cout << str << " ";
		cout << endl;
	}
	mdb.deletedb("mydb");
	mdb.disconnect();
	return 0;
}