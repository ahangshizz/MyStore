# MyStore 管理系统

#  功能

 - 连接MySQL管理信息
 - 封装MySQL接口自动完成SQL语句拼接并执行
 - 窗口界面支持优雅的操作
 - 登录系统不同权限不同界面
 - 对员工信息的增删改查
 - 对商品信息的增删改查
 - 可视化收银界面

#  项目概述
项目基于Duilib第三方库,以及对MySQL等函数的封装实现的一个超市商品及员工管理工具


<font size=4>**XML部分控件**</font>


|控件名|作用|
|---|---|---|
|VerticalLayout|纵向布局控件|
|HorizaontalLayout|水平布局控件|
|Button|按钮控件响应点击消息|
|Option和TabLayout |两控件一起使用实现多页面|
|List|在界面中显示一个列表|
|Edit|显示一个编辑框|
|Label和Text|放文字信息|
|Control|所用控件的基类可以用来放图片|
|ComboBox|下拉框控件|


<font size=4>**消息响应类型**</font>

|消息| 说明 | Sender|
|--|--|--|
| click | 鼠标点击 |CButtonUI|
| selectchanged| 变更选项| COptionUI|

<font size=4>**消息处理机制**</font>

**1.注册消息处理函数**
在CWindowWnd的Create()函数(创建窗口函数)中调用RegisterSuperclass(),注册消息回调函数(__WndProc);

**2.消息分发**
消息回调函数(处理所有系统发送的消息),通过调用HandleMessage()对消息进行分发

**3.消息循环**
在CPaintManagerUI类的MessageLoop处理消息循环;
接收到消息后,进入消息回调函数(__WndProc)

**4.处理消息**
通过CPaintManagerUI:: SendNotify回调控件注册的事件,最后通过Notify获取控件名称并进入相应的函数

#  部分函数实现
<font size=4>**管理员窗口Notify的实现**</font>

```cpp
void MainWnd::Notify(TNotifyUI& msg) {
	CDuiString StrName = msg.pSender->GetName();
	if (msg.sType == _T("click")) { 
		if (StrName == _T("BTN_CLOSE")) {	
			Close();
		}
		else if (StrName == _T("BTN_MIN")) {
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE,0);
		}
		else if (StrName == _T("BTN_SELECT")) {
			SelectEmployeeInfo();
		}
		else if (StrName == _T("BTN_DELETE")) {
			DeleteEmployeeInfo();
		}
		else if (StrName == _T("BTN_UPDATE")) {
			UpDateEmployeeInfo();
		}
		else if (StrName == _T("BTN_INSERT")) {
			InsertEmployeeInfo();
		}
		else if (StrName == _T("BTN_RECORD")) {
			MessageBox(NULL, _T("record"), _T("cashier"), IDOK);
		}
		else if (StrName == _T("BTN_GOODS_SELECT")) {
			SelectGoodsInfo();
		}
		else if (StrName == _T("BTN_GOODS_DELETE")) {
			DeleteGoodsInfo();
		}
		else if (StrName == _T("BTN_GOODS_INSERT")) {
			InsertGoodsInfo();
		}
		else if (StrName == _T("BTN_GOODS_UPDATE")) {
			UpDateGoodsInfo();
		}
	}
	else if (msg.sType == _T("selectchanged")) {	
		CTabLayoutUI* pTab =(CTabLayoutUI*)m_PaintManager.FindControl(_T("tablayout"));
		if (StrName == _T("OPTION_EMPLOYEE")) {
			pTab->SelectItem(0);
		}
		else if (StrName == _T("OPTION_GOODS"))
			pTab->SelectItem(1);
		else
			pTab->SelectItem(2);
	}
}
```
<font size=4>**登录窗口的创建**</font>

```cpp
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	LogInWnd loginWnd;
	loginWnd.Create(NULL, _T("LOGINWND"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	loginWnd.CenterWindow();
	loginWnd.ShowModal();
	return 0;
}
```


#  项目扩展
1.在本次设计当中还有很多地方需要去完善,应该更加贴近生活,使用户交互更加的人性化,比如在设计当中应该添加会员管理界面,并能实现对会员信息的增删查改,以及对会员等级的自动升级
2.在实现过程当中未考虑到多用户同时操作带来的线程安全问题
