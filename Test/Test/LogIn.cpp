#include "LogIn.h"
#include "MainWnd.h"
#include "Cashier.h"
CDuiString LogInWnd::GetSkinFolder() {
	return _T("");
}
CDuiString LogInWnd::GetSkinFile() {
	return _T("LogInWnd.xml");
}
LPCTSTR LogInWnd::GetWindowClassName(void) const {
	return _T("LogInWnd");
}

//消息捕获处理
void LogInWnd::Notify(TNotifyUI& msg) {



	//tagTNotifyUI结构体里面封装了一个 CControlUI* pSender类型的指针 CControlUI类里面封装了获取空间名称的方法
	CDuiString StrName = msg.pSender->GetName();

	//对消息进行拦截  ,msg.sType 是消息的种类   ,拦截的是点击消息
	if (msg.sType == _T("click")) {
		if (StrName == _T("Btn_MIN"))
			//MessageBox(NULL, _T("MIN"), _T("Login"), IDOK); //一个测试文本,MessageBox显示一个对话框
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		else if (StrName == _T("Btn_CLOSE"))
			Close();
		else if (StrName == _T("Btn_LOGIN"))
			LogIn();
	}
}

void LogInWnd::LogIn() {
	//从编辑框中取出用户名和密码,到时进行与数据库中的数据进行对比
	CEditUI* pUserName = (CEditUI*)m_PaintManager.FindControl(_T("Edit_USER_NAME"));//从用户名的空间里面找用户输入的用户名
	CDuiString StrUserName = pUserName->GetText();	//拿到用户输入的用户名

	//获取用户密码
	CEditUI* pUserPassword = (CEditUI*)m_PaintManager.FindControl(_T("Edit_USER_PASSWORD"));
	CDuiString StrUserPassword = pUserPassword->GetText();

	//检测用户是否输入了用户名和密码,如果没输入,提醒用户输入
	if (StrUserName.IsEmpty()) {
		MessageBox(NULL, _T("请输入用户名"), _T("警告"), IDOK);
		return;
	}
	if (StrUserPassword.IsEmpty()) {
		MessageBox(NULL, _T("请输入密码"), _T("警告"), IDOK);
		return;
	}

	MySQL mysql;
	p_mysql = &mysql;
	p_mysql->ConnectionMySql("127.0.0.1", "root", "", "weihang");

	//根据用户名和密码,查找数据库,看与数据库中的数据是否匹配
	string StrSQL("select * from employee where name='");
	//AscToUnicode
	StrSQL += UnicodeToAsc(StrUserName);
	StrSQL += "';";
	vector<vector<string>> vRet = p_mysql->Select(StrSQL);
	if (vRet.empty()) {
		MessageBox(m_hWnd, _T("用户名错误"), _T("警告"), IDOK);
		return;
	}

	string password = UnicodeToAsc(StrUserPassword);
	if (password != vRet[0][4]) {
		MessageBox(m_hWnd, _T("用户名密码错误"), _T("警告"), IDOK);
		return;
	}


	//如果匹配成功
	//隐藏登录界面
	ShowWindow(false);
	if (vRet[0][5] == "管理员") {
		//创建主窗口
		MainWnd mainWnd;
		mainWnd.Create(NULL, _T("MainWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
		mainWnd.p_mysql = p_mysql;//把MySQL的赋值
		mainWnd.CenterWindow();
		mainWnd.ShowModal();
	}
	else {
		//创建销售员总窗口
		
		CashierWnd CashierWnd;
		CashierWnd.Create(NULL, _T("CashierWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
		CashierWnd.p_mysql = p_mysql;
		CashierWnd.CenterWindow();
		CashierWnd.ShowModal();
	}
	//tablayoue  
}