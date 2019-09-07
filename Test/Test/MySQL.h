#pragma once
#include <iostream>
#include <WinSock2.h>
#include <mysql.h>
#include <string>
#include <vector>
using namespace std;
class MySQL {
public:
	MySQL();
	//连接数据库
	bool ConnectionMySql(const char* host,const char* user,const char* passward,const char* dbName,int port=3306);
	~MySQL();
	//插入数据
	bool Insert(const string& strSQL);
	//删除数据
	bool Delete(const string& strSQL);
	//修改数据
	bool Update(const string& strSQL);
	size_t GetCount(const string& strSQL);
	//查找
	vector<vector<string>> Select(const string& strSQL);

	//切换数据库
	bool SelectDB(const string& daName);
public:
	MYSQL* _mysql;
	string _dbName;
	vector<string> _tables;
};