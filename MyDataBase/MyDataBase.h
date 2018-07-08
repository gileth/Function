#pragma once
#include <mysql/mysql.h>
#include <string>

class MyDataBase
{
private:
	MYSQL *sql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	bool connect_flag;
	void showres();
public:
	MyDataBase();
	MyDataBase(MYSQL *mysql);
	~MyDataBase();
	void connect(const std::string host, const std::string user, const std::string password, 
					const std::string database = "mysql", unsigned int port = 0, 
					const char *unix_socket = nullptr, unsigned long client_flag = 0);
	void disconnect();
	void showdb();
	void createdb(const std::string &database);
	void usedb(const std::string &database);
	void deletedb(const std::string &database);
	void showtb();
	void createtb(const std::string &table, const std::string &elements);
	void selectitem(const std::string &table, const std::string &value);
	void insertitem(const std::string &table, const std::string &value);
	void insertitem(const std::string &table, const std::string &value, const std::string &col);
	void deleteitem(const std::string &table, const std::string &value);
	void query(const std::string &command);
};