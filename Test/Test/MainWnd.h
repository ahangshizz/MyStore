#pragma once

#include "Common.h"
#include "MySQL.h"
//给一个主窗口,让他继承自DUiLib库里的基类



class MainWnd : public WindowImplBase {
private:
	//基类中的三个纯虚函数,把纯虚函数实现

	//得到xml文件目录函数
	CDuiString GetSkinFolder();
	//得到xml文件名字函数
	CDuiString GetSkinFile();
	//窗口类的名字
	LPCTSTR GetWindowClassName(void) const;//const修饰的类成员函数,放在函数参数列表之后,函数体之前,表示类的this指针是一个常量,不能修改类的成员变量
	//消息响应
	void Notify(TNotifyUI& msg);
	//查询员工信息
	void SelectEmployeeInfo();
	//删除员工信息
	void DeleteEmployeeInfo();
	//插入员工信息
	void InsertEmployeeInfo();
	//更新员工信息
	void UpDateEmployeeInfo();
	//查询商品信息
	void SelectGoodsInfo();
	//删除商品信息
	void DeleteGoodsInfo();
	//插入商品信息
	void InsertGoodsInfo();
	//更新商品信息
	void UpDateGoodsInfo();
public:
	MySQL* p_mysql;

};

//字符转码
string UnicodeToAsc(const CDuiString& str);

CDuiString AscToUnicode(const string& str);