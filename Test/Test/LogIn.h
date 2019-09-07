
#include "Common.h"
#include "MySQL.h"
class LogInWnd :public WindowImplBase {
protected:
	//登录界面同样继承  窗口类这个基类, 也需要实现这三个纯虚函数
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const ;
	void LogIn();
public:
	MySQL* p_mysql;
	//继承于基类的响应消息的函数
	void Notify(TNotifyUI& msg);
};